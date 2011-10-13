default: bake

bake:
	mkdir -p build 
	cd build; cmake ..; make -j3

go: bake run

run:
	cd bin; ./ai-and-the-bomb

gdb:
	cd bin; gdb ./ai-and-the-bomb

win32:
	mkdir -p build-win32
	cd build-win32; cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake-extras/cmake-toolchain.cmake -DCMAKE_CXX_FLAGS="-mwindows"; make VERBOSE=1 -j3

win32-package:
	cd packaging && ./make_win32_package.sh
	cp packaging/ai-and-the-bomb.msi .

source-package:
	zip -r ai-and-the-bomb.zip AUTHORS cmake-extras CMakeLists.txt data license.txt Makefile README src TODO

clean:
	rm -r build
	rm -r build-win32
