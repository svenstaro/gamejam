default: bake

bake:
	mkdir -p build 
	cd build; cmake ..; make -j3

bakerun: build bake run

run:
	cd bin; ./loverobot-from-space

gdb:
	cd bin; gdb ./loverobot-from-space

clean:
	rm -r build
