all: build

.PHONY: run
run: build
	build/visigoth

.PHONY: build
build:
	mkdir -p build
	cd build && qmake ../visigoth/visigoth.pro
	make -C build

.PHONY: debug
debug:
	mkdir -p build
	cd build && qmake ../visigoth/visigoth.pro CONFIG-=release
	make -C build

.PHONY: test
test:
	mkdir -p test
	cd test && qmake ../visigoth/visigoth.pro CONFIG+=test
	make -C test
	test/test

.PHONY: local-cover
local-cover: test
	#lcov --directory test --zerocounters
	lcov --directory test --capture --output-file test/coverage.info
	genhtml --output-directory test test/coverage.info
	xdg-open "$(shell pwd)/test$(shell pwd)/visigoth/index.html"

.PHONY: ci-cover
ci-cover:
	gcovr --xml -d test > test/visigoth.xml

.PHONY: clean
clean:
	rm -rf build profile test
	rm -rf visigoth-*

.PHONY: profile
profile: build
	mkdir -p profile
	CPUPROFILE=profile/visigoth.prof LD_PRELOAD=/usr/lib/libprofiler.so build/visigoth
	pprof --text build/visigoth profile/visigoth.prof > profile/visigoth.report


