#include<vector>
#include<iostream>
#include<unordered_map>

class Solution {
public:
    int maxLength(std::vector<int>& arr) {
        //哈希表记录窗口内非重复的数字
        std::unordered_map<int, int> mp; 
        int res = 0;
        //设置窗口左右边界
        for(int left = 0, right = 0; right < arr.size(); right++){ 
            //窗口右移进入哈希表统计出现次数
            mp[arr[right]]++; 
            //出现次数大于1，则窗口内有重复
            while(mp[arr[right]] > 1) 
                //窗口左移，同时减去该数字的出现次数
                mp[arr[left++]]--; 
            //维护子数组长度最大值
            res = std::max(res, right - left + 1); 
        }
        return res;
    }
};

int main(){
    std::vector<int>vec{1, 3, 3, 5, 6, 7};
    Solution so;
    std::cout << so.maxLength(vec);
    return 0;
}