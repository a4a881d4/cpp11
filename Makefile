all:work/ptr_vector \
	work/fib \
	work/qcldpc

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

clean:
	rm work/* -f
