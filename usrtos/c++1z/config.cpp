#include <version.hpp>
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
	
	std::string getKey(std::string capName) {
		uuid key = WorkerHelper::cap2key(std::string("capWorkers") + capName);
		std::string ks = UsrtKey::key2string(key);
		return ks;
	};

	void HelloWorld() {
		task *pTask = WorkerHelper::newConfig(m_workers,std::string("HelloWorld"));
		m_workers->m_configFifo->push<task>(pTask);
	};

	void byKeyInt(std::string key, int k) {
		CPBlock::GP gpTask;
		task *pTask = WorkerHelper::newConfigTask(m_workers,gpTask)
			-> setID(0)
			-> setKey(UsrtKey::string2key(key));
		CPBlock::GP& argv = pTask->getArgv();
		int *a = m_workers->m_memory->newGP<int>(argv);
		*a = k;
		m_workers->m_configFifo->push<task>(pTask);
	};

	void byKey(std::string key) {
		CPBlock::GP gpTask;
		task *pTask = WorkerHelper::newConfigTask(m_workers,gpTask)
			-> setID(0)
			-> setKey(UsrtKey::string2key(key));
		m_workers->m_configFifo->push<task>(pTask);
	};
};

BOOST_PYTHON_MODULE(usrtconfig)
{
	class_<UsrtConfig>("UsrtConfig", init<std::string>())
		.def("HelloWorld", &UsrtConfig::HelloWorld)
		.def("byKeyInt", &UsrtConfig::byKeyInt)
		.def("byKey", &UsrtConfig::byKey)
		.def("getKey", &UsrtConfig::getKey)
	;

}
};

