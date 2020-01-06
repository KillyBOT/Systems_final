ifeq ($(DEBUG),true)
	CC = gcc -g
else
	CC = gcc
endif

IDIR = ../include
CFLAGS = -I$(IDIR)
LIBS = -lSDL2

ODIR = obj
LDIR = ../lib

_DEPS = snakeEater.h gameFuncs.h graphics.h game.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = game.o gameFuncs.o graphics.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

game: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(ODIR)/*.o