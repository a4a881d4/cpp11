#include<iostream>
#include<chrono>
using namespace std;
int main()
{
	cout << "millisecond : ";
	cout << std::chrono::milliseconds::period::num << "/" << std::chrono::milliseconds::period::den << "s" <<endl;
	chrono::time_point<chrono::system_clock, chrono::milliseconds> tp(chrono::seconds(1000));  
    cout << "to epoch : " <<tp.time_since_epoch().count() << "s" <<endl; 
    cout << sizeof(tp) << " " << hex << *(long long*)&tp << endl; 

	return 0;
}
