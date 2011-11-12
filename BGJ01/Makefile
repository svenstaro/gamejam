default: debug run

clean:
	[[ -d build ]] && rm -r build

release:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE=Release ..; make -j$(shell grep -c ^processor /proc/cpuinfo)

debug:
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE=Debug ..; make -j$(shell grep -c ^processor /proc/cpuinfo)

run:
	bin/cannonium
