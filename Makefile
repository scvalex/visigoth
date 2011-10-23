all: build

.PHONY: run
run: build
	build/visigoth

.PHONY: build
build:
	mkdir -p build
	cd build && qmake ../visigoth/visigoth.pro
	make -C build

.PHONY: clean
clean:
	rm -rf build profile
	rm -rf visigoth-*

.PHONY: profile
profile: build
	mkdir -p profile
	CPUPROFILE=profile/visigoth.prof LD_PRELOAD=/usr/lib/libprofiler.so build/visigoth
	pprof --text build/visigoth profile/visigoth.prof > profile/visigoth.report
