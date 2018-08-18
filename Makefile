all:work/ptr_vector \
	work/fib \
	work/qcldpc \
	work/say \
	work/ast \
	work/uuid


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

clean:
	rm work/* -f
