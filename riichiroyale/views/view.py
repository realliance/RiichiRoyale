
class View():
  def __init__(self, name):
    self.name = name
    self.manager = None

  def process_ui_event(self, _event):
    return False

  def update(self, _time_delta):
    return False

  def draw(self, _screen):
    return False