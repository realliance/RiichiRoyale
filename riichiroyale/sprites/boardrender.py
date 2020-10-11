import copy
from pygame.sprite import Group
from .elements import render_center_info, render_hand, render_meld_hand, render_discard_pile, render_vertical_discard_pile, render_dora_pile, render_hidden_hand

class StatefulBoardElement():
  def __init__(self, default_value, get_value, render):
    self.value = copy.deepcopy(default_value)
    self.get_value = get_value
    self.render = render
    self.rendered = Group()

  def notify(self, surface, background):
    if self.value != self.get_value():
      self.rendered.clear(surface, background)
      self.value = copy.deepcopy(self.get_value())
      self.rendered = self.render()
  
  def force_render(self):
    self.rendered = self.render()

  def draw(self, surface, background):
    self.rendered.clear(surface, background)
    self.rendered.draw(surface)

  def update(self):
    self.rendered.update()

class BoardRender():
  def __init__(self, small_dictionary, dictionary, surface, board, player_pov):
    self.board = board
    self.surface = surface
    self.small_dictionary = small_dictionary
    self.dictionary = dictionary
    self.player_pov = player_pov
    self.elements = []

    # Player Hand
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[player_pov].hand, lambda: render_hand(self, player_pov)))

    # Opponent Hands
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[1].hand, lambda: render_hidden_hand(self, 1, 1)))
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[2].hand, lambda: render_hidden_hand(self, 2, 2)))
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[3].hand, lambda: render_hidden_hand(self, 3, 3)))

    # Player Melds
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[player_pov].melded_hand, lambda: render_meld_hand(self, self.board.players[player_pov].melded_hand)))

    # Opponent Melds
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[player_pov].melded_hand, lambda: render_meld_hand(self, self.board.players[player_pov].melded_hand, seat=1)))
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[player_pov].melded_hand, lambda: render_meld_hand(self, self.board.players[player_pov].melded_hand, seat=2)))
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[player_pov].melded_hand, lambda: render_meld_hand(self, self.board.players[player_pov].melded_hand, seat=3)))


    # Discard Piles
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[0].discard_pile, lambda: render_discard_pile(self, 0)))
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[1].discard_pile, lambda: render_vertical_discard_pile(self, 1)))
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[2].discard_pile, lambda: render_discard_pile(self, 2)))
    self.elements.append(StatefulBoardElement([], lambda: self.board.players[3].discard_pile, lambda: render_vertical_discard_pile(self, 3)))

    # Center Info
    self.elements.append(StatefulBoardElement([], lambda: center_info_state(self.board), lambda: render_center_info(self)))

    # Doras
    self.elements.append(StatefulBoardElement(0, lambda: self.board.dora_revealed, lambda: render_dora_pile(self)))
    
  def update(self):
    # Notify elements to check state changes
    for element in self.elements:
      element.update()
    
  def force_redraw(self):
    for element in self.elements:
      element.force_render()

  def draw(self, background):
    for element in self.elements:
      element.notify(self.surface, background)
      element.draw(self.surface, background)

def center_info_state(board):
  return [len(board.wall), board.current_dealer, board.current_turn]