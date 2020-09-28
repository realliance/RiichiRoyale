CXX=clang++
CXXFLAGS=-I.
GTKMMFLAGS=`pkg-config gtkmm-3.0 --cflags --libs`

mahjong: mainwindow.cpp main.cpp ../../src/walls.cpp ../../include/pieces.h ../../include/walls.h mainwindow.h;
	$(CXX) -I../../include -o mahjongShuffle $(GTKMMFLAGS) mainwindow.cpp main.cpp ../../src/walls.cpp
