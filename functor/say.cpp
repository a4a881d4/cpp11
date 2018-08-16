#include <iostream>

template <typename T>
struct Say
{
	Say(){};
	operator()(T&& x){
		cout << "say: " << x << endl;

	};
};

int main()
{
	Say<string> s;
	s("OK");
}