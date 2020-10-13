#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  soundWrapper.py
#_______________________________________________________________________

import pygame

def soundInit():
	tileClacking = pygame.mixer.Sound("./audio/dominoPlace.wav")
	dominoDump = pygame.mixer.Sound("./audio/dominoDump.wav")
	dominoSelect = pygame.mixer.Sound("./audio/dominoSelect.wav")
	sounds = [[tileClacking,"tileSound"],[dominoDump,"shuffleSound"],[dominoSelect,"selected"]]
	global masterVolume
	masterVolume = 1
	global sfxVolume
	sfxVolume = 1
	global musicVolume
	musicVolume = 1
	return sounds
	
	
#  volume is an value from 0 to 1
#  sounds is the array returned from soundInit()	
#  returns new volume
def setAllSoundEffectVolume(sounds, volume):
	sfxVolume = volume
	for soundEffect in sounds:
		soundEffect[0].set_volume(sfxVolume*masterVolume)
	return sfxVolume
	
	
#  volume is an value from 0 to 1
#  sounds is the array returned from soundInit()	
#  returns new volume
def setMasterVolume(sounds, volume):
	masterVolume = volume
	for soundEffect in sounds:
		soundEffect[0].set_volume(sfxVolume*masterVolume)
	pygame.mixer.music.set_volume(musicVolume*masterVolume)
	return masterVolume


#Folk Chinese by PeriTune | http://peritune.com
#Music promoted by https://www.free-stock-music.com
#Creative Commons Attribution 3.0 Unported License
#https://creativecommons.org/licenses/by/3.0/deed.en_US
def musicInit(volume):
	gameMusic = pygame.mixer.music.load("./audio/peritune-folk-chinese.ogg")
	musicVolume = volume
	pygame.mixer.music.set_volume(musicVolume*masterVolume)

	
def musicStop():
	pygame.mixer.music.stop()
	pygame.mixer.music.rewind()


def musicStart():
	pygame.mixer.music.play(-1)


def setMusicVolume(volume):
	musicVolume = volume
	pygame.mixer.music.set_volume(musicVolume*masterVolume)
	return musicVolume
