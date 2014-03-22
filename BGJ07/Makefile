default: create

create:
	mkdir -p build
	mkdir -p bin
	cd build; cmake ..; make -j7

run: create
	./bin/noisyhunter.bin

clean:
	rm -rf build
	rm -rf bin
