CC=gcc
CFLAGS=-O2 -Wall `sdl-config --cflags`
LIBS=`sdl-config --libs` -lm -lSDL_ttf

%: model.o view.o controller.o graphics.o %.c
	rm -f $@
	$(CC) $(CFLAGS) model.o view.o controller.o  graphics.o $@.c -o $@ $(LIBS)


graphics.o: graphics.c graphics.h
	rm -f police.h
	touch police.h
	if test -e /usr/include/SDL_ttf.h;           then echo "#define SDL_TTF_OK" > police.h; fi
	if test -e /usr/include/SDL/SDL_ttf.h;       then echo "#define SDL_TTF_OK" > police.h; fi
	if test -e /usr/local/include/SDL_ttf.h;     then echo "#define SDL_TTF_OK" > police.h; fi
	if test -e /usr/local/include/SDL/SDL_ttf.h; then echo "#define SDL_TTF_OK" > police.h; fi
	$(CC) $(CFLAGS) -c graphics.c

model.o: model.c model.h
	$(CC) $(CFLAGS) -c model.c

view.o: view.c view.h
	$(CC) $(CFLAGS) -c view.c

controller.o: controller.c controller.h
	$(CC) $(CFLAGS) -c controller.c

clean:
	rm -f *core
	rm -f *.o
	rm -f police.h
	rm -f exemple
	rm -f *.tar
