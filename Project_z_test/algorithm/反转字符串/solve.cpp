#include <algorithm>
#include <string>
#include <iostream>
class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 反转字符串
     * @param str string字符串 
     * @return string字符串
     */
    std::string solve(std::string str) {
        // write code here
        std::reverse(str.begin(), str.end());
        return str;
    }
};

int main(){
    std::string str = "abcde";
    Solution so;
    std::cout << so.solve(str);
}