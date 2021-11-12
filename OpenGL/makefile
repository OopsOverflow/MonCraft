&all: run
.PHONY: run debug opti configure

configure:
	rm -rf Debug
	CXX=clang++ meson Debug

configure_opti:
	rm -rf DebugOptimized
	CXX=clang++ meson DebugOptimized --buildtype debugoptimized

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
	meson Release --buildtype release -Db_lto=true -Dcpp_args="-Ofast"
	meson configure Release
	ninja --verbose -C Release
	mkdir dist
	cp Release/MonCraft dist
	mkdir dist/src
	cp -r OpenGL/src/shader dist/src
	cp -r OpenGL/data dist
	rm -r Release

# --source-map-base http://localhost:6931/sources/__SOURCE_MAP_BASE__/ -gsource-map -O0 -s NO_DISABLE_EXCEPTION_CATCHING \

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
-s WEBSOCKET_URL='wss://pi.thissma.fr:55001' \
-s WEBSOCKET_SUBPROTOCOL='binary' \
--preload-file data --preload-file src -Wno-unused-command-line-argument
# CFLAGS = --source-map-base http://localhost:6931/sources/__SOURCE_MAP_BASE__/ -gsource-map -O0 -s WASM=1 -pthread -s PTHREAD_POOL_SIZE=4 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_GFX=2 -s USE_FREETYPE=2 -s NO_DISABLE_EXCEPTION_CATCHING -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s FULL_ES2=1 -s FULL_ES3=1 -s OFFSCREENCANVAS_SUPPORT=1 --preload-file data --preload-file src
MESON_ARGS = --prefix=${PWD}/WASM --cross-file=emscripten-crossfile.txt --default-library=static --buildtype=debug --wrap-mode=nofallback

configure_wasm:
	rm -rf WASM
	EMMAKEN_CFLAGS="${CFLAGS}" meson WASM ${MESON_ARGS}

wasm:
	rm -rf WASM/data
	rm -rf WASM/src

	# copy resources
	mkdir WASM/src
	mkdir -p WASM/data/fonts WASM/data/img
	cp -r OpenGL/data/fonts WASM/data
	cp -r OpenGL/data/wasm/img WASM/data
	cp -r OpenGL/src/shader WASM/src

	EMMAKEN_CFLAGS="${CFLAGS}" ninja -C WASM --verbose

	rm -rf WASM/sources
	mkdir WASM/sources
	mkdir -p WASM/sources/__SOURCE_MAP_BASE__/
	mkdir -p WASM/sources/OpenGL/src
	cp WASM/MonCraft.wasm.map WASM/sources/__SOURCE_MAP_BASE__
	cp -r OpenGL/src WASM/sources/OpenGL
	rm -f WASM/MonCraft.html
	cp -r OpenGL/data/wasm/. WASM

run_wasm: wasm
	emrun --no_browser --serve_after_close --serve_root WASM WASM/MonCraft.html
