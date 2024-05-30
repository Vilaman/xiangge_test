#include<iostream>
#include<vector>
#include<algorithm>

class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 旋转数组
     * @param n int整型 数组长度
     * @param m int整型 右移距离
     * @param a int整型vector 给定数组
     * @return int整型vector
     */
    std::vector<int> solve(int n, int m, std::vector<int>& a) {
        // write code here
        //取余，因为每次长度为n的旋转数组相当于没有变化
        m = m % n;
        //第一次逆转全部数组元素
        std::reverse(a.begin(), a.end());       //654321
        //第二次只逆转开头m个
        std::reverse(a.begin(), a.begin() + m); //564321
        //第三次只逆转结尾m个
        std::reverse(a.begin() + m, a.end());   //561234
        return a;
    }
};

int main(){
    int n = 6;
    int m = 2;
    std::vector<int>a{1, 2, 3, 4, 5, 6};
    Solution sol;
    sol.solve(n, m, a);
    for(int i = 0; i < a.size(); i++){
        std::cout << a[i];
    }
    std::cout << std::endl;
    return 0;
}