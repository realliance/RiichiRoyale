


class YourBot(MahjongAI, Player):
  def __init__(self):
    MahjongAI.init(self)
    Player.init(self, self.Name())


def main(args):
    return 0

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
