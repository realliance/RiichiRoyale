class View:
    def __init__(self, name):
        self.name = name

    def on_pygame_event(self, event):
        pass

    def update(self, time_delta):
        pass

    def draw(self, screen):
        pass
