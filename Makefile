CC=musl-gcc
# Tell the compiler where to find your custom headers
CFLAGS=-O0 -g3 -I/usr/local/include
# Tell the linker where to find your custom libraries (L)
# and tell the executable where to find them at runtime (rpath)
LDFLAGS=-L/usr/local/lib -Wl,--enable-new-dtags,-rpath,/usr/local/lib -lX11 -lm
EXECUTABLE=xlib-pong
OBJECT_FILES=obj/main.o obj/logging.o

$(EXECUTABLE): $(OBJECT_FILES)
	$(CC) -o $(EXECUTABLE) $(OBJECT_FILES) $(LDFLAGS)

obj/main.o: src/main.c src/logging.h
	$(CC) -c src/main.c -o obj/main.o $(CFLAGS)

obj/logging.o: src/logging.c src/logging.h
	$(CC) -c src/logging.c -o obj/logging.o $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) $(OBJECT_FILES)
