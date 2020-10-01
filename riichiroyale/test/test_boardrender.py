import unittest
from riichiroyale.game import Board
from riichiroyale.sprites import BoardRender, StatefulBoardElement

class MockRenderElement():
  def __init__(self):
    self.state_probe = 0

  def increment_state_probe(self, _):
    self.state_probe += 1

class TestBoardRender(unittest.TestCase):
  def test_stateful_board_elements(self):
    # Define board
    board = Board()

    # Define mock render element, to be mutated
    mock = MockRenderElement()

    # Define rendered board
    board_render = BoardRender(None, board, None)

    # Remove default elements for custom testing
    board_render.elements = []

    # Create custom element that increments a mock object when the board wall changes size
    board_element = StatefulBoardElement(len(board.wall), lambda b: len(b.wall), mock.increment_state_probe)
    board_render.elements.append(board_element)
    
    # No errant initial update
    self.assertEqual(mock.state_probe, 0)

    # Nothing changed, no update
    board_render.update()
    self.assertEqual(mock.state_probe, 0)

    # Update wall size, render should occur
    board.draw_tile()
    board_render.update()
    self.assertEqual(mock.state_probe, 1)

    # Should happen again
    board.draw_tile()
    board_render.update()
    self.assertEqual(mock.state_probe, 2)

    # Did not change, render did not occur
    board_render.update()
    self.assertEqual(mock.state_probe, 2)


