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

/*
class TwoPart:
    def __init__(self,A,B):
        self.A = A
        self.B = B
        self.R = []

    def do(self):
        if len(self.A)<1:
            return self
        if len(self.B)<2:
            return self
        if len(self.B)==2:
            for x in self.A:
                if x == sum(self.B):
                    self.R += [(x,self.B[0],self.B[1])]
            return self

        LA = analysis(self.A)
        LB = analysis(self.B)

        self.R += AEBE(LA['E'],LB['E']).do().result()
        self.R += AEBO(LA['E'],LB['O']).do().result()
        self.R += AOBECO(LA['O'],LB['E'],LB['O']).do().result()
        return self


class AnBp(TwoPart):  
    """
    " A -
    " B +
    """
    def __init__(self,A,B):
        TwoPart.__init__(self,[-x for x in A],B)
    def result(self):
        return [(-a,b,c) for (a,b,c) in self.R]

class ApBn(TwoPart):
    """
    " A +
    " B -
    """
    def __init__(self,A,B):
        TwoPart.__init__(self,A,[-x for x in B])
    def result(self):
        return [(a,-b,-c) for (a,b,c) in self.R]

class AEBO(TwoPart):
    """
    " A/2+1
    " (B+1)/2
    """
    def __init__(self,A,B):
        TwoPart.__init__(self,[int(x/2)+1 for x in A],[int((x+1)/2) for x in B])
    def result(self):
        return [((a-1)*2,2*b-1,2*c-1) for (a,b,c) in self.R]

class AEBE(TwoPart):
    """
    " A/2
    " B/2
    """
    def __init__(self,A,B):
        TwoPart.__init__(self,[int(x/2) for x in A],[int(x/2) for x in B])
    def result(self):
        return [(a*2,2*b,2*c) for (a,b,c) in self.R]

class ThreePart:
    """
    " A
    " B
    " C
    """
    def __init__(self,A,B,C):
        self.A = A
        self.B = B
        self.C = C
        self.R = []

    def do(self):
        LLL = len(self.A)*len(self.B)*len(self.C)
        if LLL < 8*8*8:
            if LLL == 0:
                return self
            for x in set(self.B):
                for y in set(self.C):
                    if x+y in self.A:
                        self.R += [(x+y,x,y)]
            return self

        LA = analysis(self.A)
        LB = analysis(self.B)
        LC = analysis(self.C)

        self.R += AEBECE(LA['E'],LB['E'],LC['E']).do().result()
        self.R += AEBOCO(LA['E'],LB['O'],LC['O']).do().result()
        self.R += AOBECO(LA['O'],LB['E'],LC['O']).do().result()
        self.R += AOBOCE(LA['O'],LB['E'],LC['O']).do().result()
        return self



class AEBECE(ThreePart):
    """
    " A/2
    " B/2
    " C/2
    """
    def __init__(self,A,B,C):
        ThreePart.__init__(self,[int(x/2) for x in A],[int(x/2) for x in B],[int(x/2) for x in C])
    def result(self):
        return [(a*2,2*b,2*c) for (a,b,c) in self.R]

class AEBOCO(ThreePart):
    """
    " A/2+1
    " (B+1)/2
    " (C+1)/2
    """
    def __init__(self,A,B,C):
        ThreePart.__init__(self,[int(x/2)+1 for x in A],[int((x+1)/2) for x in B],[int((x+1)/2) for x in C])
    def result(self):
        return [((a-1)*2,2*b-1,2*c-1) for (a,b,c) in self.R]

class AOBECO(ThreePart):
    """
    " (A+1)/2
    " B/2
    " (C+1)/2
    """
    def __init__(self,A,B,C):
        ThreePart.__init__(self,[int((x+1)/2) for x in A],[int(x/2) for x in B],[int((x+1)/2) for x in C])
    def result(self):
        return [(a*2-1,2*b,2*c-1) for (a,b,c) in self.R]

class AOBOCE(ThreePart):
    """
    " (A+1)/2
    " (B+1)/2
    " C/2
    """
    def __init__(self,A,B,C):
        ThreePart.__init__(self,[int((x+1)/2) for x in A],[int((x+1)/2) for x in B],[int(x/2) for x in C])
    def result(self):
        return [(a*2-1,2*b-1,2*c) for (a,b,c) in self.R]

*/