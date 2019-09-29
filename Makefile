CC=gcc
CFLAGS=-O2 -Wall `sdl-config --cflags -Isrc/include`
LIBS=`sdl-config --libs` -lm -lSDL_ttf
SOURCE_DIR=src/
INC_DIR=src/include/

%: model.o view.o controller.o graphics.o src/%.c
	rm -f $@
	$(CC) $(CFLAGS) model.o view.o controller.o graphics.o src/$@.c -o $@ $(LIBS)


graphics.o: src/graphics.c src/include/graphics.h
	rm -f police.h
	touch police.h
	if test -e /usr/include/SDL_ttf.h;           then echo "#define SDL_TTF_OK" > police.h; fi
	if test -e /usr/include/SDL/SDL_ttf.h;       then echo "#define SDL_TTF_OK" > police.h; fi
	if test -e /usr/local/include/SDL_ttf.h;     then echo "#define SDL_TTF_OK" > police.h; fi
	if test -e /usr/local/include/SDL/SDL_ttf.h; then echo "#define SDL_TTF_OK" > police.h; fi
	$(CC) $(CFLAGS) -c src/graphics.c

model.o: src/model.c src/include/model.h
	$(CC) $(CFLAGS) -c src/model.c

view.o: src/view.c src/include/view.h
	$(CC) $(CFLAGS) -c src/view.c

controller.o: src/controller.c src/include/controller.h
	$(CC) $(CFLAGS) -c src/controller.c

clean:
	rm -f *core
	rm -f *.o
	rm -f src/include/police.h
	rm -f exemple
	rm -f *.tar
