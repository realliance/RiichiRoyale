class GameManager:
    def __init__(self, board_manager, sound_manager):
        self.views = dict()
        self.board_manager = board_manager
        self.current_view = None
        self.sound_manager = sound_manager

    def add_view(self, view):
        self.views[view.name] = view

    def on_pygame_event(self, event):
        self.views[self.current_view].on_pygame_event(event)

    def set_active_view(self, name):
        self.current_view = name

    def get_active_view(self):
        return self.views[self.current_view]

    def update(self, time_delta):
        self.views[self.current_view].update(time_delta)

    def draw(self, screen):
        self.views[self.current_view].draw(screen)
