
CFLAGS+= -Wall -Werror -std=gnu99 -g
LDFLAGS= -lpthread

BINARIES=prgsem

CFLAGS+=$(shell sdl2-config --cflags)
LDFLAGS+=$(shell sdl2-config --libs) -lSDL2_image


all: clean $(BINARIES)

OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

prgsem: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(BINARIES) $(OBJS)
