UNAME := $(shell uname)
ifeq ($(UNAME),Linux)
	LDFLAG = -lrt -lpthread -lboost_filesystem -lboost_system
endif
ifeq ($(UNAME),Darwin)
	LDFLAG = -lboost_filesystem -lboost_system
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
	work/namedCondRecv \
	work/layout_test \
	work/mem_test \
	work/heap_test \
	work/time \
	work/regex \
	work/finduuid \
	work/findblock_test \
	work/log_test

CPPFLAG = -std=c++1z 

USRTOSFLAG = -Iusrtos/include

work/ptr_vector:ptr_container/ptr_vector.cpp
	g++ $(CPPFLAG) -o $@ $^

work/readdir:filesystem/readdir.cpp
	g++ $(CPPFLAG) -o $@ $^

work/fib:meta/fibonacci.cpp
	g++ $(CPPFLAG) -o $@ $^

work/qcldpc:meta/QCLDPC.cpp
	g++ $(CPPFLAG) -o $@ $^

work/say:functor/say.cpp
	g++ $(CPPFLAG) -o $@ $^ $(LDFLAG)
		
work/ast:functor/ast.cpp
	g++ $(CPPFLAG) -o $@ $^

work/uuid:uuid/uuid.cpp
	g++ $(CPPFLAG) -o $@ $^

work/time:chrono/time.cpp
	g++ $(CPPFLAG) -o $@ $^

work/regex:regex/regex.cpp
	g++ $(CPPFLAG) -o $@ $^ $(LDFLAG)

work/finduuid:regex/finduuid.cpp
	g++ $(CPPFLAG) -o $@ $^ $(LDFLAG)

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

work/layout_test:usrtos/c++1z/layout_test.cpp
	g++ $(CPPFLAG) $(USRTOSFLAG) -o $@ $^ $(LDFLAG)

work/mem_test:usrtos/c++1z/mem_test.cpp
	g++ $(CPPFLAG) $(USRTOSFLAG) -o $@ $^ $(LDFLAG)

work/heap_test:usrtos/c++1z/heap_test.cpp
	g++ $(CPPFLAG) $(USRTOSFLAG) -o $@ $^ $(LDFLAG)

work/findblock_test:usrtos/c++1z/findblock_test.cpp
	g++ $(CPPFLAG) $(USRTOSFLAG) -o $@ $^ $(LDFLAG)

work/log_test:usrtos/c++1z/log_test.cpp
	g++ $(CPPFLAG) $(USRTOSFLAG) -o $@ $^ $(LDFLAG)

clean:
	echo $(UNAME)
	rm -f work/*
