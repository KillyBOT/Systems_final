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

_DEPS = snakeEater.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = server.o client.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

client: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(ODIR)/*.o