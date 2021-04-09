all: run
.PHONY: run debug opti configure

configure:
	meson Debug

run:
	ninja -C Debug
	cd OpenGL && ../Debug/MonCraft

debug:
	cd OpenGL && gdb ../Debug/MonCraft
	
opti:
	ninja -C DebugOptimized
	cd OpenGL && ../DebugOptimized/MonCraft
