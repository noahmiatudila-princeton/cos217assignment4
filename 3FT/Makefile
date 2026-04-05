#--------------------------------------------------------------------
# Makefile for Assignment 4, Part 3
# Author: Noah Miatudila
#--------------------------------------------------------------------

GCC = gcc217
#GCC = gcc217m

all: ft

ft: ft_client.o ft.o nodeFT.o checkerFT.o path.o dynarray.o
	$(GCC) -g ft_client.o ft.o nodeFT.o checkerFT.o path.o dynarray.o -o ft

ft_client.o: ft_client.c ft.h a4def.h
	$(GCC) -g -c ft_client.c

ft.o: ft.c ft.h dynarray.h path.h nodeFT.h checkerFT.h a4def.h
	$(GCC) -g -c ft.c

nodeFT.o: nodeFT.c nodeFT.h dynarray.h path.h checkerFT.h a4def.h
	$(GCC) -g -c nodeFT.c

checkerFT.o: checkerFT.c checkerFT.h dynarray.h path.h nodeFT.h a4def.h
	$(GCC) -g -c checkerFT.c

path.o: path.c path.h dynarray.h a4def.h
	$(GCC) -g -c path.c

dynarray.o: dynarray.c dynarray.h
	$(GCC) -g -c dynarray.c

clean:
	rm -f ft *.o