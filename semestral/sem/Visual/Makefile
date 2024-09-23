
CFLAGS+= -Wall -Werror -std=gnu99 -g
LDFLAGS= -lpthread -lm

BINARIES=prgsem-comp

CFLAGS+=$(shell sdl2-config --cflags)
LDFLAGS+=$(shell sdl2-config --libs) -lSDL2_image


all: clean prgsem-comp 

OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

prgsem-comp: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(BINARIES) $(OBJS)
