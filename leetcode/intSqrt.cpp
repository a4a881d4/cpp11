#include <iostream>
using namespace std;
const int tab[9] = {0,1,1,1,2,2,2,2,2};
    int mySqrt(int x) {
        if(x<9) return tab[x];
        
        long y = x;
        long down = 1;
        long up = 1;
        while(x>0) {
            x >>=2;
            up<<=1;
        }
        while(up-down>1) {
            down = (y+up*down)/(up+down);
            up = (y+down*down+down)/(2*down);
            cout << " " << up << " " << down << endl;
        }
        return down;
    }
    
void test(int a) {
    int r = mySqrt(a);
    cout << r << " " << a-r*r << " " << a << " " << a-(r+1)*(r+1) << endl;
}
int main() {

    int a[] = {10,442258756,2147395599};
    for(int i=0;i<3;i++)
        test(a[i]);
}