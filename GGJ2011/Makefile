default: bake

bake:
	mkdir -p build 
	cd build; cmake ..; make -j3

go: build bake run

run:
	cd bin; ./ai-and-the-bomb

gdb:
	cd bin; gdb ./ai-and-the-bomb

clean:
	rm -r build
