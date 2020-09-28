from .c_mahjong import *
from .c_pieces import *
libmahjong.Walls_new.argtypes = []
libmahjong.Walls_new.restype = ctypes.c_void_p

libmahjong.Walls_delete.argtypes = [ctypes.c_void_p]
libmahjong.Walls_delete.restype = None

libmahjong.Walls_TakeHand.argtypes = [ctypes.c_void_p]
libmahjong.Walls_TakeHand.restype = fat_string

libmahjong.Walls_GetPath.argtypes = [ctypes.c_uint8]
libmahjong.Walls_GetPath.restype = fat_string

libmahjong.Walls_SetPath.argtypes = [ctypes.c_char_p]
libmahjong.Walls_SetPath.restype = None

libmahjong.fat_string_delete.argtypes = [fat_string]
libmahjong.fat_string_delete.restype = None
