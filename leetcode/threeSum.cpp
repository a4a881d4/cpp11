#include <iostream>
#include <set>
#include <map>
#include <vector>

using namespace std;
vector<vector<int> > threeSum(vector<int>& nums) {
	typedef pair<int,int> BC;
	map<int,set<BC> > bc;
	sort(nums.begin(),nums.end());
	for(auto it = nums.begin();it != nums.end()-1;++it) {
		for(auto jt = it+1;jt != nums.end();++jt) {
			auto s = *it+*jt;
			auto kt = bc.find(-s);
			if(kt == bc.end()) {
				bc[-s] = set<BC>();
				bc[-s].insert(BC(*it,*jt));
			} else {
				kt->second.insert(BC(*it,*jt));
			}
		}
	}
	set<int> sn(nums.begin(),nums.end());
	set<int> sbc;
	for(auto it=bc.begin();it!=bc.end();++it) {
		sbc.insert(it->first);
	}
	vector<int> vs(sn.size());
	set_intersection(sn.begin(),sn.end(),sbc.begin(),sbc.end(),inserter(vs,vs.begin()));
	set<int> is;
	for(auto it=vs.begin();it!=vs.end();++it) {
		cout << "insert " << *it << "@" << is.size() << endl;
		is.insert(*it);
	}
	vector<vector<int> > r;
	cout << " Is size " << is.size() << endl;
	for( auto x : is ) {
		auto kt = bc.find(x);
		if(kt == bc.end()) {
			cout << "some thing error " << x << endl;
		} else {
			for( auto y : kt->second ) {
				vector<int> a(3);
				if( x<=y.first ) {
					a[0] = x;
					a[1] = y.first;
					a[2] = y.second;
				} 
				r.push_back(a);
			}
		}
	}
	return r;
}

int l[6] = {3,0,-2,-1,1,2};
int main() {
	vector<int> vl(&l[0],&l[0]+sizeof(l)/sizeof(int));
	for(auto i : vl) {
		cout << i << endl;	
	}
	auto r = threeSum(vl);
	for(auto x : r) {
		cout << "[";
		for(auto y:x) {
			cout << y << ", ";
		}
		cout << "]" << endl;
	}
	
}