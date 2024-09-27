LINK.o = $(LINK.cc)
CC = g++

trx2x: trx2x.o DiskImage.o

clean:
	rm -f *.o trx2x *.exe *.obj
