#ifndef CLAIM_DATA_H_
#define CLAIM_DATA_H_

//定义快递柜的四个盒子的名称
#define Claim_Box1 "Claim_Box1"
#define Claim_Box2 "Claim_Box2"
#define Claim_Box3 "Claim_Box3"
#define Claim_Box4 "Claim_Box4"

//定义快递柜的四个盒子的结构体
typedef struct{
    bool is_used;
    char phone_number[12];
    int Box_ID;
    char Claim_code[7];
}Claim_BoxData;




#endif /* USEDATA_H_ */