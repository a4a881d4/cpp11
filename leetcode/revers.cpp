#include <iostream>

using namespace std;

class Node {
public:
	Node *next;
	int val;
	Node(int v) : val(v), next(nullptr){};
};

class Solution {
public:
	int len(Node *n) {
		int r = 0;
		while(n != nullptr) {
			++r;
			n = n->next;
		}
		return r;
	};
	pair<Node *,Node *> runK(Node *n, int k) {
		Node *r = n;
		while(r->next!=nullptr && k>0) {
			k--;
			r = n;
			n = n->next;
		}
		if(k!=0)
			cout << "Node Length < run length" << endl;
		return pair<Node *,Node *>(r,n);
	}
	pair<Node *,Node *> reverse(Node *n,int k,int m) {
		int l = len(n);
		if(l<2) return pair<Node*,Node*>(n,nullptr);
		Node *h = n;
		Node *Me = h;
		auto Next = runK(Me,k);
		while(Next.second!=nullptr && m>0) {
			auto t = Next;
			Next = runK(Next.second,k);
			t.second->next = Me;
			Me = t.first;
			m--;
		}
		h->next = Next.second;
		return pair<Node *, Node *>(Me,Next.second);
	};
};

Node *newL(int l) {
	if(l==0) return nullptr;
	Node *head = new Node(0);
	Node *it = head;
	for(int i=1;i<l;i++) {
		it->next = new Node(i);
		it = it->next; 
	}
	return head;
}

void dump(Node *h) {
	while(h != nullptr) {
		cout << h->val << ",";
		h = h->next;
	}
	cout << endl;
}

int main() {
	for(int l=0;l<20;l++) {
		Node *head = newL(l);
		dump(head);
		Solution a;
		int k = 1;
		int n = l/k;
		auto re = a.reverse(head,k,n);
		dump(re.first);
		dump(re.second);
	}
}
