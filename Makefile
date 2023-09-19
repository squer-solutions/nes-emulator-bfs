serve:
	npx serve dist

build: clean simple-nes
	mkdir ./dist
	cp webApp/index.html ./dist
	cp webApp/style.css ./dist
	cp webApp/src/index.js ./dist
	cp webApp/src/simple-nes.js ./dist
	cp SimpleNES/build/emulator.wasm ./dist
	cp SimpleNES/build/emulator.data ./dist

clean:
	rm -rf webApp/src/simple-nes 
	rm -rf SimpleNES/build
	rm -rf dist/

simple-nes-build:
	rm -rf SimpleNES/build
	mkdir SimpleNES/build
	docker run --rm -v $(shell pwd):/src -u $(id -u):$(id -g) \
	emscripten/emsdk emcc SimpleNES/context.cpp \
	-ISimpleNES/include/ -ISimpleNES/src/ \
	-o SimpleNES/build/emulator.js \
	--preload-file roms/ \
	--no-entry --bind -sEXPORT_ES6=1 -sMODULARIZE=1

simple-nes: simple-nes-build
	cp -R SimpleNES/build/emulator.js webApp/src/simple-nes.js
