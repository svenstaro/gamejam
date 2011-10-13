default:
	g++ -Wall -O2 -lBox2D -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -o AsteroidJam.bin src/*.cpp

debug:
	g++ -Wall -g -lBox2D -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -o AsteroidJam.bin src/*.cpp

win:
	i486-mingw32-g++ -Wall -o AsteroidJam.exe src/*.cpp /usr/i486-mingw32/lib/libsfml-{audio,graphics,system,window}.a /usr/i486-mingw32/lib/libBox2D.a
