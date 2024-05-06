CC=clang
CFLAGS=-O2
LDFLAGS=-lX11
EXECUTABLE=fuckassface
OBJECT_FILES=obj/main.o

$(EXECUTABLE): $(OBJECT_FILES)
	$(CC) -o $(EXECUTABLE) $(OBJECT_FILES) $(LDFLAGS)

obj/main.o: src/main.c
	$(CC) -c src/main.c -o obj/main.o $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(EXECUTABLE) $(OBJECT_FILES)
