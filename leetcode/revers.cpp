#include <iostream>

using namespace std;

class ListNode {
public:
	ListNode *next;
	int val;
	ListNode(int v) : val(v), next(nullptr){};
};

void dump(ListNode *h,int n=9) {
	while(h != nullptr && n>0) {
		cout << h->val << ",";
		h = h->next;
		n--;
	}
	cout << endl;
}

class Solution {
public:
	int len(ListNode *n) {
		int r = 0;
		while(n != nullptr) {
			++r;
			n = n->next;
		}
		return r;
	};
	pair<ListNode *,ListNode *> runK(ListNode *n, int k) {
		ListNode *r = n;
		while(r->next!=nullptr && k>0) {
			k--;
			r = n;
			n = n->next;
		}
		if(k!=0)
			cout << "ListNode Length < run length" << endl;
		return pair<ListNode *,ListNode *>(r,n);
	}
	ListNode * reverse(ListNode *n,int m) {
		m--;
		int l = len(n);
		if(l<2) return n;
		ListNode *h = n;
		ListNode *Me = h;
		auto Next = Me->next;
		while(Next!=nullptr && m>0) {
			// cout << Me->val << " " << Next->val << endl;
			auto t = Next;
			Next = Next->next;
			t->next = Me;
			Me = t;
			m--;
		}
		h->next = Next;
		return Me;
	};

	ListNode * reverseN(ListNode *n,int k,int m) {
		int l = len(n);
		if(l<2) return n;
		m--;
		ListNode *h = n;
		ListNode *Me = h;
		auto Next = runK(Me,k);
		ListNode *end = Next.first;
		while(Next.second!=nullptr && m>0) {
			// cout << Me->val << " " << Next.first->val << " " << Next.second->val << endl;
			auto t = Next;
			Next = runK(Next.second,k);
			Next.first->next = Me;
			Me = t.second;
			end->next = Next.second;
			m--;
			// dump(Me,20);
		}
		return Me;
	};
	ListNode* reverseKGroup(ListNode* head, int k) {
        int nl = len(head);
		int n = nl/k;
		auto re1 = reverse(head,nl-nl%k);
		auto re = reverseN(re1,k,n);
		return re;
    };
};

ListNode *newL(int l) {
	if(l==0) return nullptr;
	ListNode *head = new ListNode(0);
	ListNode *it = head;
	for(int i=1;i<l;i++) {
		it->next = new ListNode(i);
		it = it->next; 
	}
	return head;
}



int main() {
	for(int l=0;l<20;l++) {
		cout << l << "th" << endl;
		ListNode *head = newL(l);
		dump(head,20);
		Solution a;
		auto re = a.reverseKGroup(head,3);
		dump(re,20);
	}
}
