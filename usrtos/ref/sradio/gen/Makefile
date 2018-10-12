
exe: genSync gentran
LIB = ../lib
CFLAGS = -I../include -O2 -msse2

genSync:genSync.c
	gcc -lm -o genSync genSync.c

testsyncdata:testsyncdata.cpp
	g++ $(CFLAGS) -lm -o testsyncdata testsyncdata.cpp $(LIB)/sync.a $(LIB)/complexoperation.a 

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.bak *.exe $(exe)

syncdata:testsyncdata.cpp
	g++ -I../../HD_Simplex_BS_Mac48/Include -O2 -msse2 -lm -o testsync testsyncdata.cpp ../../HD_Simplex_BS_Mac48/Lib/Sync.a ../../HD_Simplex_BS_Mac48/Lib/ComplexOperation.a 

gentran:gentran.c
	gcc -o gentran gentran.c

all:exe