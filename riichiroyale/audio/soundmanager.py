import pygame
import random
from pygame.mixer import Sound

class SoundManager():
	def __init__(self):
		self.audio_sources = dict()
		self.playlist_running = False
		self.music_playlist = []
		self.sfx_sets = dict()
		self.current_playlist_song = 0
		self.master_volume = 1
		self.music_volume = 1
		self.sfx_volume = 1

	def register_sfx_set(self, name):
		self.sfx_sets[name] = []

	def add_to_sfx_set(self, name, sfx):
		self.sfx_sets[name] += [sfx]

	def play_from_set(self, set_name):
		self.play_sfx(random.choice(self.sfx_sets[set_name]))

	def add_audio_source(self, name, path, preload=True):
		if preload:
			sound_obj = Sound(path)
		else:
			sound_obj = None
		self.audio_sources[name] = [preload, path, sound_obj]

	def play_sfx(self, name, preload_if_not=True):
		if name not in self.audio_sources:
			raise "Attempted to play unregistered audio source"
		preloaded, path, sound_obj = self.audio_sources[name]
		if not preloaded and preload_if_not:
			sound_obj = Sound(path)
			self.audio_sources[name] = [True, path, sound_obj]
		sound_obj.set_volume(self.master_volume * self.sfx_volume)
		sound_obj.play()

	def on_update(self):
		if not pygame.mixer.music.get_busy():
			self.current_playlist_song += 1
			if self.current_playlist_song >= len(self.current_playlist_song):
				self.current_playlist_song = 0
			self.play_music(self.music_playlist[self.current_playlist_song])

	def start_playlist(self):
		self.playlist_running = True
		path = self.audio_sources[self.music_playlist[self.current_playlist_song]][1]
		pygame.mixer.music.load(path)
		pygame.mixer.music.set_volume(self.music_volume * self.master_volume)
		pygame.mixer.music.play()

	def play_music(self, name):
		if name not in self.audio_sources:
			raise "Attempted to play unregistered audio source"
		path = self.audio_sources[name][1]
		pygame.mixer.music.load(path)
		pygame.mixer.music.set_volume(self.music_volume * self.master_volume)
		pygame.mixer.music.play()

	def stop_music(self):
		pygame.mixer.music.stop()
		pygame.mixer.music.rewind()

	def set_music_volume(self, volume):
		self.music_volume = volume
		pygame.mixer.music.set_volume(self.music_volume * self.master_volume)

	def set_master_volume(self, volume):
		self.master_volume = volume
		pygame.mixer.music.set_volume(self.music_volume * self.master_volume)

	def set_sfx_volume(self, volume):
		self.sfx_volume = volume
	



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
