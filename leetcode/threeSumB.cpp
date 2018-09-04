#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

class Solution {
private:
    static pair<vector<int>,vector<int> > analysis(vector<int>& a) {
        pair<vector<int>,vector<int> > r;
        for(auto x : a) {
            if(x&1)
                r.second.push_back(x);
            else
                r.first.push_back(x);
        }
        return r;
    };
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
        
        TwoPart & done() {
            if(m_A->size()<1 || m_B->size()<2)
                return *this;
            if(m_B->size() == 2) {
                for(auto x : *m_A) {
                    if(x == (*m_B)[0]+(*m_B)[1])
                        m_R.push_back({x,(*m_B)[0],(*m_B)[1]});
                }
                return *this;
            }

            auto LA = Solution::analysis(*m_A);
            auto LB = Solution::analysis(*m_B);
            
            AEBE aebe;
            vector<int> LAE0(LA.first);
            vector<int> LBE0(LB.first);
            for(auto x : aebe
                .init(LAE0,LBE0)
                .done()
                .result()
                )
                m_R.push_back(x);
            AEBO aebo;
            vector<int> LAE1(LA.first);
            vector<int> LBO1(LB.second);
            for(auto x : aebo
                .init(LAE1,LBO1)
                .done()
                .result()
                )
                m_R.push_back(x);
            AOBECO aobeco;
            vector<int> LAO2(LA.second);
            vector<int> LBE2(LB.first);
            vector<int> LBO2(LB.second);
            for(auto x : aobeco
                .init(LAO2,LBE2,LBO2)
                .done()
                .result()
                )
                m_R.push_back(x);
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

    class AEBE : public TwoPart {
    public:
        AEBE(){};
        AEBE& init(vector<int>& A, vector<int>& B) {
            for(int& i : A) 
                i = i/2;
            for(int& i : B) 
                i = i/2;
            TwoPart::init(A,B);
            return *this;
        };
        vector<vector<int> >& result() {
            for(auto& x : m_R) {
                x[0] *= 2;
                x[1] *= 2;
                x[2] *= 2;
            }
            return m_R;
        };
    };
    class AEBO : public TwoPart {
    public:
        AEBO(){};
        AEBO& init(vector<int>& A, vector<int>& B) {
            for(int& i : A) 
                i = i/2+1;
            for(int& i : B) 
                i = (i+1)/2;
            TwoPart::init(A,B);
            return *this;
        };
        vector<vector<int> >& result() {
            for(auto& x : m_R) {
                x[0] = (x[0]-1)*2;
                x[1] = x[1]*2-1;
                x[2] = x[2]*2-1;
            }
            return m_R;
        };
    };

    class ThreePart {
    private:
        vector<int>* m_A;
        vector<int>* m_B;
        vector<int>* m_C;
    public:
        vector<vector<int> > m_R;
        
        ThreePart(){};

        ThreePart & init(vector<int>& A, vector<int>& B,  vector<int>& C) {
            m_A = &A;
            m_B = &B;
            m_C = &C;
            return *this;
        };

        ThreePart & done() {
            int len = m_A->size()*m_B->size()*m_C->size();
            if( len<1 ) {
                return *this;
            }
            if( len==1 ) {
                if((*m_A)[0] == (*m_B)[0]+(*m_C)[0]) {
                    m_R.push_back({(*m_A)[0],(*m_B)[0],(*m_C)[0]});
                    return *this;
                }
                else
                    return *this;
            }
            
            auto LA = Solution::analysis(*m_A);
            auto LB = Solution::analysis(*m_B);
            auto LC = Solution::analysis(*m_C);
            
            AEBECE aebece;
            vector<int> LAE0(LA.first);
            vector<int> LBE0(LB.first);
            vector<int> LCE0(LC.first);
            for(auto x : aebece
                .init(LAE0,LBE0,LCE0)
                .done()
                .result()
                )
                m_R.push_back(x);
            AEBOCO aeboco;
            vector<int> LAE1(LA.first);
            vector<int> LBO1(LB.second);
            vector<int> LCO1(LC.second);
            for(auto x : aeboco
                .init(LAE1,LBO1,LCO1)
                .done()
                .result()
                )
                m_R.push_back(x);
            AOBECO aobeco;
            vector<int> LAO2(LA.second);
            vector<int> LBE2(LB.first);
            vector<int> LCO2(LC.second);
            for(auto x : aobeco
                .init(LAO2,LBE2,LCO2)
                .done()
                .result()
                )
                m_R.push_back(x);
            AOBOCE aoboce;
            vector<int> LAO3(LA.second);
            vector<int> LBO3(LB.second);
            vector<int> LCE3(LC.first);
            for(auto x : aoboce
                .init(LAO3,LBO3,LCE3)
                .done()
                .result()
                )
                m_R.push_back(x);
            return *this;
        };
        virtual vector<vector<int> >& result() {
            return m_R;
        }; 
    };
    class AEBECE : public ThreePart {
    public:
        AEBECE(){};
        AEBECE & init(vector<int>& A,vector<int>& B,vector<int>& C) {
            for(int& i : A) 
                i = i/2;
            for(int& i : B) 
                i = i/2;
            for(int& i : C) 
                i = i/2;
            ThreePart::init(A,B,C);
            return *this;
        };
        vector<vector<int> >& result() {
            for(auto& x : m_R) {
                x[0] *= 2;
                x[1] *= 2;
                x[2] *= 2;
            }
            return m_R;
        };
    };
    
    class AEBOCO : public ThreePart {
    public:
        AEBOCO(){};
        AEBOCO & init(vector<int>& A,vector<int>& B,vector<int>& C) {
            for(int& i : A) 
                i = i/2+1;
            for(int& i : B) 
                i = (i+1)/2;
            for(int& i : C) 
                i = (i+1)/2;
            ThreePart::init(A,B,C);
            return *this;
        };
        vector<vector<int> >& result() {
            for(auto& x : m_R) {
                x[0] = (x[0]-1)*2;
                x[1] = x[1]*2-1;
                x[2] = x[2]*2-1;
            }
            return m_R;
        };
    };

    class AOBECO : public ThreePart {
    public:
        AOBECO(){};
        AOBECO & init(vector<int>& A,vector<int>& B,vector<int>& C) {
            for(int& i : A) 
                i = (i+1)/2;
            for(int& i : B) 
                i = i/2;
            for(int& i : C) 
                i = (i+1)/2;
            ThreePart::init(A,B,C);
            return *this;
        };
        vector<vector<int> >& result() {
            for(auto& x : m_R) {
                x[0] = x[0]*2-1;
                x[1] = x[1]*2;
                x[2] = x[2]*2-1;
            }
            return m_R;
        };
    };

    class AOBOCE : public ThreePart {
    public:
        AOBOCE(){};
        AOBOCE & init(vector<int>& A,vector<int>& B,vector<int>& C) {
            for(int& i : A) 
                i = (i+1)/2;
            for(int& i : B) 
                i = (i+1)/2;
            for(int& i : C) 
                i = i/2;
            ThreePart::init(A,B,C);
            return *this;
        };
        vector<vector<int> >& result() {
            for(auto& x : m_R) {
                x[0] = x[0]*2-1;
                x[1] = x[1]*2-1;
                x[2] = x[2]*2;
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
    };
public:
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

//vector<int> vl{3,0,-2,-1,1,2};
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
