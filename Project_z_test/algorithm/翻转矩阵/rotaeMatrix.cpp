#include<iostream>
#include<vector>

class Solution {
  public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     *
     * @param mat int整型vector<vector<>>
     * @param n int整型
     * @return int整型vector<vector<>>
     */
    std::vector<std::vector<int> > rotateMatrix(std::vector<std::vector<int> >& mat, int n) {
        // write code here
        int i, j, k = n - 1, temp;
        for (i = 0; i < n / 2; i++) {
            for (j = 0; j < n; j++) {
                temp = mat[i][j];
                mat[i][j] = mat[k][j];
                mat[k][j] = temp;
            }
            k--;
        }
        for (i = 0; i < n; i++) {
            for (j = i; j < n; j++) {
                std::swap(mat[i][j], mat[j][i]);
            }
        }
        return mat;
    }
};

int main(){
    std::vector<std::vector<int>>mat{{1,2,3},{4,5,6},{7,8,9}};
    int n = 3;
    Solution a;
    a.rotateMatrix(mat,n);
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            std::cout<<mat[i][j];
        }
        std::cout << std::endl;
    }
    
    return 0;
}