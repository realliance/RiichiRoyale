#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  testing.py
#_______________________________________________________________________

def main(args):
	return 0

if __name__ == '__main__':
	import sys
	import pygame
	import soundWrapper
	RED = (255, 0, 0)
	pygame.init()
	size = (500, 500)
	screen = pygame.display.set_mode(size)
	pygame.display.set_caption("My Audio Test")
	done = False
	clock = pygame.time.Clock()
	sounds = soundWrapper.soundInit()
	while not done:
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				done = True
			if event.type == pygame.KEYDOWN:# any keyboard input
				sounds[0][0].play()
		screen.fill(RED)
		pygame.display.flip()
		clock.tick(60)
	pygame.quit()
	sys.exit(main(sys.argv))


