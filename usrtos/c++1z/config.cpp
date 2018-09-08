#include <usrtworker.hpp>
#include <workerhelper.hpp>
#include <boost/python.hpp>
using namespace boost::python;

namespace usrtos {

class UsrtConfig {
private:
	UsrtWorkers *m_workers;
public:
	UsrtConfig(std::string dir) {
		m_workers = new UsrtWorkers(dir.c_str());
	};
	
	void HelloWorld() {
		task *pTask = WorkerHelper::newConfig(m_workers,std::string("HelloWorld"));
		std::cout << "in config" << UsrtKey::key2string(pTask->key) << endl;
		m_workers->m_configFifo->push<task>(pTask);
	};
};

BOOST_PYTHON_MODULE(usrtconfig)
{
	class_<UsrtConfig>("UsrtConfig", init<std::string>())
		.def("HelloWorld", &UsrtConfig::HelloWorld)
	;

}
};

