#include <algorithm>
#include <iostream>
#include "walls.h"

struct fat_string{
  char* data;
  size_t size;
};

fat_string to_fat_string(std::vector<Piece>&& vect){
  Piece* fat = new Piece[vect.size()];
  std::copy_n(vect.data(), vect.size(), fat);
  return {reinterpret_cast<char*>(fat), vect.size()};
}

fat_string to_fat_string(std::string&& vect){
  char* fat = new char[vect.size()];
  std::copy_n(vect.data(), vect.size(), fat);
  return {fat, vect.size()};
}

extern "C" {
  Walls* Walls_new(){ return new Walls(); }
  void Walls_delete(Walls* walls){ delete walls; }
  fat_string Walls_TakeHand(Walls* walls){return to_fat_string(walls->TakeHand());}
  fat_string Walls_GetPath(Piece piece){ return to_fat_string(Walls::GetPath(piece));}
  void Walls_SetPath(const char* filePath){ Walls::SetPath(filePath);}
  void fat_string_delete(fat_string fat){
    delete[] fat.data;
  }

}
