#include <iostream>
#include <string.h>

using namespace std;
int lengthOfLongestSubstring(string s) {
        char buf[s.size()+1];
        int cc[26];
        auto dump = [](int *cc) { for(auto k=0;k<26;k++) cout << k <<"-" << cc[k] << " "; cout << endl;};
        memset(cc,0,sizeof(int)*26);
        char *ss = buf;
        char *bestS = ss;
        char *bestE = ss;
        char *TS = ss;
        char *TE = ss;
        int bestL = 0;
        for(int i=0;i<s.size();i++) {
            *ss = s[i];
            int is = (int)(*ss-'a');
            cc[is]++;
            if(cc[is]==2) {
                dump(cc);
                TE = ss;
                if(TE-TS>bestL) {
                    bestS = TS;
                    bestE = TE;
                    bestL = TE-TS;
                }
                while(cc[is]!=1) {
                    dump(cc);
                    cout << *TS;
                    cc[(int)(*TS-'a')]--;
                    TS++;
                }
            }
            ss++;
            cout << "TS: " << TS-buf << " TE: " << TE-buf << endl;
            cout << "BS: " << bestS-buf << " BE: " << bestE-buf << " BL: " << bestL << endl;
        }
        while(bestS!=bestE) {
            cout << *bestS;
            bestS++;
        }
        return bestL;
    }

int main() {
    cout << lengthOfLongestSubstring("abcabcbb") << endl;
}
