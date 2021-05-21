CC = clang
CFLAGS = -std=c17 -I./include -Wall -O3

UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	DYN_EXT = dylib
else
	DYN_EXT = so
endif

SRCS := $(shell echo src/*.c)
OBJS := $(patsubst %.c,%.o,$(subst src/,build/,$(SRCS)))

gtp: gtp-static gtp-dynamic

install: gtp
	rm -rf /usr/local/include/GTP
	cp -r include/GTP /usr/local/include
	cp -r lib/libgtp.a lib/libgtp.$(DYN_EXT) /usr/local/lib

gtp-static: $(OBJS)
	@mkdir -p lib
	@ar -r lib/libgtp.a $^

gtp-dynamic: $(OBJS)
	@mkdir -p lib
	$(CC) -shared -fpic $^ -o lib/libgtp.$(DYN_EXT)

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $^ -o $@

.PHONY: clean
clean:
	@rm -rf build lib