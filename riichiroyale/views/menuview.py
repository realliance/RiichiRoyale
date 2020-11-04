import pygame
from .view import View


class MenuView(View):
    def __init__(self, name, ui_manager):
        super().__init__(name)
        self.manager = ui_manager

    def process_ui_event(self, event):
        pass

    def on_pygame_event(self, event):
        if event.type == pygame.USEREVENT:
            self.process_ui_event(event)
        self.manager.process_events(event)

    def update(self, time_delta):
        self.manager.update(time_delta)

    def draw(self, screen):
        self.manager.draw_ui(screen)
