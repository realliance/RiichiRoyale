import unittest
from libmahjong import EventType, PieceType
from riichiroyale.game import (
    Player,
    BoardManager,
    Match,
    Event,
    process_event_queue,
    Call,
    CallDirection,
)
from riichiroyale.views import GameManager


class TestBoardManager(unittest.TestCase):
    def setUp(self):
        self.game_manager = GameManager(BoardManager(), None)
        self.match = Match(0)
        self.match.register_player(Player("Player"))
        self.match.register_player(Player("Bot 1"))
        self.match.register_player(Player("Bot 2"))
        self.match.register_player(Player("Bot 3"))

        self.match.players[0].hand = [
            PieceType.GREEN_DRAGON,
            PieceType.GREEN_DRAGON,
            PieceType.TWO_BAMBOO,
            PieceType.THREE_BAMBOO,
        ]
        self.match.players[1].hand = [PieceType.ERROR] * 13
        self.match.players[2].hand = [PieceType.ERROR] * 13
        self.match.players[3].hand = [PieceType.ERROR] * 13

        self.match.new_board(
            wall=[PieceType.ERROR] * 70, deadwall=[PieceType.ERROR] * 14
        )

    def tearDown(self):
        self.match = None
        self.game_manager = None

    def test_init_round(self):
        dora = Event(EventType.Dora, None, PieceType.WHITE_DRAGON, False)
        draw = Event(EventType.Discard, 0, PieceType.WHITE_DRAGON, True)

        process_event_queue(self.game_manager, self.match, [dora, draw])

        self.assertEqual(self.match.current_board.dora_revealed, 1)
        self.assertEqual(self.match.current_board.deadwall[0], PieceType.WHITE_DRAGON)
        self.assertEqual(
            self.match.current_board.players[0].hand[-1], PieceType.WHITE_DRAGON
        )
        self.assertTrue(self.game_manager.board_manager.waiting_on_decision)

    def test_pon_decision(self):
        self.match.current_board.current_turn = 3

        three_discards = Event(EventType.Discard, 3, PieceType.GREEN_DRAGON, False)
        pon_decision = Event(EventType.Pon, 0, PieceType.GREEN_DRAGON, True)

        process_event_queue(
            self.game_manager, self.match, [three_discards, pon_decision]
        )

        self.assertEqual(self.match.current_board.current_turn, 0)
        self.assertTrue(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(
            self.match.current_board.players[3].discard_pile[-1], PieceType.GREEN_DRAGON
        )
        self.assertEqual(
            self.match.current_board.players[0].calls_avaliable, [Call.Pon]
        )

        chose_to_take_pon = Event(EventType.Pon, 0, PieceType.GREEN_DRAGON, False)

        process_event_queue(self.game_manager, self.match, [chose_to_take_pon])

        self.assertFalse(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(self.match.current_board.players[3].discard_pile, [])
        self.assertEqual(self.match.current_board.players[0].calls_avaliable, [])
        self.assertEqual(
            self.match.current_board.players[0].melded_hand[0].tiles,
            [PieceType.GREEN_DRAGON, PieceType.GREEN_DRAGON, PieceType.GREEN_DRAGON],
        )
        self.assertEqual(
            self.match.current_board.players[0].melded_hand[0].call_direction,
            CallDirection.Left,
        )
        self.assertEqual(
            self.match.current_board.players[0].hand,
            [PieceType.TWO_BAMBOO, PieceType.THREE_BAMBOO],
        )

    def test_pon_decision_ai(self):
        self.match.current_board.current_turn = 2

        self.match.players[3].hand += [PieceType.GREEN_DRAGON] * 2

        three_discards = Event(EventType.Discard, 2, PieceType.GREEN_DRAGON, False)
        pon_decision = Event(EventType.Pon, 3, PieceType.GREEN_DRAGON, False)

        process_event_queue(
            self.game_manager, self.match, [three_discards, pon_decision]
        )

        self.assertEqual(self.match.current_board.current_turn, 3)
        self.assertFalse(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(self.match.current_board.players[2].discard_pile, [])
        self.assertEqual(self.match.current_board.players[3].calls_avaliable, [])
        self.assertEqual(
            self.match.current_board.players[3].melded_hand[0].tiles,
            [PieceType.GREEN_DRAGON, PieceType.GREEN_DRAGON, PieceType.GREEN_DRAGON],
        )
        self.assertEqual(
            self.match.current_board.players[3].melded_hand[0].call_direction,
            CallDirection.Left,
        )
        self.assertEqual(len(self.match.current_board.players[3].hand), 13)

    def test_chi_decision(self):
        self.match.current_board.current_turn = 3

        three_discards = Event(EventType.Discard, 3, PieceType.FOUR_BAMBOO, False)
        chi_decision = Event(EventType.Chi, 0, PieceType.FOUR_BAMBOO, True)

        process_event_queue(
            self.game_manager, self.match, [three_discards, chi_decision]
        )

        self.assertEqual(self.match.current_board.current_turn, 0)
        self.assertTrue(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(
            self.match.current_board.players[3].discard_pile[-1], PieceType.FOUR_BAMBOO
        )
        self.assertEqual(
            self.match.current_board.players[0].calls_avaliable, [Call.Chi]
        )

        chose_to_take_chi = Event(EventType.Chi, 0, PieceType.TWO_BAMBOO, False)

        process_event_queue(self.game_manager, self.match, [chose_to_take_chi])

        self.assertFalse(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(self.match.current_board.players[3].discard_pile, [])
        self.assertEqual(self.match.current_board.players[0].calls_avaliable, [])
        self.assertEqual(
            self.match.current_board.players[0].melded_hand[0].tiles,
            [PieceType.TWO_BAMBOO, PieceType.THREE_BAMBOO, PieceType.FOUR_BAMBOO],
        )
        self.assertEqual(
            self.match.current_board.players[0].melded_hand[0].call_direction,
            CallDirection.Left,
        )
        self.assertEqual(
            self.match.current_board.players[0].hand,
            [PieceType.GREEN_DRAGON, PieceType.GREEN_DRAGON],
        )

    def test_chi_decision_ai(self):
        self.match.current_board.current_turn = 0

        self.match.players[1].hand += [PieceType.TWO_BAMBOO, PieceType.THREE_BAMBOO]

        three_discards = Event(EventType.Discard, 0, PieceType.FOUR_BAMBOO, False)
        chi_decision = Event(EventType.Chi, 1, PieceType.TWO_BAMBOO, False)

        process_event_queue(
            self.game_manager, self.match, [three_discards, chi_decision]
        )

        self.assertEqual(self.match.current_board.current_turn, 1)
        self.assertFalse(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(self.match.current_board.players[0].discard_pile, [])
        self.assertEqual(self.match.current_board.players[1].calls_avaliable, [])
        self.assertEqual(
            self.match.current_board.players[1].melded_hand[0].tiles,
            [PieceType.TWO_BAMBOO, PieceType.THREE_BAMBOO, PieceType.FOUR_BAMBOO],
        )
        self.assertEqual(
            self.match.current_board.players[1].melded_hand[0].call_direction,
            CallDirection.Left,
        )
        self.assertEqual(len(self.match.current_board.players[1].hand), 13)

    def test_kan_decision(self):
        self.match.current_board.current_turn = 2

        self.match.players[0].hand += [PieceType.GREEN_DRAGON]

        three_discards = Event(EventType.Discard, 2, PieceType.GREEN_DRAGON, False)
        kan_decision = Event(EventType.Kan, 0, PieceType.GREEN_DRAGON, True)

        process_event_queue(
            self.game_manager, self.match, [three_discards, kan_decision]
        )

        self.assertEqual(self.match.current_board.current_turn, 3)
        self.assertTrue(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(
            self.match.current_board.players[2].discard_pile, [PieceType.GREEN_DRAGON]
        )
        self.assertEqual(
            self.match.current_board.players[0].calls_avaliable, [Call.Kan]
        )

        chose_to_take_kan = Event(EventType.Kan, 0, PieceType.GREEN_DRAGON, False)

        process_event_queue(self.game_manager, self.match, [chose_to_take_kan])

        self.assertFalse(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(self.match.current_board.players[2].discard_pile, [])
        self.assertEqual(self.match.current_board.players[0].calls_avaliable, [])
        self.assertEqual(
            self.match.current_board.players[0].melded_hand[0].tiles,
            [
                PieceType.GREEN_DRAGON,
                PieceType.GREEN_DRAGON,
                PieceType.GREEN_DRAGON,
                PieceType.GREEN_DRAGON,
            ],
        )
        self.assertEqual(
            self.match.current_board.players[0].melded_hand[0].call_direction,
            CallDirection.Forward,
        )
        self.assertEqual(
            self.match.current_board.players[0].hand,
            [PieceType.TWO_BAMBOO, PieceType.THREE_BAMBOO],
        )

    def test_kan_decision_ai(self):
        self.match.current_board.current_turn = 1

        self.match.players[3].hand += [PieceType.GREEN_DRAGON] * 3

        three_discards = Event(EventType.Discard, 1, PieceType.GREEN_DRAGON, False)
        kan_decision = Event(EventType.Kan, 3, PieceType.GREEN_DRAGON, False)

        process_event_queue(
            self.game_manager, self.match, [three_discards, kan_decision]
        )

        self.assertEqual(self.match.current_board.current_turn, 3)
        self.assertFalse(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(self.match.current_board.players[1].discard_pile, [])
        self.assertEqual(self.match.current_board.players[3].calls_avaliable, [])
        self.assertEqual(
            self.match.current_board.players[3].melded_hand[0].tiles,
            [PieceType.GREEN_DRAGON] * 4,
        )
        self.assertEqual(
            self.match.current_board.players[3].melded_hand[0].call_direction,
            CallDirection.Forward,
        )
        self.assertEqual(len(self.match.current_board.players[3].hand), 13)

    def test_riichi(self):
        decision = Event(EventType.Riichi, 0, PieceType.GREEN_DRAGON, True)

        process_event_queue(self.game_manager, self.match, [decision])

        self.assertEqual(self.match.current_board.current_turn, 0)
        self.assertEqual(
            self.match.current_board.players[0].calls_avaliable, [Call.Riichi]
        )
        self.assertTrue(self.game_manager.board_manager.waiting_on_decision)

        riichi_discard = Event(EventType.Riichi, 0, PieceType.GREEN_DRAGON, False)
        process_event_queue(self.game_manager, self.match, [riichi_discard])

        self.assertFalse(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(self.match.current_board.current_turn, 1)
        self.assertEqual(
            self.match.current_board.players[0].discard_pile, [PieceType.GREEN_DRAGON]
        )
        self.assertTrue(self.match.current_board.players[0].riichi_declared)

    def test_riichi_ai(self):
        self.match.current_board.current_turn = 2
        riichi_discard = Event(EventType.Riichi, 2, PieceType.GREEN_DRAGON, False)
        process_event_queue(self.game_manager, self.match, [riichi_discard])

        self.assertFalse(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(self.match.current_board.current_turn, 3)
        self.assertEqual(
            self.match.current_board.players[2].discard_pile, [PieceType.GREEN_DRAGON]
        )
        self.assertTrue(self.match.current_board.players[2].riichi_declared)

    def test_ckan_decision(self):
        self.match.current_board.current_turn = 0

        self.match.players[0].hand += [PieceType.GREEN_DRAGON] * 2

        ckan_decision = Event(EventType.ConcealedKan, 0, PieceType.GREEN_DRAGON, True)

        process_event_queue(self.game_manager, self.match, [ckan_decision])

        self.assertEqual(self.match.current_board.current_turn, 0)
        self.assertTrue(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(
            self.match.current_board.players[0].calls_avaliable, [Call.Concealed_Kan]
        )

        chose_ckan = Event(EventType.ConcealedKan, 0, PieceType.GREEN_DRAGON, False)

        process_event_queue(self.game_manager, self.match, [chose_ckan])

        self.assertFalse(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(self.match.current_board.players[0].calls_avaliable, [])
        self.assertEqual(
            self.match.current_board.players[0].melded_hand[0].tiles,
            [
                PieceType.GREEN_DRAGON,
                PieceType.GREEN_DRAGON,
                PieceType.GREEN_DRAGON,
                PieceType.GREEN_DRAGON,
            ],
        )
        self.assertEqual(
            self.match.current_board.players[0].melded_hand[0].call_direction,
            CallDirection.Concealed,
        )
        self.assertEqual(
            self.match.current_board.players[0].hand,
            [PieceType.TWO_BAMBOO, PieceType.THREE_BAMBOO],
        )
        self.assertEqual(self.match.current_board.current_turn, 0)

    def test_ckan_decision_ai(self):
        self.match.current_board.current_turn = 2

        self.match.players[2].hand += [PieceType.GREEN_DRAGON] * 4

        ckan_decision = Event(EventType.ConcealedKan, 2, PieceType.GREEN_DRAGON, False)

        process_event_queue(self.game_manager, self.match, [ckan_decision])

        self.assertFalse(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(self.match.current_board.players[2].calls_avaliable, [])
        self.assertEqual(
            self.match.current_board.players[2].melded_hand[0].tiles,
            [
                PieceType.GREEN_DRAGON,
                PieceType.GREEN_DRAGON,
                PieceType.GREEN_DRAGON,
                PieceType.GREEN_DRAGON,
            ],
        )
        self.assertEqual(
            self.match.current_board.players[2].melded_hand[0].call_direction,
            CallDirection.Concealed,
        )
        self.assertEqual(len(self.match.current_board.players[2].hand), 13)
        self.assertEqual(self.match.current_board.current_turn, 2)

    def test_ron_decision(self):
        self.match.current_board.current_turn = 2

        three_discards = Event(EventType.Discard, 2, PieceType.GREEN_DRAGON, False)
        ron_decisions = Event(EventType.Ron, 0, PieceType.GREEN_DRAGON, True)

        process_event_queue(
            self.game_manager, self.match, [three_discards, ron_decisions]
        )

        self.assertEqual(self.match.current_board.current_turn, 2)
        self.assertFalse(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(
            self.match.current_board.players[2].discard_pile[-1], PieceType.GREEN_DRAGON
        )
        self.assertEqual(
            self.match.current_board.players[0].calls_avaliable, [Call.Ron]
        )

        ron = Event(EventType.Ron, 0, PieceType.GREEN_DRAGON, False)

        process_event_queue(self.game_manager, self.match, [ron])

        self.assertFalse(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(self.match.current_board.players[0].calls_avaliable, [])
        self.assertEqual(
            self.match.current_board.players[0].hand,
            [PieceType.GREEN_DRAGON] * 2
            + [PieceType.TWO_BAMBOO, PieceType.THREE_BAMBOO]
            + [PieceType.GREEN_DRAGON],
        )
        self.assertTrue(self.game_manager.board_manager.round_should_end)

    def test_ron_decision_ai(self):
        three_discards = Event(EventType.Discard, 2, PieceType.GREEN_DRAGON, False)
        ron_decisions = Event(EventType.Ron, 1, PieceType.GREEN_DRAGON, False)

        process_event_queue(
            self.game_manager, self.match, [three_discards, ron_decisions]
        )

        self.assertFalse(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(self.match.current_board.players[1].calls_avaliable, [])
        self.assertEqual(
            self.match.current_board.players[1].hand,
            [PieceType.ERROR] * 13 + [PieceType.GREEN_DRAGON],
        )
        self.assertTrue(self.game_manager.board_manager.round_should_end)

    def test_tsumo(self):
        self.match.current_board.current_turn = 0

        tsumo = Event(EventType.Tsumo, 0, PieceType.GREEN_DRAGON, True)

        process_event_queue(self.game_manager, self.match, [tsumo])

        self.assertEqual(self.match.current_board.current_turn, 0)
        self.assertEqual(
            self.match.current_board.players[0].calls_avaliable, [Call.Tsumo]
        )
        self.assertTrue(self.game_manager.board_manager.waiting_on_decision)

        do_tsumo = Event(EventType.Tsumo, 0, PieceType.GREEN_DRAGON, False)

        process_event_queue(self.game_manager, self.match, [do_tsumo])

        self.assertFalse(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(self.match.current_board.players[0].calls_avaliable, [])
        self.assertTrue(self.game_manager.board_manager.round_should_end)

    def test_tsumo_ai(self):
        self.match.current_board.current_turn = 2

        tsumo = Event(EventType.Tsumo, 2, PieceType.GREEN_DRAGON, False)

        process_event_queue(self.game_manager, self.match, [tsumo])

        self.assertFalse(self.game_manager.board_manager.waiting_on_decision)
        self.assertEqual(self.match.current_board.players[2].calls_avaliable, [])
        self.assertTrue(self.game_manager.board_manager.round_should_end)

    def test_point_diff(self):
        player1 = Event(EventType.PointDiff, 0, 0, False)
        player2 = Event(EventType.PointDiff, 1, -100, False)
        player3 = Event(EventType.PointDiff, 2, 200, False)
        player4 = Event(EventType.PointDiff, 3, 300, False)

        process_event_queue(
            self.game_manager, self.match, [player1, player3, player2, player4]
        )

        self.assertEqual(self.match.scores[0], 25000)
        self.assertEqual(self.match.scores[1], 24900)
        self.assertEqual(self.match.scores[2], 25200)
        self.assertEqual(self.match.scores[3], 25300)
