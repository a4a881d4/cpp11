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
	Node *reverse(Node *n) {
		int l = len(n);
		if(l<2) return n;
		Node *h = n;
		Node *Me = h;
		Node *Next = Me->next;
		while(Next!=nullptr) {
			Node *t = Next;
			Next = Next->next;
			t->next = Me;
			Me = t;
		}
		h->next = nullptr;
		return Me;
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
		Node *re = a.reverse(head);
		dump(re);
	}
}
