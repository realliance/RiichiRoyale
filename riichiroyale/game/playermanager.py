from threading import Lock
from libmahjong import MahjongAI, EventType, EngineEvent, PieceType
from .player import Player
from .gameevent import Event

class PlayerManager(MahjongAI, Player):
  def __init__(self):
    MahjongAI.__init__(self)
    Player.__init__(self, self.Name())
    self.received_events = []
    self.decision_complete = False
    self.decision_made = None
    self.current_match = None
    self.lock = Lock()

  def GameStart(self, player_id):
    print('==GAME STARTED==')
    with self.lock:
      self.player_id = player_id

  def RoundStart(self, hand, seatWind, prevalentWind):
    print('====ROUND START CALLED====')
    print('SEATWIND:', seatWind)
    print(list(map(lambda x: PieceType(x.get_raw_value()), hand)))
    with self.lock:
      if len(self.hand) == 0:
        self.hand = hand
        self.hand.sort()
        self.seat_wind = seatWind
        self.prevalent_wind = prevalentWind
      else:
        self.next_round_hand = hand
        self.next_round_seat = seatWind
        self.next_round_prevalent_wind = prevalentWind

  def next_round(self):
    with self.lock:
      self.hand = self.next_round_hand
      self.hand.sort()
      self.seat_wind = self.next_round_seat
      self.prevalent_wind = self.next_round_prevalent_wind

  def ReceiveEvent(self, event):
    should_notify = True
    with self.lock:
      self.received_events += [event]
      if event.type == EventType.Discard:
        if not event.decision:
          should_notify = False
    if should_notify:
      with self.current_match.process_lock:
        self.current_match.process_lock.notify()

  def RetrieveDecision(self):
    print('[PlayerManager] Decision Retrieve Started')
    while not self.decision_complete:
      continue
    with self.lock:
      self.decision_complete = False
      print('[PlayerManager] Decision Retrieved')
      return self.decision_made

  def MakeDecision(self, event):
    if isinstance(event, Event):
      print('INFO: Casted Event to Engine Event')
      engine_event = EngineEvent()
      engine_event.type = event.type
      engine_event.player = event.player
      print(event.piece.get_raw_value())
      engine_event.piece = event.piece.get_raw_value()
      engine_event.decision = event.decision
      event = engine_event

    with self.lock:
      self.calls_avaliable = []
      self.decision_made = event
      self.decision_complete = True

  def Pop(self):
    with self.lock:
      if len(self.received_events) == 0:
        return None
      return self.received_events.pop(0)

  def AddToEventQueue(self, event):
    with self.lock:
      self.received_events = [event] + self.received_events

  def GetQueueLength(self):
    with self.lock:
      return len(self.received_events)

  @staticmethod
  def Name():
    return "Player"