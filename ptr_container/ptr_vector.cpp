#include <iostream>
#include <typeinfo>
#include <boost/ptr_container/ptr_vector.hpp>
using namespace std;
using namespace boost;

int main()
{
	typedef ptr_vector<string> ptr_vec;
	ptr_vec vec;
	vec.push_back(new string("123"));
	vec.push_back(new string("abc"));
	vec.push_back(new string("ABC"));
	vec.push_back(new string("EFG"));

	for(auto a : vec){
		cout<<a<<endl;
	}
	vec.clear();
}