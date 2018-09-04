#include <iostream>
#include <stack>
#include <vector>

using namespace std;

class Solution {
public:
    
    int evalRPN(vector<string>& tokens) {
        stack<int> s;
        int i,j;
        for(auto a : tokens) {
            switch(a[0]) {
                case '+': {
                        i = s.top();s.pop();
                        j = s.top();s.pop();
                        s.push(i+j);
                    }
                    break;
                case '*': {
                    i = s.top();s.pop();
                    j = s.top();s.pop();
                    s.push(j*i);
                }
                    break;
                case '/': {
                    i = s.top();s.pop();
                    j = s.top();s.pop();
                    s.push(j/i);
                }
                    break;
                case '-': 
                    if(a.size()==1) {
                        i = s.top();s.pop();
                        j = s.top();s.pop();
                        s.push(j-i);
                        break;
                    }
                default:
                    s.push(stoi(a));
                    break;
            }
        }
        return s.top();
    }
};

int main() {
    vector<string> t{"10","6","9","3","+","-11","*","/","*","17","+","5","+"};
    Solution a;
    int r = a.evalRPN(t);
    cout << r << endl;
}