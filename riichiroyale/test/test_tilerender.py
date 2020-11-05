import unittest
from libmahjong import PieceType
from riichiroyale.sprites import get_tile_front_name, get_tile_front_path


class TestTileRender(unittest.TestCase):
    def test_get_tile_front_name(self):
        self.assertEqual(get_tile_front_name(PieceType.ONE_BAMBOO), "Sou1")
        self.assertEqual(get_tile_front_name(PieceType.RED_FIVE_BAMBOO), "Sou5-Dora")
        self.assertEqual(get_tile_front_name(PieceType.EAST_WIND), "Ton")

    def test_get_tile_front_path(self):
        self.assertEqual(
            get_tile_front_path("/test/dir/path/", PieceType.ONE_BAMBOO),
            "/test/dir/path/Sou1.svg",
        )
