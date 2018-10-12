exec=testSend testRecv testSR1 testsync save2file testfde testtran udpRecv
CFLAGS=-I../include -O2 -msse2
LIB = ../lib
FFTLIB = /usr/local/lib
all: ${exec}

testSend:testSend.c testHelper.c
	gcc ${CFLAGS} -o testSend testSend.c testHelper.c

testRecv:testRecv.c
	gcc ${CFLAGS} -o testRecv testRecv.c
testSR1:testSR1.c testHelper.c
	gcc ${CFLAGS} -o testSR1 testSR1.c testHelper.c	
testsync:testsync.cpp $(LIB)/hds.a
	g++ ${CFLAGS} -o testsync testsync.cpp $(LIB)/hds.a
testfde:testfde.cpp $(LIB)/hds.a
	g++ ${CFLAGS} -o testfde testfde.cpp $(LIB)/hds.a $(FFTLIB)/libfftw3f.a

save2file:save2file.c
	gcc ${CFLAGS} -o save2file save2file.c

udpRecv: udpRecv.cpp
	g++ ${CFLAGS} -o udpRecv udpRecv.cpp 


clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.bak ${exec} *.exe

testtran:testtran.cpp
	g++ ${CFLAGS} -o testtran testtran.cpp 


