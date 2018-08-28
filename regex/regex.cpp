#include <regex>
#include <string>
#include <iostream>
using namespace std;

regex expression("^select ([a-zA-Z]*) from ([a-zA-Z]*)");  

int main(int argc, char* argv[]) {  
	cmatch what;  
	string in = "select ABC from abcABC";

	if(regex_match(in.c_str(), what, expression)){  
		for(int i=0;i<what.size();i++)  
			cout<<"str :"<<what[i].str()<<endl;
	}
	else {
		cout<<"Error Input"<<endl;
	}
	return 0;
}