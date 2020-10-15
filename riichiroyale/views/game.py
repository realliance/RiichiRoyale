import math
import libmahjong
import random
import pygame
from pygame import Rect
from pygame import surface
from riichiroyale import Board, Player, BoardRender, Match
from .view import View

class GameView(View):
  def __init__(self, sound_manager, screen, tile_dict, small_tile_dict, screen_width, screen_height, width_ratio, height_ratio):
    super().__init__("game")
    self.screen = screen
    self.screen_width = screen_width
    self.screen_height = screen_height
    self.screen_width_ratio = width_ratio
    self.screen_height_ratio = height_ratio

    # Fill background
    background = surface.Surface(screen.get_size())
    self.background = background.convert_alpha()
    self.background.fill((7, 99, 36))

    self.sound_manager = sound_manager

    # Play Area Surface
    self.player_area_rect = self.get_play_area_pos(screen)
    self.play_area = surface.Surface((self.player_area_rect.width, self.player_area_rect.height), flags=pygame.SRCALPHA)

    self.match = Match(sound_manager)
    self.match.register_player(Player("Player"))
    self.match.register_player(Player("Bot 1"))
    self.match.register_player(Player("Bot 2"))
    self.match.register_player(Player("Bot 3"))

    self.tile_dict = tile_dict
    self.small_tile_dict = small_tile_dict
    self.board_render = None

  def get_play_area_pos(self, screen):
    w, h = screen.get_size()
    play_area_height = h
    player_area_width = math.floor(h * (float(self.screen_width_ratio)/self.screen_height_ratio))
    return Rect((w - player_area_width)//2, 0, player_area_width, play_area_height)

  def on_match_start(self):
    random.shuffle(self.sound_manager.music_playlist)
    self.sound_manager.start_playlist()
    self.match.new_board()
    self.board_render = BoardRender(self.small_tile_dict, self.tile_dict, self.play_area, self.match.current_board, 0)
    self.match.current_board.on_turn()

  def update(self, _):
    self.board_render.update()

  def draw(self, screen):
    self.board_render.draw(self.background)
    screen.blit(self.background, (0, 0))
    screen.blit(self.play_area, (self.player_area_rect.x, self.player_area_rect.y))