default: bake

bake:
	mkdir -p build 
	cd build; cmake -DCMAKE_BUILD_TYPE="Release" ..; make -j3

go: bake run

run:
	cd bin; ./energymaster

gdb:
	cd bin; gdb ./energymaster

valgrind:
	cd bin; valgrind --leak-check=yes ./energymaster

win32:
	mkdir -p build-win32
	cd build-win32; cmake -DCMAKE_BUILD_TYPE="Release" .. -DCMAKE_TOOLCHAIN_FILE=../cmake-extras/cmake-toolchain.cmake -DCMAKE_CXX_FLAGS="-mwindows"; make VERBOSE=1 -j3

clean:
	rm -r build
