import unittest
from riichiroyale.game import Player, Board, Call, Tile, pon_possible, kan_possible, concealed_kan_possible, chi_possible

class TestGameCalls(unittest.TestCase):
  def test_pon_possible(self):
    self.assertTrue(pon_possible([Tile.SEVEN_PIN, Tile.SEVEN_PIN], Tile.SEVEN_PIN))
    self.assertTrue(pon_possible([Tile.SEVEN_PIN, Tile.SEVEN_PIN, Tile.SEVEN_PIN], Tile.SEVEN_PIN))
    self.assertFalse(pon_possible([Tile.SEVEN_PIN], Tile.SEVEN_PIN))
    self.assertFalse(pon_possible([Tile.SEVEN_PIN, Tile.SEVEN_PIN], Tile.SEVEN_BAMBOO))

  def test_kan_possible(self):
    self.assertTrue(kan_possible([Tile.SEVEN_PIN, Tile.SEVEN_PIN, Tile.SEVEN_PIN], Tile.SEVEN_PIN))
    self.assertFalse(kan_possible([Tile.SEVEN_PIN, Tile.SEVEN_PIN], Tile.SEVEN_PIN))
    self.assertFalse(kan_possible([Tile.SEVEN_PIN, Tile.SEVEN_PIN, Tile.SEVEN_PIN], Tile.SEVEN_BAMBOO))

  def test_concealed_kan_possible(self):
    self.assertTrue(concealed_kan_possible([Tile.SEVEN_PIN, Tile.SEVEN_PIN, Tile.SEVEN_PIN, Tile.SEVEN_PIN]))
    self.assertFalse(concealed_kan_possible([Tile.SEVEN_PIN, Tile.SEVEN_PIN, Tile.SEVEN_PIN]))
    self.assertTrue(concealed_kan_possible([Tile.SEVEN_PIN, Tile.SEVEN_BAMBOO, Tile.SEVEN_PIN, Tile.SEVEN_PIN, Tile.SEVEN_PIN]))

  def test_chi_possible(self):
    self.assertTrue(chi_possible([Tile.ONE_PIN, Tile.TWO_PIN], Tile.THREE_PIN))
    self.assertTrue(chi_possible([Tile.THREE_PIN, Tile.TWO_PIN], Tile.ONE_PIN))
    self.assertTrue(chi_possible([Tile.TWO_PIN, Tile.FOUR_PIN], Tile.THREE_PIN))
    self.assertFalse(chi_possible([Tile.SOUTH_WIND, Tile.WEST_WIND], Tile.EAST_WIND))
    self.assertFalse(chi_possible([Tile.ONE_PIN, Tile.FOUR_PIN], Tile.THREE_PIN))

  def test_board_example_pon(self):
    player_one = Player("Player One", starting_hand=[Tile.ONE_PIN, Tile.TWO_PIN])

    # Notable Player: P2 has two One Bamboos
    player_two = Player("Player Two", starting_hand=[Tile.ONE_BAMBOO, Tile.ONE_BAMBOO])
    player_three = Player("Player Three", starting_hand=[Tile.ONE_CHARACTER, Tile.TWO_CHARACTER])
    player_four = Player("Player Four", starting_hand=[Tile.WEST_WIND, Tile.EAST_WIND])
    board = Board(players=[player_one, player_two, player_three, player_four])

    # P1 discards a One Bamboo
    player_one.discard_pile = [Tile.ONE_BAMBOO]
    board.on_discard(player_one)

    # P2 can call Pon
    self.assertEqual(player_two.calls_avaliable, [Call.Pon])

    # P2 has a decision to make, so turn number did not automatically increment
    self.assertEqual(board.current_turn, 0)

  def test_board_example_chi(self):
    player_one = Player("Player One", starting_hand=[Tile.ONE_PIN, Tile.TWO_PIN])

    # Notable Player: P2 has a One Bamboo and Two Bamboo
    player_two = Player("Player Two", starting_hand=[Tile.ONE_BAMBOO, Tile.TWO_BAMBOO])

    # Notable Player: P3 has a One Bamboo and Two Bamboo
    player_three = Player("Player Three", starting_hand=[Tile.ONE_BAMBOO, Tile.TWO_BAMBOO])
    player_four = Player("Player Four", starting_hand=[Tile.WEST_WIND, Tile.EAST_WIND])
    board = Board()

    board.register_player(player_one)
    board.register_player(player_two)
    board.register_player(player_three)
    board.register_player(player_four)

    # P1, who is sitting on P2's left, discards a Three Bamboo
    player_one.discard_pile = [Tile.THREE_BAMBOO]
    board.on_discard(player_one)

    # P2 can call Chi
    self.assertEqual(player_two.calls_avaliable, [Call.Chi])

    # P3 cannot call chi, as P1 is not sitting on their immediate left
    self.assertEqual(player_three.calls_avaliable, None)

    # P2 has a call decision, so don't auto increment the turn
    self.assertEqual(board.current_turn, 0)

  def test_board_example_kan(self):
    player_one = Player("Player One", starting_hand=[Tile.ONE_PIN, Tile.TWO_PIN])

    # Notable Player: P2 has three One Bamboos
    player_two = Player("Player Two", starting_hand=[Tile.ONE_BAMBOO, Tile.ONE_BAMBOO, Tile.ONE_BAMBOO])
    player_three = Player("Player Three", starting_hand=[Tile.ONE_CHARACTER, Tile.TWO_CHARACTER])
    player_four = Player("Player Four", starting_hand=[Tile.WEST_WIND, Tile.EAST_WIND])
    board = Board(players=[player_one, player_two, player_three, player_four])

    # P1 discards a One Bamboo
    player_one.discard_pile = [Tile.ONE_BAMBOO]
    board.on_discard(player_one)

    # P2 can call either a Pon or Kan
    self.assertEqual(player_two.calls_avaliable, [Call.Pon, Call.Kan])

    # P2 has a decision, so don't auto increment the turn
    self.assertEqual(board.current_turn, 0)

  def test_board_example_no_calls_avaliable(self):
    player_one = Player("Player One", starting_hand=[Tile.ONE_PIN, Tile.TWO_PIN])
    player_two = Player("Player Two", starting_hand=[Tile.RED_DRAGON])
    player_three = Player("Player Three", starting_hand=[Tile.ONE_CHARACTER, Tile.TWO_CHARACTER])
    player_four = Player("Player Four", starting_hand=[Tile.WEST_WIND, Tile.EAST_WIND])
    board = Board(players=[player_one, player_two, player_three, player_four])

    # Call does not aid anyone
    player_one.discard_pile = [Tile.ONE_BAMBOO]
    board.on_discard(player_one)

    # As no call decisions were avaliable, the turn automatically incremented
    self.assertEqual(board.current_turn, 1)

    # On a new turn, the next player auto draws
    self.assertEqual(len(player_two.hand), 2)