import logging


class DialogManager:
    def __init__(self):
        self.events = dict()
        self.current_page = 0
        self.current_event = None
        self.logger = logging.getLogger(__name__)
        self.logger.setLevel(logging.DEBUG)

    def register_dialog_event(self, name):
        self.events[name] = []

    def append_dialog_event(self, name, text):
        self.events[name] += text

    def start_event(self, name):
        if name in self.events:
            self.logger.debug("Started event %s", name)
            self.current_event = name
            self.current_page = 0
            return True
        return False

    def get_current_page(self):
        return self.events[self.current_event][self.current_page]

    def is_last_page(self):
        return len(self.events[self.current_event]) == self.current_page + 1

    def next_page(self):
        self.current_page += 1
