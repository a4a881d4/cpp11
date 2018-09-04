#include <iostream>
#include <list>

using namespace std;

class Node {
public:
	enum NodeType { LB, RB, DA, NA };
	NodeType t;
	int val = 0;
	string mem;
	Node(char a) {
		if(a=='(') 
			t = LB;
		else if(a==')') 
			t = RB;
		else 
			t = NA;
	};
	Node(int v) {
		t = DA;
		val = v;
	};
};

class Solution {

public:
	list<Node *> myList;
	Solution(string s) {
		toNodeList(s);
	};
	void toNodeList(string s) {
		for(char a : s) {
			Node *n = new Node(a);
			if(n->t!=Node::NA) {
				myList.push_back(n);
			}
		}
	};

	void dump() {
		for(auto it = myList.begin();it != myList.end();++it) {
			Node * h = *it;
			if(h->t==Node::LB)
				cout << '(';
			else if(h->t==Node::RB)
				cout << ')';
			else if(h->t==Node::DA)
				cout << "-" << h->val <<"\"" << h->mem << "\"-";
		}
		cout << endl;
	};
	int maxab(int a, int b) {
		if(a>b)
			return a;
		else
			return b;
	};
	int scan() {
		int change = 0;
		int max = 0;
		auto it = myList.begin();
		while(it!=myList.end()) {
			switch((*it)->t) {
				case Node::RB: {
						cout << ")" << endl;
						it++;
					}
					break;
				case Node::LB: {
						cout << "(" << endl;
						auto jt = it;
						jt++;
						if(jt!=myList.end()) {
							if((*jt)->t == Node::RB) {
								(*it)->t = Node::DA;
								(*it)->val = 2;
								max = maxab((*it)->val,max);
								(*it)->mem = "()";
								change += 2;
								myList.erase(jt);
								if(it!=myList.begin())
									it--;
								break;
							}
						}
						it++;
					}
					break;
				case Node::DA:{
						cout << "-" << (*it)->val <<"\"" << (*it)->mem << "\"-" << endl;
						if(it!=myList.begin()) {
							auto it_l = it;
							it_l--;
							auto it_n = it;
							it_n++;
							if(it_n!=myList.end()) {
								if((*it_l)->t == Node::LB && (*it_n)->t == Node::RB) {
									(*it_l)->t = Node::DA;
									(*it_l)->val = (*it)->val+2;
									max = maxab((*it)->val+2,max);
									(*it_l)->mem = "("+(*it)->mem+")";
									change += 2;
									cout << "before" << " ";
									dump();
									myList.erase(it);
									myList.erase(it_n);
									cout << "after" << " ";
									dump();
									it = it_l;
									if(it!=myList.begin())
										it--;
									break;
									//if(it_l!=myList.begin())
									//	it--;
								}
							}
						}
						auto it_n = it;
						it_n++;
						if(it_n!=myList.end()) {
							if((*it_n)->t == Node::DA) {
								(*it)->val += (*it_n)->val;
								max = maxab((*it)->val,max);
								(*it)->mem += (*it_n)->mem;
								change += 2;
								myList.erase(it_n);
								break;
							}
						}
						it++;
					}
					break;
				default:
					break;
			}
			dump();
		}
		return max;
	}

    int longestValidParentheses(string s) {
    	toNodeList(s);
    	return scan();
    }
};

int main(int argc, char *argv[]) {
	string ttu(argv[1]);
	Solution a("");
	a.dump();
	auto m = a.longestValidParentheses(ttu);
	cout << "max match " << m << endl;
}