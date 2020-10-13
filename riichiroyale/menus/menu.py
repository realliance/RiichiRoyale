from riichiroyale import View

class Menu(View):
  def __init__(self, name, gui_manager, elements=None):
    super().__init__(name)
    self.manager = gui_manager
    if elements is None:
      self.uiElements = []
    else:
      self.uiElements = elements

    self.process_ui_event = None

  def update(self, time_delta):
    return self.manager.update(time_delta)

  def draw(self, screen):
    self.manager.draw_ui(screen)