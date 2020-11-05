from threading import Lock
from libmahjong import MahjongAI, EventType, EngineEvent
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
    with self.lock:
      self.hand = hand
      self.seat_wind = seatWind
      self.prevalent_wind = prevalentWind

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
    while not self.decision_complete:
      pass
    with self.lock:
      self.decision_complete = False
      return self.decision_made

  def MakeDecision(self, event):
    if isinstance(event, Event):
      print('INFO: Casted Event to Engine Event')
      engine_event = EngineEvent()
      engine_event.type = event.type
      engine_event.player = event.player
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