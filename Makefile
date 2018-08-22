UNAME := $(shell uname)
ifeq ($(UNAME),Linux)
	LDFLAG = -lrt -lpthread
endif
ifeq ($(UNAME),Darwin)
	LDFLAG = 
endif

all:work/ptr_vector \
	work/fib \
	work/qcldpc \
	work/say \
	work/ast \
	work/uuid \
	work/sharedMemory \
	work/namedSharedMemory \
	work/offsetPtr \
	work/vectorInSharedMemory \
	work/mapInSharedMemory \
	work/sharedRegion \
	work/mappedFile \
	work/writingTrace \
	work/namedCondSent \
	work/namedCondRecv

CPPFLAG = -std=c++1z



work/ptr_vector:ptr_container/ptr_vector.cpp
	g++ $(CPPFLAG) -o $@ $^

#-L/usr/lib/x86_64-linux-gnu/ -lboost_filesystem

work/readdir:filesystem/readdir.cpp
	g++ $(CPPFLAG) -o $@ $^

work/fib:meta/fibonacci.cpp
	g++ $(CPPFLAG) -o $@ $^

work/qcldpc:meta/QCLDPC.cpp
	g++ $(CPPFLAG) -o $@ $^

work/say:functor/say.cpp
	g++ $(CPPFLAG) -o $@ $^
		
work/ast:functor/ast.cpp
	g++ $(CPPFLAG) -o $@ $^

work/uuid:uuid/uuid.cpp
	g++ $(CPPFLAG) -o $@ $^

work/sharedMemory:interprocess/sharedMemory.cpp
	g++ $(CPPFLAG) -o $@ $^ $(LDFLAG)

work/namedSharedMemory:interprocess/namedSharedMemory.cpp
	g++ $(CPPFLAG) -o $@ $^ $(LDFLAG)

work/offsetPtr:interprocess/offsetPtr.cpp
	g++ $(CPPFLAG) -o $@ $^ $(LDFLAG)

work/vectorInSharedMemory:interprocess/vectorInSharedMemory.cpp
	g++ $(CPPFLAG) -o $@ $^ $(LDFLAG)

work/mapInSharedMemory:interprocess/mapInSharedMemory.cpp
	g++ $(CPPFLAG) -o $@ $^ $(LDFLAG)

work/sharedRegion:interprocess/sharedRegion.cpp
	g++ $(CPPFLAG) -o $@ $^ $(LDFLAG)

work/mappedFile:interprocess/mappedFile.cpp
	g++ $(CPPFLAG) -o $@ $^ $(LDFLAG)

work/writingTrace:interprocess/writingTrace.cpp
	g++ $(CPPFLAG) -o $@ $^ $(LDFLAG)

work/namedCondSent:interprocess/namedCondSent.cpp
	g++ $(CPPFLAG) -o $@ $^ $(LDFLAG)

work/namedCondRecv:interprocess/namedCondRecv.cpp
	g++ $(CPPFLAG) -o $@ $^ $(LDFLAG)

clean:
	echo $(UNAME)
	rm -f work/*
