#include <iostream>
#include <vector>

// 函数用于找出立方数可以表示的连续奇数
std::vector<int> nicomachusTheorem(int m) {
    int start = m * m - m + 1; // 计算连续奇数的起始点
    std::vector<int> oddNumbers;
    
    // 将 m 个连续奇数添加到数组中
    for (int i = 0; i < m; ++i) {
        oddNumbers.push_back(start + i * 2);
    }
    
    return oddNumbers;
}

// 主函数
int main() {
    int m;
    std::cout << "请输入一个整数m，以计算其立方可由连续奇数相加而成: ";
    std::cin >> m;

    std::vector<int> oddNumbers = nicomachusTheorem(m);
    int sum = 0;

    std::cout << m << "的立方可由以下连续奇数相加而成: ";
    for (int num : oddNumbers) {
        std::cout << num << " ";
        sum += num;
    }
    std::cout << std::endl << "验证: " << m << "^3 = " << m * m * m << " = " 
              << sum << std::endl;

    return 0;
}