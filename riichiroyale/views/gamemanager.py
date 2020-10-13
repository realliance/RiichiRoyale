class GameManager:
  def __init__(self, sound_manager):
    self.views = dict()
    self.current_view = None
    self.sound_manager = sound_manager

  def add_view(self, view):
    self.views[view.name] = view

  def process_ui_event(self, event):
    self.views[self.current_view].process_ui_event(event)

  def update_gui_manager(self, event):
    if self.views[self.current_view].manager is not None:
      self.views[self.current_view].manager.process_events(event)

  def set_active_view(self, name):
    self.current_view = name

  def get_active_view(self):
    return self.views[self.current_view]

  def update(self, time_delta):
    self.views[self.current_view].update(time_delta)

  def draw(self, screen):
    self.views[self.current_view].draw(screen)