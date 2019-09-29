CC=gcc
CFLAGS=-O2 -Wall -pg -no-pie `sdl-config --cflags`
LIBS=`sdl-config --libs` -lm -lSDL_ttf
SRCDIR=src
BUILDDIR=build
TARGET=bin/main
INC=-Iinclude
SRCEXT=c
OBJECTS=$(BUILDDIR)/model.o $(BUILDDIR)/view.o $(BUILDDIR)/controller.o $(BUILDDIR)/graphics.o
SOURCES=$(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

$(BUILDDIR)/model.o: $(SRCDIR)/model.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<


$(TARGET): $(OBJECTS)
	$(CC) $^ -o $(TARGET) $(LIBS)

$(BUILDDIR)/graphics.o: $(SRCDIR)/graphics.c
	rm -f include/police.h
	touch include/police.h
	if test -e /usr/include/SDL_ttf.h;           then echo "#define SDL_TTF_OK" > police.h; fi
	if test -e /usr/include/SDL/SDL_ttf.h;       then echo "#define SDL_TTF_OK" > police.h; fi
	if test -e /usr/local/include/SDL_ttf.h;     then echo "#define SDL_TTF_OK" > police.h; fi
	if test -e /usr/local/include/SDL/SDL_ttf.h; then echo "#define SDL_TTF_OK" > police.h; fi
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
$(BUILDIR)/view.o: $(SRCDIR)/view.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(BUILDIR)/controller.o: $(SRCDIR)/controller.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm -f $(BUILDDIR)/*.o
	rm -f $(TARGET)
	rm -f include/police.h
