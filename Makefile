demo-clean:
	rm -rf demoApp/build
	mkdir demoApp/build

demo: demo-clean
	docker run --rm -v $(shell pwd):/src -u $(id -u):$(id -g) emscripten/emsdk \
	emcc demoApp/demo.cpp -o demoApp/build/demo.js \
	-s WARN_ON_UNDEFINED_SYMBOLS=0

simple-nes-clean:
	rm -rf webApp/src/simple-nes 
	rm -rf SimpleNES/build
	mkdir SimpleNES/build

simple-nes: simple-nes-clean
	docker run --rm -v $(shell pwd):/src -u $(id -u):$(id -g) \
	emscripten/emsdk emcc SimpleNES/context.cpp \
	-ISimpleNES/include/ -ISimpleNES/src/ \
	-o SimpleNES/build/emulator.js \
	--preload-file roms/ \
	--no-entry --bind -sEXPORT_ES6=1 -sMODULARIZE=1

serve:
	npx serve dist

build: clean demo simple-nes
	mkdir ./dist
	cp webApp/index.html ./dist
	cp webApp/style.css ./dist
	cp webApp/src/index-demo.js ./dist
	cp webApp/src/index.js ./dist
	cp demoApp/build/demo.js ./dist
	cp demoApp/build/demo.wasm ./dist
	cp SimpleNES/build/emulator.js ./dist/simple-nes.js
	cp SimpleNES/build/emulator.wasm ./dist
	cp SimpleNES/build/emulator.data ./dist

clean: simple-nes-clean demo-clean
	rm -rf dist/
