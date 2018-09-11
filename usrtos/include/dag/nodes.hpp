#pragma once

#include <string>
#include <sstream>
#include <map>
#include <vector>

namespace usrtos {
namespace dag {

using namespace std;

class Var;
class Func;
typedef pair<Func*,string> Args;
typedef pair<Var*,string> Bind;

class NodeBase {
public:
	enum NodeType {
		  variable = 0
		, temporary
		, function
	};
	static map<string,NodeBase*> symbol;
	std::string name;
	NodeType type;
	NodeBase() {};
	NodeBase(string& n) {
		name = n;
		NodeBase::symbol[name] = this;
	};
	NodeBase *setName(string&& n) {
		name = n;
		NodeBase::symbol[name] = this;
		return this;
	};
	NodeBase *setType(NodeType t) {
		type = t;
		return this;
	};
	static map<string,NodeBase*> Funcs() {
		map<string,NodeBase*> r;
		for(auto it = NodeBase::symbol.begin();it != NodeBase::symbol.end();++it) {
			if(it->second->type == function)
				r.insert(*it);
		}
		return r;
	};
	static string Dot();
	static string DotwithV();
};


struct TempState {
	int t;
};

class Var : public NodeBase {
public:
	vector<Args> m_asOut;
	vector<Args> m_asIn;
	size_t m_size;	
public:
	Var(const char* n) : NodeBase() {
		(NodeBase *)this->setName(string(n));
		type = NodeBase::NodeType::variable;
	};
	Var(string&& n) : NodeBase(n) {
		type = NodeBase::NodeType::variable;
	};
	Var(string& n, size_t s) : NodeBase(n) {
		type = NodeBase::NodeType::variable;
		setSize(s);
	};
	static Var* Temp(TempState& state) {
		stringstream s1;
		s1 << "Temporary" << hex << state.t;
		Var *r = new Var(move(s1.str()));
		(NodeBase *)r -> setType(NodeBase::NodeType::temporary);
		state.t++;
		return r;
	};
	void setSize(size_t s) {
		m_size = s;
	};
	void addIn(Args a) {
		m_asIn.push_back(a);
	};
	void addOut(Args a) {
		m_asOut.push_back(a);
	};
};

class Func : public NodeBase {
public:
	vector<Bind> m_in;
	vector<Bind> m_out;
	Func(string& n) : NodeBase(n) {
		type = NodeBase::NodeType::function;
	};
	Func(const char *n) : NodeBase() {
		(NodeBase *)this->setName(string(n));
		type = NodeBase::NodeType::function;
	};
	Func *In(Var& v,const char * n) {
		string argName(n);
		m_in.push_back(pair(&v,argName));
		v.addIn(pair(this,argName));
		return this;
	};
	Func *Out(Var& v,const char * n) {
		string argName(n);
		m_out.push_back(pair(&v,argName));
		v.addOut(pair(this,argName));
		return this;
	};
};
map<string,NodeBase*> NodeBase::symbol;

string NodeBase::DotwithV() {
	auto f = NodeBase::Funcs();
	stringstream r;
	r << "digraph {" << endl;
	r << "\t//rankdir=LR" << endl; 
	// for(auto x : f) {
	// 	r << "\t" << x.first << " [shape=doublecircle, color=yellow]" << endl;	
	// }
	for(auto x : f) {
		Func *pf = (Func *)x.second;
		for(auto v : pf->m_in) {
			r << "\t" << v.first->name << "->" << x.first << "[label=\"" << v.second << "\"];" << endl;
		}
		for(auto v : pf->m_out) {
			r << "\t" << x.first << "->" << v.first->name << "[label=\"" << v.second << "\"];" << endl;
		}
	}
	r << "}" << endl;
	return move(r.str());
}
string NodeBase::Dot() {
	auto f = NodeBase::Funcs();
	stringstream r;
	r << "digraph {" << endl;
	r << "\t//rankdir=LR" << endl; 
	// for(auto x : f) {
	// 	r << "\t" << x.first << " [shape=doublecircle, color=yellow]" << endl;	
	// }
	for(auto x : f) {
		Func *pf = (Func *)x.second;
		for(auto v : pf->m_in) {
			string label = v.first->name;
			for(auto df : v.first->m_asOut) {
				string fn = df.first->name;
				r << "\t" << df.first->name << "->" << x.first << "[label=\"" << label << "\"];" << endl;
			}
		}
	}
	r << "}" << endl;
	return move(r.str());
}

};
};
