#include <boost/python.hpp>
#include <iostream>

using namespace boost::python;
typedef struct _globe_pointer {
	long long id;
	size_t offset;
	size_t objsize;
	void dump() {
		std::cout << " id:" << id 
			<< " os:" << offset 
			<< " sz:" << objsize 
			<< std::endl;
	};
} GP;

char const* greet()
{
		return "hello, world";
}

BOOST_PYTHON_MODULE(hello_ext)
{
	def("greet", greet);
	;
	class_<_globe_pointer>("GP")
		.def("dump",&_globe_pointer::dump)
		.def_readwrite("offset",&_globe_pointer::offset)
	;
}