default:
	g++ -Wall -Wextra -Werror -pedantic -Wno-unused-parameter -O2 -std=c++0x -lBox2D -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -o AsteroidJam.bin src/*.cpp

debug:
	g++ -Wall -Wextra -Werror -pedantic -Wno-unused-parameter -g -std=c++0x -lBox2D -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system -o AsteroidJam.bin src/*.cpp

win:
	i486-mingw32-g++ -Wall -std=c++0x -O2 -Wno-unused-parameter -o AsteroidJam.exe src/*.cpp /usr/i486-mingw32/lib/libsfml-{audio,graphics,system,window}.a /usr/i486-mingw32/lib/libBox2D.a
