#include <iostream>
#include <typeinfo>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/bind.hpp>

using namespace std;
using namespace boost;

void two(int n,string& x) {
	for(int i=0;i<n;i++)
		cout<< x << endl;
}

void twoSwap(string& x,int n) {
	cout<< n << " "<< x << endl;
}

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

	for_each( vec.begin()
		, vec.end()
		, bind(two,2,_1)
		);

	for_each( vec.begin()
		, vec.end()
		, bind(twoSwap,_1, 2)
		);
	
	vec.clear();
}