#include <iostream>
#include <vector>
#include <algorithm>

class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 计算成功举办活动需要多少名主持人
     * @param n int整型 有n个活动
     * @param startEnd int整型vector<vector<>> startEnd[i][0]用于表示第i个活动的开始时间，startEnd[i][1]表示第i个活动的结束时间
     * @return int整型
     */
    int minmumNumberOfHost(int n, std::vector<std::vector<int> >& startEnd) {
        // write code here
        std::vector<int>start;
        std::vector<int>end;
        for(int i = 0; i < n; i++){
            start.push_back(startEnd[i][0]);
            end.push_back(startEnd[i][1]);
        }
        sort(start.begin(), start.end());
        sort(end.begin(), end.end());
        int res = 0;
        int j = 0;
        for(int i = 0; i < n; i++){
            //新开始的节目大于上一轮结束的时间，主持人不变
            if(start[i] >= end[j]) 
                j++;  
            else
                //主持人增加
                res++;
        }
        return res;
    }
};

int main() {
	std::vector<std::vector<int> >startEnd{ {1, 2}, {2, 4} };
	int n = 2;
	Solution so;
	std::cout << so.minmumNumberOfHost(n, startEnd);
	return 0;
}