#include <iostream>
#include <vector>

int maxArea(std::vector<int>& height){
    //排除不能形成容器的情况
    if(height.size() < 2) 
        return 0;
    int res = 0; 
    //双指针左右界
    int left = 0; 
    int right = height.size() - 1;
    //共同遍历完所有的数组
    while(left < right){ 
        //计算区域水容量
        int capacity = std::min(height[left], height[right]) * (right - left);
        //维护最大值 
        res = std::max(res, capacity); 
        //优先舍弃较短的边
        if(height[left] < height[right]) 
            left++;
        else
            right--;
    }
    return res;
}

int main(){
    std::vector<int>vec{ 1, 7, 3, 2, 4, 5, 8, 2, 7 };
    std::cout << maxArea(vec);
    return 0;
}

