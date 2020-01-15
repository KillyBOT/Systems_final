ifeq ($(DEBUG),true)
	CC = gcc -g
else
	CC = gcc
endif

IDIR = ../include
CFLAGS = -I$(IDIR)
LIBS = -lSDL2 -lm

ODIR = obj
LDIR = ../lib

_DEPS = snakeEater.h gameFuncs.h graphics.h game.h network.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ_CLIENT = game.o gameFuncs.o graphics.o network.o
OBJ_CLIENT = $(patsubst %,$(ODIR)/%,$(_OBJ_CLIENT))

_OBJ_SERVER = network.o gameFuncs.o server.o
OBJ_SERVER = $(patsubst %,$(ODIR)/%,$(_OBJ_SERVER))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

game: $(OBJ_CLIENT)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

server: $(OBJ_SERVER)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(ODIR)/*.o