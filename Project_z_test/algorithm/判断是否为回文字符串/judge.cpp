#include<iostream>
#include<string>

bool judge(std::string str){
    int right = str.length()-1;
    int left = 0;
    while (right > left){
        if(str[right]!=str[left]){
            return false;
        }
        right--;
        left++;
    }
    return true;
}

int main(){
    std::string str = "a";
    std:: cout << judge(str);
}