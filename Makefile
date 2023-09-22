# demo-clean:
# 	rm -rf demoApp/build
# 	mkdir demoApp/build

# demo: demo-clean
# 	docker run --rm -v $(shell pwd):/src -u $(id -u):$(id -g) emscripten/emsdk \
# 	emcc

# simple-nes-clean:
# 	rm -rf webApp/src/simple-nes 
# 	rm -rf SimpleNES/build
# 	mkdir SimpleNES/build

# simple-nes: simple-nes-clean
# 	docker run --rm -v $(shell pwd):/src -u $(id -u):$(id -g) emscripten/emsdk
# 	emcc

serve:
	npx serve dist

build: clean
	mkdir ./dist
	cp -a webApp/ ./dist

clean:
	rm -rf dist/

get-nes-code:
	git cherry-pick 0e7935769b9838e32b75bd90c8494e828e4dd767
