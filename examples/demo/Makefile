CFLAGS = -I../..
ifeq ($(OS),Windows_NT)
	LDFLAGS = -ld3d9
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		LDFLAGS = -framework OpenGL -framework Cocoa
	endif
endif

demo : demo.cpp ../../tigr.c
	g++ $^ -s -Os -o $@ $(CFLAGS) $(LDFLAGS)
