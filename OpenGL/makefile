&all: run
.PHONY: run debug opti configure

configure:
	meson Debug

configure_opti:
	meson DebugOptimized --buildtype debugoptimized

run:
	ninja -C Debug
	cd OpenGL && ../Debug/MonCraft

debug:
	ninja -C Debug
	cd OpenGL && gdb ../Debug/MonCraft

debug_opti:
	ninja -C Debug
	cd OpenGL && gdb ../DebugOptimized/MonCraft

opti:
	ninja -C DebugOptimized
	cd OpenGL && ../DebugOptimized/MonCraft

server:
	ninja -C DebugOptimized
	cd OpenGL && ../DebugOptimized/MonCraftServer

release:
	rm -rf Release
	rm -rf dist
	meson Release --buildtype release
	ninja -C Release
	mkdir dist
	cp Release/MonCraft dist
	mkdir dist/src
	cp -r OpenGL/src/shader dist/src
	cp -r OpenGL/data dist
	rm -r Release

# -O3 \
--source-map-base http://localhost:6931/sources/__SOURCE_MAP_BASE__/ -gsource-map -O0 -s NO_DISABLE_EXCEPTION_CATCHING \

CFLAGS = -fexceptions \
-O3 \
-s WASM=1 \
-pthread -s PTHREAD_POOL_SIZE=4 \
-s USE_SDL=2 \
-s USE_SDL_IMAGE=2 \
-s SDL2_IMAGE_FORMATS='["png"]' \
-s USE_SDL_GFX=2 \
-s USE_FREETYPE=2 \
-s ALLOW_MEMORY_GROWTH=1 \
-s MAX_WEBGL_VERSION=2 \
-s MIN_WEBGL_VERSION=2 \
-s FULL_ES2=1 \
-s FULL_ES3=1 \
--preload-file data --preload-file src -Wno-unused-command-line-argument
# CFLAGS = --source-map-base http://localhost:6931/sources/__SOURCE_MAP_BASE__/ -gsource-map -O0 -s WASM=1 -pthread -s PTHREAD_POOL_SIZE=4 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_GFX=2 -s USE_FREETYPE=2 -s NO_DISABLE_EXCEPTION_CATCHING -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s FULL_ES2=1 -s FULL_ES3=1 -s OFFSCREENCANVAS_SUPPORT=1 --preload-file data --preload-file src
MESON_ARGS = --prefix=${PWD}/WASM --cross-file=emscripten-crossfile.txt --default-library=static --buildtype=debug --wrap-mode=nofallback

configure_wasm:
	rm -rf WASM
	EMMAKEN_CFLAGS="${CFLAGS}" meson WASM ${MESON_ARGS}

wasm:
	rm -rf WASM/data
	rm -rf WASM/src
	mkdir WASM/src
	cp -r OpenGL/data WASM
	cp -r OpenGL/src/shader WASM/src

	EMMAKEN_CFLAGS="${CFLAGS}" ninja -C WASM --verbose

	rm -rf WASM/sources
	mkdir WASM/sources
	mkdir WASM/sources/__SOURCE_MAP_BASE__
	-cp WASM/MonCraft.wasm.map WASM/sources/__SOURCE_MAP_BASE__
	cp -r OpenGL WASM/sources/OpenGL
	rm -f WASM/MonCraft.html
	cp MonCraft.html WASM

run_wasm: wasm
	emrun --no_browser --serve_after_close --serve_root WASM WASM/MonCraft.html
