import json
import os


class SettingsManager:
    def __init__(self):
        self.settings_data = {
            "Master Volume": 100,
            "Music Volume": 100,
            "SFX Volume": 100,
        }
        self.settings_file = "settings.json"
        if not os.path.exists(self.settings_file):
            #print("Creating settings file:", os.path.abspath(self.settings_file))
            file = open(self.settings_file, "w")
            json.dump(self.settings_data, file)
            file.close()
        #print("Loading settings file:", os.path.abspath(self.settings_file))
        file = open(self.settings_file, "r")
        self.settings_data = json.load(file)

    def get_setting(self, setting):
        return self.settings_data[setting]

    def set_setting(self, setting, value):
        self.settings_data[setting] = value

    def save_to_file(self):
        #print("Saving settings to file: ", os.path.abspath(self.settings_file))
        file = open(self.settings_file, "w")
        json.dump(self.settings_data, file)
        file.close()
