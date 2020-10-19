from libmahjong import EventType
from .gameevent import Event, CombinedEventType
from .meld import Meld
from .call import Call, CallDirection

class BoardManager():
  def __init__(self):
    self.last_event = None
    self.last_decision_event = None
    self.waiting_on_decision = False
    self.round_should_end = False
    self.game_should_end = False

def process_event_queue(game_manager, match, current_queue):
  if (len(current_queue) == 0):
    return
  event = current_queue.pop(0)
  if event.type == EventType.Discard and len(current_queue) > 0:
    next_event = current_queue.pop(0)
    if next_event.type in (EventType.Pon, EventType.Ron, EventType.Kan, EventType.Chi) and event.player == next_event.player:
      new_event = Event(CombinedEventType(100 + int(next_event.type)), event.player, event.piece, next_event.decision, extra_player=next_event.player, extra_piece=next_event.piece, raw_event_a=event, raw_event_b=next_event)
      process_event_queue(game_manager, match, [new_event] + current_queue)
      return
    current_queue = [next_event] + current_queue
  on_game_event(game_manager, event, match)
  process_event_queue(game_manager, match, current_queue)

def on_game_event(game_manager, event, match):
  print("===NEW EVENT===")
  print(event.type, "Int Value:", int(event.type))
  print(event.player)
  print(event.piece)
  print(event.decision)
  is_ai = event.player != match.player_id
  is_decision = event.decision
  event_type = event.type

  actual_player = match.current_board.players[match.player_id]

  if event.player is not None or event.player in range(4):
    player = match.current_board.players[event.player]
  else:
    player = None

  if hasattr(event, 'extra_player'):
    if event.extra_player is not None or event.extra_player in range(4):
      extra_player = match.current_board.players[event.extra_player]
    else:
      extra_player = None

  # Align Turn
  if (event_type in (EventType.Discard, CombinedEventType.DiscardChi, CombinedEventType.DiscardKan, CombinedEventType.DiscardPon, CombinedEventType.DiscardRon)):
    match.current_board.current_turn = event.player

  if (event_type == EventType.Chi):
    if not is_decision:
      last_event = game_manager.board_manager.last_decision_event
      del match.current_board.players[last_event.player].discard_pile[-1]
      player.hand += [last_event.extra_piece]

      game_manager.board_manager.last_decision_event = None
      game_manager.board_manager.waiting_on_decision = False

      match.current_board.current_turn = event.player

      if is_ai:
        del player.hand[-3:]
      else:
        player.hand.remove(event.piece)
        player.hand.remove(event.piece + 1)
        player.hand.remove(event.piece + 2)

      player.melded_hand += [Meld([event.piece, event.piece + 1, event.piece + 2], CallDirection.Left)]
      player.calls_avaliable = []

  elif (event_type == EventType.Pon):
    if not is_decision:
      last_event = game_manager.board_manager.last_decision_event
      del match.current_board.players[last_event.player].discard_pile[-1]

      if is_ai:
        del player.hand[-2:]
      else:
        player.hand.remove(event.piece)
        player.hand.remove(event.piece)

      match.current_board.current_turn = event.player
      player.melded_hand += [Meld([event.piece] * 3, CallDirection.get_call_direction(event.player, last_event.player))]
      player.calls_avaliable = []

      game_manager.board_manager.last_decision_event = None
      game_manager.board_manager.waiting_on_decision = False

  elif (event_type == EventType.Ron):
    if not is_decision:
      player.hand += [event.piece]
      player.calls_avaliable = []
      game_manager.board_manager.waiting_on_decision = False
      game_manager.board_manager.round_should_end = True

  elif (event_type == EventType.ConvertedKan):
    if is_decision:
      game_manager.board_manager.waiting_on_decision = True
      game_manager.board_manager.last_decision_event = event
      player.calls_avaliable += [Call.Kan]
    else:
      # Converted Kan
      for melds in player.melded_hand:
        if (melds.tiles == [event.piece] * 3):
          melds.tiles += [event.piece]
          melds.converted_kan = True
          break

      player.hand.remove(event.piece)
      player.calls_avaliable = []
      game_manager.board_manager.last_decision_event = None
      game_manager.board_manager.waiting_on_decision = False

  elif (event_type == EventType.Kan):
    if not is_decision:
      # Calling Kan
      last_event = game_manager.board_manager.last_decision_event
      del match.current_board.players[last_event.player].discard_pile[-1]

      match.current_board.current_turn = event.player

      player.melded_hand += [Meld([event.piece] * 4, CallDirection.get_call_direction(event.player, last_event.player))]

      if is_ai:
        del player.hand[-3:]
      else:
        player.calls_avaliable = []
        player.hand.remove(event.piece)
        player.hand.remove(event.piece)
        player.hand.remove(event.piece)
        game_manager.board_manager.last_decision_event = None
        game_manager.board_manager.waiting_on_decision = False

  elif (event_type == EventType.Tsumo):
    if is_decision:
      game_manager.board_manager.waiting_on_decision = True
      player.calls_avaliable += [Call.Tsumo]
    else:
      player.calls_avaliable = []
      game_manager.board_manager.waiting_on_decision = False
      game_manager.board_manager.round_should_end = True

  elif (event_type == EventType.Riichi):
    if is_decision:
      game_manager.board_manager.waiting_on_decision = True
      player.calls_avaliable += [Call.Riichi]
    else:
      if is_ai:
        del player.hand[-1]
      else:
        player.hand.remove(event.piece)
      game_manager.board_manager.waiting_on_decision = False
      player.calls_avaliable += []
      player.discard_pile += [event.piece]
      player.riichi_declared = True
      match.current_board.next_turn()

  elif (event_type == EventType.Discard):
    if is_decision:
      if game_manager.board_manager.last_event is None or game_manager.board_manager.last_event.type not in (EventType.Chi, EventType.Pon, EventType.Kan):
        player.hand += [event.piece]
        if len(match.current_board.wall) > 0:
          del match.current_board.wall[-1]
      player.my_turn = True
      game_manager.board_manager.waiting_on_decision = True
    else:
      if not is_ai:
        player.hand.remove(event.piece)
        player.hand.sort()
      else:
        if len(match.current_board.wall) > 0:
          del match.current_board.wall[-1]
      player.discard_pile += [event.piece]

  elif (event_type == EventType.ConcealedKan):
    if is_decision:
      game_manager.board_manager.waiting_on_decision = True
      player.calls_avaliable += [Call.Concealed_Kan]
    else:
      if is_ai:
        del player.hand[-4:]
      else:
        player.hand.remove(event.piece)
        player.hand.remove(event.piece)
        player.hand.remove(event.piece)
        player.hand.remove(event.piece)

      game_manager.board_manager.waiting_on_decision = False
      player.calls_avaliable = []
      player.melded_hand += [Meld([event.piece] * 4, CallDirection.Concealed)]

  elif (event_type == EventType.Decline):
    return

  elif (event_type == EventType.Dora):
    match.current_board.deadwall[match.current_board.dora_revealed * 2] = event.piece
    match.current_board.dora_revealed += 1

  elif (event_type == EventType.PointDiff):
    match.scores[event.player] += event.piece * 1000
    game_manager.board_manager.round_should_end = True

  elif (event_type == EventType.End):
    game_manager.board_manager.game_should_end = True

  elif (event_type == CombinedEventType.DiscardChi):
    if (is_decision):
      player.discard_pile += [event.piece]

      game_manager.board_manager.last_decision_event = event
      game_manager.board_manager.waiting_on_decision = True
      actual_player.calls_avaliable += [Call.Chi]
    else:
      del extra_player.hand[-2:]
      chi_tile = event.extra_piece
      extra_player.melded_hand += [Meld([chi_tile, chi_tile + 1, chi_tile + 2], CallDirection.Left)]
      match.current_board.current_turn = event.extra_player

  elif (event_type == CombinedEventType.DiscardKan):
    if (is_decision):
      player.discard_pile += [event.piece]

      game_manager.board_manager.last_decision_event = event
      game_manager.board_manager.waiting_on_decision = True
      actual_player.calls_avaliable += [Call.Kan]
    else:
      del extra_player.hand[-3:]
      kan_tile = event.extra_piece

      extra_player.melded_hand += [Meld([kan_tile] * 4, CallDirection.get_call_direction(event.player, event.extra_player))]
      match.current_board.current_turn = event.extra_player

  elif (event_type == CombinedEventType.DiscardPon):
    if (is_decision):
      player.discard_pile += [event.piece]

      game_manager.board_manager.last_decision_event = event
      game_manager.board_manager.waiting_on_decision = True
      actual_player.calls_avaliable += [Call.Pon]
    else:
      del extra_player.hand[-2:]
      pon_tile = event.extra_piece
      extra_player.melded_hand += [Meld([pon_tile] * 3, CallDirection.get_call_direction(event.extra_player, event.player))]
      match.current_board.current_turn = event.extra_player

  elif (event_type == CombinedEventType.DiscardRon):
    if (is_decision):
      player.discard_pile += [event.piece]

      game_manager.board_manager.last_decision_event = event
      game_manager.board_manager.waiting_on_decision = True
      actual_player.calls_avaliable += [Call.Ron]
    else:
      extra_player.hand += [event.piece]
      game_manager.board_manager.waiting_on_decision = False
      game_manager.board_manager.round_should_end = True
  game_manager.board_manager.last_event = event

