OBJS = linked_list.o robust_predicates.o mesh.o main.o
CC = g++
DEBUG = -g
GLLIBS = -lGLEW -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread
CFLAGS = -Wall -std=gnu++0x -c $(DEBUG)
LFLAGS = -Wall -std=gnu++0x $(DEBUG)
BUILDDIR = _build
EXEC = $(BUILDDIR)/mesh_gen

all: prog clean run

prog: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(EXEC) $(GLLIBS)

main.o: main.h main.cc
	$(CC) $(CFLAGS) main.cc

linked_list.o: linked_list.h linked_list.cc
	$(CC) $(CFLAGS) linked_list.cc

robust_predicates.o: robust_predicates.h robust_predicates.cc
	$(CC) $(CFLAGS) robust_predicates.cc

mesh.o: mesh.h mesh.cc
	$(CC) $(CFLAGS) mesh.cc

clean:
	rm -f *.o

run:
	$(EXEC)
