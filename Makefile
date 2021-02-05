CC=gcc
CFLAGS=-O2 -Wall -g `sdl-config --cflags`
LIBS=`sdl-config --libs` -lm -lSDL_ttf
SRCDIR=src
BUILDDIR=build
TARGET=bin/main
INC=-Iinclude
SRCEXT=c
OBJECTS=$(BUILDDIR)/model.o $(BUILDDIR)/view.o $(BUILDDIR)/controller.o $(BUILDDIR)/graphics.o
SOURCES=$(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INC) $^ src/main.c -o $(TARGET) $(LIBS)

build/graphics.o: $(SRCDIR)/graphics.c
	mkdir -p bin
	rm -f include/police.h
	touch include/police.h
	if test -e /usr/include/SDL_ttf.h;           then echo "#define SDL_TTF_OK" > include/police.h; fi
	if test -e /usr/include/SDL/SDL_ttf.h;       then echo "#define SDL_TTF_OK" > include/police.h; fi
	if test -e /usr/local/include/SDL_ttf.h;     then echo "#define SDL_TTF_OK" > include/police.h; fi
	if test -e /usr/local/include/SDL/SDL_ttf.h; then echo "#define SDL_TTF_OK" > include/police.h; fi
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

build/model.o: $(SRCDIR)/model.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

build/view.o: $(SRCDIR)/view.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

build/controller.o: $(SRCDIR)/controller.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm -f $(BUILDDIR)/*.o
	rm -f $(TARGET)
	rm -f include/police.h
