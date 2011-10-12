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
	rm -rf build

