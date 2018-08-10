// #include <boost/filesystem/config.hpp>
// #include <boost/filesystem/operations.hpp>
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS
#include <iostream>

using namespace boost::filesystem;
using namespace std;

int main()
{
	auto current = current_path();
	if(is_directory(current)) {
		cout << current << " is dir " << endl;
	}
}