import math
from pygame import Rect
from libmahjong import MahjongGameManager, PythonAIInterface, EventType, EngineEvent
from riichiroyale import Tile
from riichiroyale.game import Player, Match, process_event_queue
from .boardview import BoardView

class GameView(BoardView):
  def __init__(self, game_manager, screen, tile_dict, small_tile_dict, screen_width, screen_height, width_ratio, height_ratio):
    super().__init__("game", game_manager, None, screen, tile_dict, small_tile_dict, screen_width, screen_height, width_ratio, height_ratio)
    self.player_ai_inst = None

  def on_match_init(self):
    if self.match is not None:
      if self.match.player_ai_inst is not None:
        self.match.player_ai_inst.ReceiveEvent()
    self.match = None
    self.match = Match(None, sound_manager=self.sound_manager, ai_managed=True)

  def on_pov_init(self):
    self.player = Player("Player")
    self.match.register_player(self.player)
    self.match.player_id = 0
    
    # Init Mahjong Game Engine
    MahjongGameManager.start_game(["PythonAIInterface"] + ["AngryDiscardoBot"] * 3, True)
    self.match.player_ai_inst = PythonAIInterface.Inst()
    self.match.player_id = self.match.player_ai_inst.GameStart()
    self.match.players[0].ai_managed = True
    self.match.register_player(Player("Bot 1", ai_managed=True))
    self.match.register_player(Player("Bot 2", ai_managed=True))
    self.match.register_player(Player("Bot 3", ai_managed=True))
    wall = [Tile.ERROR_PIECE] * 70
    deadwall = [Tile.ERROR_PIECE] * 14
    self.ai_game_active = True

    self.match.new_board(wall=wall, deadwall=deadwall)

  def get_play_area_pos(self, screen):
    w, h = screen.get_size()
    play_area_height = h
    player_area_width = math.floor(h * (float(self.screen_width_ratio)/self.screen_height_ratio))
    return Rect((w - player_area_width)//2, 0, player_area_width, play_area_height)

  def on_tile_pressed(self, owner, tile_hand_index):
    if owner.my_turn:
      tile = owner.hand[tile_hand_index]
      if self.game_manager.board_manager.waiting_on_decision and len(owner.calls_avaliable) == 0:
        self.match.current_board.play_clack()
        event = EngineEvent()
        event.type = EventType.Discard
        event.piece = int(tile)
        event.player = owner.player_id
        event.decision = True
        PythonAIInterface.Inst().RetrieveDecision(event)

  def on_pon_button_pressed(self):
    self.match.player_ai_inst.RetrieveDecision(self.game_manager.board_manager.last_decision_event.raw_event_b)
    return True

  def on_chi_button_pressed(self):
    self.match.player_ai_inst.RetrieveDecision(self.game_manager.board_manager.last_decision_event.raw_event_b)
    return True

  def on_kan_button_pressed(self):
    if self.game_manager.board_manager.last_decision_event.type == EventType.ConvertedKan:
      self.match.player_ai_inst.RetrieveDecision(self.game_manager.board_manager.last_decision_event)
    else:
      self.match.player_ai_inst.RetrieveDecision(self.game_manager.board_manager.last_decision_event.raw_event_b)
    return True

  def on_tsumo_button_pressed(self):
    self.match.player_ai_inst.RetrieveDecision(self.game_manager.board_manager.last_decision_event.raw_event_b)
    return True

  def on_riichi_button_pressed(self):
    self.match.player_ai_inst.RetrieveDecision(self.game_manager.board_manager.last_decision_event)
    return True
  
  def on_ron_button_pressed(self):
    self.match.player_ai_inst.RetrieveDecision(self.game_manager.board_manager.last_decision_event.raw_event_b)
    return True

  def on_skip_button_pressed(self):
    event = EngineEvent()
    event.type = EventType.Decline
    event.piece = -1
    event.player = -1
    event.decision = True
    PythonAIInterface.Inst().RetrieveDecision(event)
    return True

  def on_dialogue_event_ending(self, event_name):
    if event_name == 'round_end':
      self.game_manager.set_active_view('main_menu')
      self.game_manager.sound_manager.play_music('lobby')

  def _end_round_dialog(self):
    self.ai_game_active = False
    self.game_manager.board_manager.round_should_end = False
    self.dialogue_manager.register_dialog_event("round_end")
    self.dialogue_manager.append_dialog_event("round_end", ["Round Complete! Now for the Results..."])
    i = 0
    for score in self.match.scores:
      self.dialogue_manager.append_dialog_event("round_end", ["Player {0} was awarded {1} points!".format(i + 1, score)])
      i += 1
    self.dialogue_manager.append_dialog_event("round_end", ["Thank you for playing a demo match of Riichi Royale! Press Next to return to the main menu."])
    self.dialogue_manager.start_event('round_end')
    self.player.calls_avaliable = []
    for button in self.buttons:
      self.buttons[button].hide()

  def update(self, time_delta):
    queued_events = self.match.player_ai_inst.ReceiveEvent()
    if len(queued_events) != 0:
      process_event_queue(self.game_manager, self.match, queued_events)
    if self.game_manager.board_manager.round_should_end:
      self._end_round_dialog()

    super().update(time_delta)