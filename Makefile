OBJS = linked_list.o vertex.o mesh.o main.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
BUILDDIR = _build

all: prog clean

prog: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(BUILDDIR)/mesh_gen

main.o: main.h main.cc
	$(CC) $(CFLAGS) main.cc

linked_list.o: linked_list.h linked_list.cc
	$(CC) $(CFLAGS) linked_list.cc

vertex.o: vertex.h vertex.cc linked_list.h
	$(CC) $(CFLAGS) vertex.cc

mesh.o: mesh.h mesh.cc vertex.h
	$(CC) $(CFLAGS) mesh.cc

clean:
	rm -f *.o
