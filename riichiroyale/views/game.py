import math
import libmahjong
import pygame
from pygame import Rect
from pygame import surface
from riichiroyale import Board, Player, BoardRender
from .view import View

class GameView(View):
  def __init__(self, screen, tile_dict, small_tile_dict, screen_width, screen_height, width_ratio, height_ratio):
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

    # Play Area Surface
    self.player_area_rect = self.get_play_area_pos(screen)
    self.play_area = surface.Surface((self.player_area_rect.width, self.player_area_rect.height), flags=pygame.SRCALPHA)

    # Example game Board
    main_player = Player("Player")
    bot_one = Player("Bot 1")
    bot_two = Player("Bot 2")
    bot_three = Player("Bot 3")
    board = Board()

    board.register_player(main_player)
    board.register_player(bot_one)
    board.register_player(bot_two)
    board.register_player(bot_three)

    #board.shuffle_dealer()
    main_player.hand = board.draw_tile(num=13)
    main_player.hand.sort()
    bot_one.hand = board.draw_tile(num=13)
    bot_two.hand = board.draw_tile(num=13)
    bot_three.hand = board.draw_tile(num=13)
    board.on_turn()

    self.board_render = BoardRender(small_tile_dict, tile_dict, self.play_area, board, 0)

  def process_ui_event(self, _event):
    return False

  def update(self, _time_delta):
    self.board_render.update()

  def draw(self, screen):
    self.board_render.draw(self.background)
    screen.blit(self.background, (0, 0))
    screen.blit(self.play_area, (self.player_area_rect.x, self.player_area_rect.y))

  def get_play_area_pos(self, screen):
    w, h = screen.get_size()
    play_area_height = h
    player_area_width = math.floor(h * (float(self.screen_width_ratio)/self.screen_height_ratio))
    return Rect((w - player_area_width)//2, 0, player_area_width, play_area_height)