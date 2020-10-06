#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  soundWrapper.py
#_______________________________________________________________________



import pygame

def soundInit():
	tileClacking = pygame.mixer.Sound("dominoPlace.wav")
	dominoDump = pygame.mixer.Sound("dominoDump.wav")
	dominoSelect = pygame.mixer.Sound("dominoSelect.wav")
	sounds = [[tileClacking,"tileSound"],[dominoDump,"shuffleSound"],[dominoSelect,"selected"]]
	return sounds
	
	
#  volume is an value from 0 to 1
#  sounds is the array returned from soundInit()	
#  returns new volume
def setAllVolume(volume, sounds):
	for soundEffect in sounds:
		soundEffect[0].set_volume(volume)
	return volume
