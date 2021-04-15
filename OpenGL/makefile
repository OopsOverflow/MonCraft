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

opti:
	ninja -C DebugOptimized
	cd OpenGL && ../DebugOptimized/MonCraft
