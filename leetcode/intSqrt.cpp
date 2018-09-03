#include <iostream>
using namespace std;

    int mySqrt(int x) {
        if(x==0) return 0;
        if(x==1) return 1;
        int ss = 0;
        long y = x;
        while(y!=0) {
            ss++;
            y>>=1;
        }
        long up = 1<<((ss+2)/2);
        long down = 1;
        y = x;
        while(up-down>1) {
            long mid;
            if(up-down>ss)
                mid = (y+up*down)/(up+down)+ss/2;
            else
                mid = (up+down)/2;
            cout << " " << mid 
                 <<  " " << up 
                 <<  " " << down 
                << endl;
            long m2 = mid*mid;
            if(m2==y)
                return mid;
            else if(m2>y)
                up = mid;
            else
                down = mid;
        }
        return down;
    }
    

int main() {
    int a = 10;//442258756;//2147395599;
    int r = mySqrt(a);
    cout << r << " " << a-r*r << " " << a << " " << a-(r+1)*(r+1) << endl;
}