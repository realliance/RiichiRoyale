import random
import copy
from libmahjong import PieceType

PIECE_SET = (
    [
        PieceType.ONE_BAMBOO,
        PieceType.TWO_BAMBOO,
        PieceType.THREE_BAMBOO,
        PieceType.FOUR_BAMBOO,
        PieceType.SIX_BAMBOO,
        PieceType.SEVEN_BAMBOO,
        PieceType.EIGHT_BAMBOO,
        PieceType.NINE_BAMBOO,
        PieceType.ONE_PIN,
        PieceType.TWO_PIN,
        PieceType.THREE_PIN,
        PieceType.FOUR_PIN,
        PieceType.SIX_PIN,
        PieceType.SEVEN_PIN,
        PieceType.EIGHT_PIN,
        PieceType.NINE_PIN,
        PieceType.ONE_CHARACTER,
        PieceType.TWO_CHARACTER,
        PieceType.THREE_CHARACTER,
        PieceType.FOUR_CHARACTER,
        PieceType.SIX_CHARACTER,
        PieceType.SEVEN_CHARACTER,
        PieceType.EIGHT_CHARACTER,
        PieceType.NINE_CHARACTER,
        PieceType.NORTH_WIND,
        PieceType.EAST_WIND,
        PieceType.SOUTH_WIND,
        PieceType.WEST_WIND,
        PieceType.RED_DRAGON,
        PieceType.GREEN_DRAGON,
        PieceType.WHITE_DRAGON,
    ]
    * 4
    + [PieceType.FIVE_BAMBOO, PieceType.FIVE_CHARACTER, PieceType.FIVE_PIN] * 3
    + [PieceType.RED_FIVE_BAMBOO, PieceType.RED_FIVE_CHARACTER, PieceType.RED_FIVE_PIN]
)


def generate_start_board():
    board = copy.deepcopy(PIECE_SET)
    random.shuffle(board)
    return board[14:], board[:14]
