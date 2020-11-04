import os
import pygame
from pygame.transform import smoothscale


def load_image(name, surface, size=None, position=(0, 0)):
    try:
        image = pygame.image.load(name)
    except pygame.error() as message:
        print("Cannot load image:", name)
        raise SystemExit from message
    if size is not None:
        image = smoothscale(image, size)
    surface.blit(image, position)

def load_image_resource(name, surface, size=None, position=(0, 0)):
    current_path = os.path.dirname(os.path.realpath(__file__))
    load_image(os.path.join(current_path, "..", "resources", "images", name), surface, size, position)