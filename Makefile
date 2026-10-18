CC=clang
# Tell the compiler where to find your custom headers
CFLAGS=-O0 -g -std=c99
# Tell the linker where to find your custom libraries (L)
# and tell the executable where to find them at runtime (rpath)
LDFLAGS=-lX11 -lm
EXECUTABLE=xlib-pong
OBJECT_FILES=obj/main.o obj/logging.o obj/ll.o obj/x11_display.o obj/globals.o obj/x11_screen.o

$(EXECUTABLE): $(OBJECT_FILES)
	$(CC) -o $(EXECUTABLE) $(OBJECT_FILES) $(LDFLAGS)

obj/main.o: src/main.c src/logging.c src/logging.h src/x11_display.c src/x11_display.h src/globals.c src/globals.h
	$(CC) -c src/main.c -o obj/main.o $(CFLAGS)

obj/logging.o: src/logging.c src/logging.h src/globals.c src/globals.h
	$(CC) -c src/logging.c -o obj/logging.o $(CFLAGS)

obj/ll.o: src/ll.c src/ll.h src/globals.c src/globals.h
	$(CC) -c src/ll.c -o obj/ll.o $(CFLAGS)

obj/x11_display.o: src/x11_display.c src/x11_display.h src/globals.c src/globals.h src/x11_screen.c src/x11_screen.h
	$(CC) -c src/x11_display.c -o obj/x11_display.o $(CFLAGS)

obj/x11_screen.o: src/x11_screen.c src/x11_screen.h src/globals.c src/globals.h
	$(CC) -c src/x11_screen.c -o obj/x11_screen.o $(CFLAGS)

obj/globals.o: src/globals.c src/globals.h
	$(CC) -c src/globals.c -o obj/globals.o $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) $(OBJECT_FILES)
