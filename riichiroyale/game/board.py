from libmahjong import Piece, PieceType


class Board:
    def __init__(
        self,
        sound_manager=None,
        wall=None,
        deadwall=None,
        dora_revealed=1,
        current_turn=0,
    ):
        self.sound_manager = sound_manager
        if wall is not None and deadwall is not None:
            self.wall = wall
            self.deadwall = deadwall
        else:
            self.wall = [Piece(PieceType.ERROR)] * 70
            self.deadwall = [Piece(PieceType.ERROR)] * 14
        self.dora_revealed = dora_revealed
        self.current_turn = current_turn
        self.current_dealer = 0

    def out_of_tiles(self):
        return len(self.wall) == 0

    def get_revealed_dora(self):
        dora = []
        for index in range(0, self.dora_revealed + 1, 2):
            dora.append(self.deadwall[index])
        return dora
