default: bake

bake:
	mkdir -p build 
	cd build; cmake ..; make -j3

go: build bake run

run:
	cd bin; ./ai-and-the-bomb

gdb:
	cd bin; gdb ./ai-and-the-bomb

win32:
	mkdir -p build-win32
	cd build-win32; cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake-extras/cmake-toolchain.cmake; make -j3

clean:
	rm -r build
	rm -r build-win32
