import random
import pygame
from pygame.mixer import Sound
from riichiroyale.utils import settingsmanager


class SoundManager:
    def __init__(self):
        self.audio_sources = dict()
        self.playlist_running = False
        self.music_playlist = []
        self.sfx_sets = dict()
        self.current_playlist_song = 0
        self.master_volume = 1  # acceptable values 0 to 1
        self.music_volume = 1  # percernatage of master volume(acceptable values 0 to 1)
        self.sfx_volume = 1  # percernatage of master volume(acceptable values 0 to 1)

        settings = settingsmanager.SettingsManager()  # Load settings from file (or create new one)
        self.set_master_volume(
            settings.get_setting("Master Volume") / 100.0
        )
        self.set_music_volume(
            settings.get_setting("Music Volume") / 100.0
        )
        self.set_sfx_volume(
            settings.get_setting("SFX Volume") / 100.0
        )


    # define new sound set
    def register_sfx_set(self, name):
        self.sfx_sets[name] = []

    # add to sound set
    def add_to_sfx_set(self, name, sfx):
        self.sfx_sets[name] += [sfx]

    # play from set
    def play_from_set(self, set_name):
        self.play_sfx(random.choice(self.sfx_sets[set_name]))

    def add_audio_source(self, name, path, preload=True):
        if preload:  # default is to preload
            sound_obj = Sound(path)
        else:
            sound_obj = None
        self.audio_sources[name] = [preload, path, sound_obj]

    def play_sfx(self, name, preload_if_not=True):
        if name not in self.audio_sources:
            raise "Attempted to play unregistered audio source"
        preloaded, path, sound_obj = self.audio_sources[name]
        if not preloaded and preload_if_not:  # preload logic
            sound_obj = Sound(path)
            self.audio_sources[name] = [True, path, sound_obj]
        sound_obj.set_volume(
            self.master_volume * self.sfx_volume
        )  # set volume before playing
        sound_obj.play()

    def on_update(self):
        if not pygame.mixer.music.get_busy():  # plays next song when previous song ends
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
        if name not in self.audio_sources:  # exception handling
            raise "Attempted to play unregistered audio source"
        path = self.audio_sources[name][1]
        pygame.mixer.music.load(path)
        pygame.mixer.music.set_volume(self.music_volume * self.master_volume)
        pygame.mixer.music.play(-1)

    @staticmethod
    def stop_music():
        pygame.mixer.music.stop()
        pygame.mixer.music.rewind()  # restarts song so that if play is called it is at the beginning

    def set_music_volume(self, volume):
        self.music_volume = volume
        pygame.mixer.music.set_volume(self.music_volume * self.master_volume)

    def set_master_volume(self, volume):
        self.master_volume = volume
        pygame.mixer.music.set_volume(self.music_volume * self.master_volume)

    def set_sfx_volume(self, volume):
        self.sfx_volume = volume
