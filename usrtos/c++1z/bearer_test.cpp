#include <capabilityBearer.hpp>
#include <iostream>
#include <usrtworker.hpp>

using namespace std;
using namespace usrtos;
int main(int argc, char *argv[])
{
	UsrtCapabilityBearer b("libWorkersHelloWorld.so");
	struct mainWorkerCTX mCtx;
	UsrtWorkers *workers = new UsrtWorkers( argv[1] );
 	mCtx.workers = workers;
	b.runLP((void*)&mCtx);
	uuid key = b.getKey();
	cout << UsrtKey::key2string(key) << endl;
	return 0;

}