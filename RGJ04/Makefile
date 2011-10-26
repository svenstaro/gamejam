default:
	g++ -O3 -Wall -Wextra -Werror -Wno-unused-parameter -std=c++0x -lsfml-graphics -lsfml-window -lsfml-system -I/usr/include/bullet -lBulletCollision -lBulletDynamics src/*.cpp -opolymorphosis.bin

debug:
	g++ -g -Wall -Wextra -Werror -Wno-unused-parameter -std=c++0x -lsfml-graphics -lsfml-window -lsfml-system -I/usr/include/bullet -lBulletCollision -lBulletDynamics src/*.cpp -opolymorphosis.bin

win:
	i486-mingw32-g++ -I/usr/i486-mingw32/include/bullet/ src/*.cpp -lsfml-graphics -lsfml-window -lsfml-system -lBulletCollision -lBulletDynamics -lLinearMath -static-libgcc -static-libstdc++ -opolymorphosis.exe
