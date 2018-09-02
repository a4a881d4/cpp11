#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums, int t) {
        vector<vector<int>> res;
        sort(nums.begin(), nums.end());
        if (!nums.empty() && nums.back() < 0) return {};
        for (int k = 0; k < nums.size(); ++k) {
            if (nums[k] > 0) break;
            if (k > 0 && nums[k] == nums[k - 1]) continue;
            int target = t - nums[k];
            int i = k + 1, j = nums.size() - 1;
            while (i < j) {
                if (nums[i] + nums[j] == target) {
                    res.push_back({nums[k], nums[i], nums[j]});
                    while (i < j && nums[i] == nums[i + 1]) ++i;
                    while (i < j && nums[j] == nums[j - 1]) --j;
                    ++i; --j;
                } else if (nums[i] + nums[j] < target) ++i;
                else --j;
            }
        }
        return res;
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
    vector<vector<int> > r = a.threeSum(vl,0);
    for(auto x : r) {
        cout << "[";
        for(auto y:x) {
            cout << y << ", ";
        }
        cout << "]" << endl;
    }
    cout << "total: " << r.size() << endl;
}

