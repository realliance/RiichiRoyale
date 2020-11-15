import os
import sys
import toml

def get_object(object_name):
    _ensure_base_storage_dir_created()
    with open(_name_to_path(object_name), 'r') as file:
        return toml.loads(file.read())

def save_object(object_name, obj):
    _ensure_base_storage_dir_created()
    with open(_name_to_path(object_name), 'w') as file:
        file.write(toml.dumps(obj))

def does_object_exist(object_name):
    return os.path.exists(_name_to_path(object_name))

def bootstrap_base_storage():
    _ensure_base_storage_dir_created()
    if not does_object_exist('matches'):
        with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', 'storyfiles', 'matches.toml'), 'r') as file:
            save_object('matches', toml.loads(file.read()))
    perform_verion_checking()

def perform_verion_checking():
    current_matches = dict()
    with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', 'storyfiles', 'matches.toml'), 'r') as file:
        current_matches = toml.loads(file.read())
    stored_matches = get_object('matches')
    if current_matches['version'] > stored_matches['version']:
        save_object('matches', current_matches)


def _ensure_base_storage_dir_created():
    os.makedirs(_get_base_storage_dir(), exist_ok=True)

def _name_to_path(name):
    return os.path.join(_get_base_storage_dir(), *name.split('.')) + '.toml'

def _get_base_storage_dir():
    if sys.platform.startswith('linux'):
        return os.path.expanduser(os.path.join('~', '.local', 'share', 'riichiroyale'))
    if sys.platform.startswith('win32'):
        return os.path.join(os.getenv('APPDATA'), 'riichiroyale')
    raise "On unknown platform! Cannot get base storage directory."
