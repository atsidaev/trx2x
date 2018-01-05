LINK.o = $(LINK.cc)
CC = g++

trx2x: trx2x.o DiskImage.o

clean:
	rm *.o trx2x *.exe *.obj
