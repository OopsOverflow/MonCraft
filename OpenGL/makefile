all: run
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
