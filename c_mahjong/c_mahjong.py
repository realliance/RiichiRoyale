import ctypes
from .c_pieces import c_Piece
libmahjong = ctypes.cdll.LoadLibrary('./libmahjong.so')

class fat_string(ctypes.Structure):
  _fields_ = [("data", ctypes.c_char_p),
              ("size", ctypes.c_int)]
  def __repr__(self):
        return '(data: {0}, size: {1})'.format(self.data, self.size)

class Walls:
  def __init__(self):
    self.inst = libmahjong.Walls_new()

  def __del__(self):
    libmahjong.Walls_delete(self.inst)

  def TakeHand(self):
    c_pieces = libmahjong.Walls_TakeHand(self.inst)
    pieces = [c_Piece(c_pieces.data[i]) for i in range(c_pieces.size)]
    libmahjong.fat_string_delete(c_pieces)
    return pieces

  @staticmethod
  def GetPath(piece):
    c_path = libmahjong.Walls_GetPath(piece.value)
    path = [chr(c_path.data[i]) for i in range(c_path.size)]
    libmahjong.fat_string_delete(c_path)
    return "".join(path)

  @staticmethod
  def SetPath(path):
    libmahjong.Walls_SetPath(path.encode('utf-8'))
