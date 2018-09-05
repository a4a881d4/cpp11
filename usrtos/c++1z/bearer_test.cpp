#include <capabilityBearer.hpp>
#include <iostream>

using namespace std;
using namespace usrtos;
int main()
{
	USRTCapabilityBearer b("libWorkersHelloWorld.so");
	
	b.runLP(NULL);
	uuid key = b.getKey();
	cout << UsrtKey::key2string(key) << endl;
	return 0;

}