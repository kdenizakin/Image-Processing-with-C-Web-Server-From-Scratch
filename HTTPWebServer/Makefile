
CC = gcc
CFLAGS = $(shell pkg-config --cflags MagickWand)
LDFLAGS = $(shell pkg-config --libs MagickWand)
SRC = main.c Server.c Request.c Response.c ProcessImageWithMagickWand.c
OBJ = $(SRC:.c=.o)
TARGET = cserver

# Hedef burası
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

# Temizlemek için clean commandi
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
