cpp = g++
UsrInc = ../include
C++FLAGS = -O2 -msse2 -g
LIB = ../lib
work = ../work

OBJ=$(work)/complexoperation.o \
	$(work)/sync.o	\
	$(work)/cfftw.o	\
	$(work)/fde.o	

all: $(LIB)/hds.a
is95Test: $(LIB)/hds.a

$(work)/complexoperation.o : complexoperation.cpp
	$(cpp) $(C++FLAGS) -I$(UsrInc) -c -o $(work)/complexoperation.o complexoperation.cpp

$(work)/sync.o : sync.cpp
	$(cpp) $(C++FLAGS) -I$(UsrInc) -c -o $(work)/sync.o sync.cpp

$(work)/cfftw.o : cfftw.cpp
	$(cpp) $(C++FLAGS) -I$(UsrInc) -c -o $(work)/cfftw.o cfftw.cpp

$(work)/fde.o : fde.cpp
	$(cpp) $(C++FLAGS) -I$(UsrInc) -c -o $(work)/fde.o fde.cpp

$(LIB)/hds.a : $(OBJ)
	rm -f $(LIB)/hds.a
	ar cr $(LIB)/hds.a $(OBJ)
	ranlib $(LIB)/hds.a	

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.bak *.exe $(OBJ) $(LIB)/hds.a
