CXX=clang++
CXXFLAGS=-I.

build: libmahjong.so
	mv libmahjong.so ..

clean: 
	rm -f *.o ../libmahjong.so

libmahjong.so: walls.o python.o
	$(CXX) -g -Wall -Wextra -Wmost -shared -Wl,-soname,libmahjong.so -o libmahjong.so  walls.o python.o

walls.o: walls.cpp ../include/pieces.h ../include/walls.h
	$(CXX) -g -Wall -Wextra -Wmost -I../include -c -fPIC walls.cpp -o walls.o

python.o: python.cpp ../include/pieces.h ../include/walls.h
	$(CXX) -g -Wall -Wextra -Wmost -I../include -c -fPIC python.cpp -o python.o
