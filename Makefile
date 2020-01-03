ifeq ($(DEBUG),true)
	CC = gcc -g
else
	CC = gcc
endif

IDIR = ../include
CFLAGS = -I$(IDIR)

ODIR = obj
LDIR = ../lib

_DEPS = snakeEater.h client.h server.h gameFuncs.h
DEPS = $(pathsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = client.o server.o gameFuncs.o main.o
OBJ = $(pathsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -o 
