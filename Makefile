UNAME := $(shell uname)

CPPFLAG = -std=c++1z
LDFLAG = -lboost_filesystem -lboost_system
PYFLAG = 
ifeq ($(UNAME),Linux)
	LDFLAG += -lrt -lpthread -lboost_python3 -ldl
	PYTHON_INCLUDE = /usr/include/python3.6m/
endif
ifeq ($(UNAME),Darwin)
	PYFLAG += -lboost_python37 -lpython3.7
	PYTHON_INCLUDE = /usr/local/Cellar/python/3.7.0/Frameworks/Python.framework/Versions/3.7/include/python3.7m/
	LIBPYTHON_PATH = /usr/local/Cellar/python/3.7.0/Frameworks/Python.framework/Versions/3.7/lib/
	CPPFLAG += -L$(LIBPYTHON_PATH)
endif
	CPPFLAG += -I$(PYTHON_INCLUDE) 

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
	work/mem_test \
	work/heap_test \
	work/time \
	work/regex \
	work/finduuid \
	work/findblock_test \
	work/log_test \
	work/hello_ext.so \
	work/usrtos.so \
	work/bearer_test \
	work/workers \
	work/dag_test \
	work/vm_test \
	work/task_test \
	work/exception_test



# work/layout_test \
	
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

work/heap_test:usrtos/c++1z/heap_test.cpp work/usrtoslib.so
	g++ $(CPPFLAG) $(USRTOSFLAG) -o $@ $^ $(LDFLAG)

work/findblock_test:usrtos/c++1z/findblock_test.cpp
	g++ $(CPPFLAG) $(USRTOSFLAG) -o $@ $^ $(LDFLAG)

work/log_test:usrtos/c++1z/log_test.cpp
	g++ $(CPPFLAG) $(USRTOSFLAG) -o $@ $^ $(LDFLAG)

work/bearer_test:usrtos/c++1z/bearer_test.cpp work/usrtoslib.so work/system.so
	g++ $(CPPFLAG) $(USRTOSFLAG) -o $@ $^ $(LDFLAG)

work/vm_test:usrtos/c++1z/vm_test.cpp work/system.so work/usrtoslib.so
	g++ $(CPPFLAG) $(USRTOSFLAG) -o $@ $^ $(LDFLAG)

work/task_test:usrtos/c++1z/task_test.cpp work/system.so
	g++ $(CPPFLAG) $(USRTOSFLAG) -o $@ $^ $(LDFLAG)

work/exception_test:usrtos/c++1z/exception_test.cpp
	g++ $(CPPFLAG) $(USRTOSFLAG) -o $@ $^ $(LDFLAG)

usrtos/c++1z/dag_test.cpp:usrtos/include/dag/nodes.hpp
	touch usrtos/c++1z/dag_test.cpp

work/dag_test:usrtos/c++1z/dag_test.cpp
	g++ $(CPPFLAG) $(USRTOSFLAG) -o $@ $^ $(LDFLAG)

work/workers:usrtos/c++1z/workers.cpp work/usrtoslib.so work/system.so
	g++ $(CPPFLAG) $(USRTOSFLAG) -o $@ $^ $(LDFLAG)

work/hello_ext.so: python/hello.cpp
	g++ $(CPPFLAG) $(USRTOSFLAG) -fPIC -shared python/hello.cpp -o work/hello_ext.so $(PYFLAG) $(LDFLAG)

work/usrtos.so: usrtos/c++1z/usrt.cpp work/usrtoslib.so work/system.so
	g++ $(CPPFLAG) $(USRTOSFLAG) -fPIC -shared $^ -o $@ $(LDFLAG) $(PYFLAG)

clean:
	echo $(UNAME)
	rm -f work/*

WorkersInternalCapabilities := $(wildcard usrtos/workers/cap*.cpp)
WorkersInternalLibs := $(patsubst %.cpp,%.so,$(subst usrtos/workers/cap,work/lib,$(WorkersInternalCapabilities)))

$(WorkersInternalLibs): %.so: $(patsubst %.so,%.cpp,$(subst work/lib,usrtos/workers/cap,$@)) work/system.so work/usrtoslib.so
	g++ -std=c++1z ${USRTOSFLAG} -fPIC -shared $(patsubst %.so,%.cpp,$(subst work/lib,usrtos/workers/cap,$@)) work/system.so work/usrtoslib.so -o $@ $(LDFLAG)

UserCapabilities := $(wildcard usrtos/examples/cap*.cpp)
UserLibs := $(patsubst %.cpp,%.so,$(subst usrtos/examples/cap,work/lib,$(UserCapabilities)))

$(UserLibs): %.so: $(patsubst %.so,%.cpp,$(subst work/lib,usrtos/examples/cap,$@)) work/usrtoslib.so work/system.so
	g++ -std=c++1z ${USRTOSFLAG} -fPIC -shared $(patsubst %.so,%.cpp,$(subst work/lib,usrtos/examples/cap,$@)) work/system.so work/usrtoslib.so -o $@ $(LDFLAG) 

workers : $(WorkersInternalLibs)
	
examples : $(UserLibs)

reworker : 
	rm -f $(WorkersInternalLibs)
	rm -f work/workers
	make all
	make workers

dag : work/dag_test
	work/dag_test > work/gr.dot
	dot work/gr.dot -Tpng -o work/gr.png

scriptDep = work/usrtos.so work/libWorkersScript.so
script:
	rm -f $(scriptDep)
	make all
	make workers

work/usrtoslib.so : usrtos/c++1z/usrtoslib.cpp
	g++ -std=c++1z -Iusrtos/include -fPIC -shared -o $@ $^ $(LDFLAG)

work/system.so : $(WorkersInternalCapabilities) usrtos/workers/system.cpp work/usrtoslib.so
	g++ -std=c++1z -DSYSTEM_FUNCTION -Iusrtos/include -Iusrtos/workers -fPIC -shared -o $@ $^ $(LDFLAG) 