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
def setAllSoundEffectVolume(sounds, volume):
	for soundEffect in sounds:
		soundEffect[0].set_volume(volume)
	return volume
	
#  volume is an value from 0 to 1
#  sounds is the array returned from soundInit()	
#  returns new volume
def setAllVolume(sounds, volume):
	for soundEffect in sounds:
		soundEffect[0].set_volume(volume)
	pygame.mixer.music.set_volume(volume)
	return volume


#Folk Chinese by PeriTune | http://peritune.com
#Music promoted by https://www.free-stock-music.com
#Creative Commons Attribution 3.0 Unported License
#https://creativecommons.org/licenses/by/3.0/deed.en_US
def musicInit(volume):
	gameMusic = pygame.mixer.music.load("peritune-folk-chinese.ogg")
	pygame.mixer.music.set_volume(volume)

	
def musicStop():
	pygame.mixer.music.stop()
	pygame.mixer.music.rewind()

def musicStart():
	pygame.mixer.music.play()



def setMusicVolume(music, volume):
	pygame.mixer.music.set_volume(volume)
	return volume

