#include <iostream>
#include <string>

using namespace std;

template <typename T>
struct Say
{
	Say(){};
	void operator()(T&& x){
		cout << "say: " << x << endl;

	};
};

int main()
{
	Say<string> say;
	say("ok");
}