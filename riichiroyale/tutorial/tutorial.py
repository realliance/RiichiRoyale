class Tutorial:
    def __init__(self, tiles_to_discard, calls, winning_tile, wall, deadwall):
        self.tiles_to_discard = tiles_to_discard
        self.calls = calls
        self.winning_tile = winning_tile
        self.next_discard = self.tiles_to_discard.pop(0)
        self.next_call = self.calls.pop(0)
        self.wall = wall
        self.deadwall = deadwall

    def discard(self):
        self.next_discard = self.tiles_to_discard.pop(0)

    def call(self):
        self.next_call = self.calls.pop(0)
