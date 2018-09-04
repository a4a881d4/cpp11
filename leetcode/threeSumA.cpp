#include <set>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class Solution {
    class TwoPart {
    private:
        vector<int>* m_A;
        vector<int>* m_B;
    public:
        vector<vector<int> > m_R;
        
        TwoPart(){};

        TwoPart & init(vector<int>& A, vector<int>& B) {
            m_A = &A;
            m_B = &B;
            return *this;
        };
        
        typedef pair<int,int> BC;
        typedef map<int,set<BC> > BAC;
        TwoPart & done() {
            if(m_A->size()<1 || m_B->size()<2)
                return *this;
            BAC bc;
            int maxA = *(m_A->end()-1);
            for( auto it = m_B->begin(); it != m_B->end()-1; ++it ) {
                for( auto jt = it+1; jt != m_B->end(); ++jt ) {
                    int s = *it + *jt;
                    if( s>maxA )
                        break;
                    auto kt = bc.find(s);
                    if( kt==bc.end() ) {
                        bc[s] = set<BC>();
                        bc[s].insert(BC(*it,*jt));
                    } else {
                        kt->second.insert(BC(*it,*jt));
                    }
                }
            }
            for( auto x : *m_A ) {
                auto kt = bc.find(x);
                if(kt != bc.end()) {
                    for( auto y : kt->second ) {
                        m_R.push_back(vector<int>{x,y.first,y.second});
                    }
                }
            }
            return *this;
        };
        
        virtual vector<vector<int> >& result() {
            return m_R;
        }; 
    };

    class AnBp : public TwoPart {
    public:
        AnBp(){};
        AnBp& init(vector<int>& A, vector<int>& B) {
            for(int& i : A) 
                i = -i;
            sort(A.begin(),A.end());
            TwoPart::init(A,B);
            return *this;
        };
        vector<vector<int> >& result() {
            for(auto& x : m_R) 
                x[0] = -x[0];
            return m_R;
        };
    };

    class ApBn : public TwoPart {
    public:
        ApBn(){};
        ApBn& init(vector<int>& A, vector<int>& B) {
            for(int& i : B) 
                i = -i;
            sort(B.begin(),B.end());
            // cout << "ApBn:" << endl;
            // Solution::_dump(A);
            // Solution::_dump(B);
            TwoPart::init(A,B);
            return *this;
        };
        vector<vector<int> >& result() {
            for(auto& x : m_R) {
                x[1] = -x[1];
                x[2] = -x[2];
            }
            return m_R;
        };
    };

    class preProcessor {
    private:
        set<int> m_key;
        set<int> contain_zero;
        set<int> dup;
        set<int> negative;
        set<int> positive;
        vector<vector<int> > m_R;
    public:
        set<int>& getkey() {return m_key;};
        set<int>& getP() {return positive;};
        set<int>& getN() {return negative;};
        
        vector<vector<int> >& result() {
            for(auto x : dup) {
                m_R.push_back(vector<int>{-2*x,x,x});
            }
            for(auto x : contain_zero) {
                m_R.push_back(vector<int>{-x,0,x});
            }
            return m_R;
        };
        preProcessor(vector<int>& nums) {
            for( auto x : nums ) 
                m_key.insert(x);
            int zeros = count(nums.begin(),nums.end(),0);
            if( zeros>2 )
                contain_zero.insert(0);
            m_key.erase(0);
            for( int x : m_key ) {
                if(x < 0) {
                    negative.insert(x);
                }
                if(x > 0) {
                    positive.insert(x);
                    if( zeros>0 )
                        if(negative.count(-x)>0)
                            contain_zero.insert(x);
                }
                if(count(nums.begin(),nums.end(),x)>1 && count(nums.begin(),nums.end(),-2*x)>0) {
                    dup.insert(x);
                }
            }
        };
        void dump() {
            cout << "m_key" << endl;
            Solution::_dump<set<int>>(m_key);
            cout << "contain_zero" << endl;
            Solution::_dump<set<int>>(contain_zero);
            cout << "dup" << endl;
            Solution::_dump<set<int>>(dup);
            cout << "negative" << endl;
            Solution::_dump<set<int>>(negative);
            cout << "positive" << endl;
            Solution::_dump<set<int>>(positive);
            Solution::dumpResult(m_R);                            
        };

    };
public:
    template <typename T>
    static void _dump(T& x) {
        for(auto y : x) {
            cout << y << " ";
        }
        cout << endl;
    };
    
    static void dumpResult(vector<vector<int> >& x) {
        for(auto y : x) {
            Solution::_dump(y);
            cout << endl;
        }
        cout << endl;
    };

    vector<int>& fconv(vector<int>& x) {
        sort(x.begin(),x.end()); 
        return x;
    };
    
    vector<vector<int> >& threeSum(vector<int>& nums) {
        AnBp anbp;
        ApBn apbn;
        vector<vector<int> >* r = new vector<vector<int> >;
        preProcessor pre(nums);
        auto n = pre.getN();
        auto p = pre.getP();
        
        for(auto x : pre.result()) {
            r->push_back(fconv(x));
        }
        // pre.dump();
        vector<int> anbpA(n.begin(), n.end());
        vector<int> anbpB(p.begin(), p.end());
        for(auto x : anbp
            .init(anbpA,anbpB)
            .done()
            .result()) 
        {
            r->push_back(fconv(x));
        }
        vector<int> apbnA(n.begin(), n.end());
        vector<int> apbnB(p.begin(), p.end());
        for(auto x : apbn
            .init(apbnB, apbnA)
            .done()
            .result()) 
        {
            r->push_back(fconv(x));
        }
        sort(r->begin(),r->end());
        return *r;    
    }
};

//vector<int> vl{0,0,0};
extern vector<int> vl;
// int l[6] = {3,0,-2,-1,1,2};
int main() {
    // vector<int> vl(&l[0],&l[0]+sizeof(l)/sizeof(int));
    // for(auto i : vl) {
    //     cout << i << endl;  
    // }
    Solution a;
    vector<vector<int> >& r = a.threeSum(vl);
    for(auto x : r) {
        cout << "[";
        for(auto y:x) {
            cout << y << ", ";
        }
        cout << "]" << endl;
    }
    cout << "total: " << r.size() << endl;
    delete &r;
}


/*
class Solution {
    class TwoPart {
    private:
        vector<int>* m_A;
        vector<int>* m_B;
    public:
        vector<vector<int> > m_R;
        
        TwoPart(){};

        TwoPart & init(vector<int>& A, vector<int>& B) {
            m_A = &A;
            m_B = &B;
            return *this;
        };
        
        typedef pair<int,int> BC;
        typedef map<int,set<BC> > BAC;
        TwoPart & done() {
            if(m_A->size()<1 || m_B->size()<2)
                return *this;
            BAC bc;
            int maxA = *(m_A->end()-1);
            for( auto it = m_B->begin(); it != m_B->end()-1; ++it ) {
                for( auto jt = it+1; jt != m_B->end(); ++jt ) {
                    int s = *it + *jt;
                    if( s>maxA )
                        break;
                    auto kt = bc.find(s);
                    if( kt==bc.end() ) {
                        bc[s] = set<BC>();
                        bc[s].insert(BC(*it,*jt));
                    } else {
                        kt->second.insert(BC(*it,*jt));
                    }
                }
            }
            for( auto x : *m_A ) {
                auto kt = bc.find(x);
                if(kt != bc.end()) {
                    for( auto y : kt->second ) {
                        m_R.push_back(vector<int>{x,y.first,y.second});
                    }
                }
            }
            return *this;
        };
        
        virtual vector<vector<int> >& result() {
            return m_R;
        }; 
    };

    class AnBp : public TwoPart {
    public:
        AnBp(){};
        AnBp& init(vector<int>& A, vector<int>& B) {
            for(int& i : A) 
                i = -i;
            sort(A.begin(),A.end());
            TwoPart::init(A,B);
            return *this;
        };
        vector<vector<int> >& result() {
            for(auto& x : m_R) 
                x[0] = -x[0];
            return m_R;
        };
    };

    class ApBn : public TwoPart {
    public:
        ApBn(){};
        ApBn& init(vector<int>& A, vector<int>& B) {
            for(int& i : B) 
                i = -i;
            sort(B.begin(),B.end());
            // cout << "ApBn:" << endl;
            // Solution::_dump(A);
            // Solution::_dump(B);
            TwoPart::init(A,B);
            return *this;
        };
        vector<vector<int> >& result() {
            for(auto& x : m_R) {
                x[1] = -x[1];
                x[2] = -x[2];
            }
            return m_R;
        };
    };

    class preProcessor {
    private:
        set<int> m_key;
        set<int> contain_zero;
        set<int> dup;
        set<int> negative;
        set<int> positive;
        vector<vector<int> > m_R;
    public:
        set<int>& getkey() {return m_key;};
        set<int>& getP() {return positive;};
        set<int>& getN() {return negative;};
        
        vector<vector<int> >& result() {
            for(auto x : dup) {
                m_R.push_back(vector<int>{-2*x,x,x});
            }
            for(auto x : contain_zero) {
                m_R.push_back(vector<int>{-x,0,x});
            }
            return m_R;
        };
        preProcessor(vector<int>& nums) {
            for( auto x : nums ) 
                m_key.insert(x);
            int zeros = count(nums.begin(),nums.end(),0);
            if( zeros>2 )
                contain_zero.insert(0);
            m_key.erase(0);
            for( int x : m_key ) {
                if(x < 0) {
                    negative.insert(x);
                }
                if(x > 0) {
                    positive.insert(x);
                    if( zeros>0 )
                        if(negative.count(-x)>0)
                            contain_zero.insert(x);
                }
                if(count(nums.begin(),nums.end(),x)>1 && count(nums.begin(),nums.end(),-2*x)>0) {
                    dup.insert(x);
                }
            }
        };
        void dump() {
            cout << "m_key" << endl;
            Solution::_dump<set<int>>(m_key);
            cout << "contain_zero" << endl;
            Solution::_dump<set<int>>(contain_zero);
            cout << "dup" << endl;
            Solution::_dump<set<int>>(dup);
            cout << "negative" << endl;
            Solution::_dump<set<int>>(negative);
            cout << "positive" << endl;
            Solution::_dump<set<int>>(positive);
            Solution::dumpResult(m_R);                            
        };

    };
public:
    template <typename T>
    static void _dump(T& x) {
        for(auto y : x) {
            cout << y << " ";
        }
        cout << endl;
    };
    
    static void dumpResult(vector<vector<int> >& x) {
        for(auto y : x) {
            Solution::_dump(y);
            cout << endl;
        }
        cout << endl;
    };

    vector<int>& fconv(vector<int>& x) {
        sort(x.begin(),x.end()); 
        return x;
    };
    
    vector<vector<int> >& threeSum(vector<int>& nums) {
        AnBp anbp;
        ApBn apbn;
        vector<vector<int> >* r = new vector<vector<int> >;
        if(nums.size()<3)
            return *r;
        preProcessor pre(nums);
        auto n = pre.getN();
        auto p = pre.getP();
        
        for(auto x : pre.result()) {
            r->push_back(fconv(x));
        }
        // pre.dump();
        vector<int> anbpA(n.begin(), n.end());
        vector<int> anbpB(p.begin(), p.end());
        for(auto x : anbp
            .init(anbpA,anbpB)
            .done()
            .result()) 
        {
            r->push_back(fconv(x));
        }
        vector<int> apbnA(n.begin(), n.end());
        vector<int> apbnB(p.begin(), p.end());
        for(auto x : apbn
            .init(apbnB, apbnA)
            .done()
            .result()) 
        {
            r->push_back(fconv(x));
        }
        sort(r->begin(),r->end());
        return *r;    
    }
};
*/