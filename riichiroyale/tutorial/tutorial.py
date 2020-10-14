class Tutorial:
    def __init__(self, tiles_to_discard, calls):
        self.tiles_to_discard = tiles_to_discard
        self.calls = calls
        self.next_discard = self.tiles_to_discard.pop(0)
        self.next_call = self.calls.pop(0)

    def discard(self):
        self.next_discard = self.tiles_to_discard.pop(0)

    def call(self):
        self.next_call = self.calls.pop(0)