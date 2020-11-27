


class bolickBot(MahjongAI, Player):
  def __init__(self):
    MahjongAI.init(self)
    Player.init(self, self.Name())

