CC = clang
CFLAGS = -std=c17 -I./include -Wall -O3

SRCS := $(shell echo src/*.c)
OBJS := $(patsubst %.c,%.o,$(subst src/,build/,$(SRCS)))

gtp-static: $(OBJS)
	@mkdir -p lib
	ar -rv lib/gtp.a $^

gtp-dynamic: $(OBJS)
	@mkdir -p lib
	$(CC) -shared -fpic $^ -o lib/gtp.dylib

install: gtp-static gtp-dynamic
	cp -r include/* /usr/local/include
	cp -r lib/* /usr/local/lib

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean
clean:
	@rm -rf build lib