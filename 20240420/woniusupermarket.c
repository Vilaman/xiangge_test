/***********************************************************************************
# File Name:    woniusupermarket.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年05月02日 星期四 21时03分11秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#define N 20 //定义字符串空间

int line;//定义初始文件行数


//定义商品结构体
typedef struct Product{
int id;
char name[N];
float price;
int sales;
}Product;


//定义商品结构体链表
typedef struct Productnode{
	Product *product;
	struct Productnode *Next;
}Node;


//定义购物车结构体
typedef struct Shopping{
char goods[N];
int num;
float t_price;
}Shopping;

//定义购物车结构体链表
typedef struct shopnode{
	Shopping *shopping;
	struct shopnode *P_Next;
}P_Node;


//定义admin
typedef struct User{
	char username[N];
	char password[N];
}User;
User user={"admin","ad123"};
Node *pt=NULL;//定义全局变量商品指针
P_Node *sp=NULL;//定义全局变量购物车指针

//函数声明
P_Node *shopspace(Shopping *s);
void appendshopping(P_Node **sphead,Shopping *s);
void Productinit();
Node *Addspace(Product *p);
void appendProduct(Node **phead,Product *p);
void choserole();
void customer();
void administrator();
void showAllproduction(Node *phead);
void AddProduct(Node **phead);
void changeProduct(Node **phead);
void delProduct(Node **phead);
bool login();
void showShopping(P_Node *phead);
void buyGoods(P_Node **sphead, Node *phead);
bool payforShopping(P_Node *sphead);





int main(){

	Productinit();
	choserole();

    return 0;
}

//申请购物车空间
P_Node *shopspace(Shopping *s){
	P_Node *newnode=(P_Node *)malloc(sizeof(P_Node));
	if(newnode==NULL){

		perror("malloc()");
		exit(1);
	}
	newnode->shopping=s;
	newnode->P_Next=NULL;
	return newnode;
}


//购物车链表添加数据
void appendshopping(P_Node **sphead,Shopping *s){
	P_Node *newnode=shopspace(s);
	if(*sphead==NULL){

		*sphead=newnode;
	}else{
	
		P_Node *tail=*sphead;
		while(tail->P_Next != NULL){

			tail=tail->P_Next;
		}
		tail->P_Next=newnode;
	}
}



//初始化商品列表
void Productinit(){
	FILE *goodfd=fopen("goods.txt","r+");
	if(goodfd==NULL){

		perror("fopen()");
		exit(1);
	}
	char buf[60]={0};
	while(fgets(buf,sizeof(buf),goodfd)!=NULL){

		if(buf[strlen(buf)-1]=='\n'){
			
			line++;
		}
	}
	if(line==0){

		puts("当前没有商品请添加");
		return;
	}
	Product *p=(Product *)malloc(line*sizeof(Product));
	fseek(goodfd,0,SEEK_SET);
	for(int i=0;i<line;i++){
		fscanf(goodfd,"%d %s %f %d",&p[i].id,p[i].name,&p[i].price,&p[i].sales);
		printf("%s %d %f %d",p[i].name,p[i].id,p[i].price,p[i].sales);
		appendProduct(&pt,(p+i));  //初始化商品清单

	}

	fclose(goodfd);
	goodfd=NULL;
	return;

}

//申请商品链表的空间
Node *Addspace(Product *p){
	Node *newnode=(Node *)malloc(sizeof(Node));
	if(newnode==NULL){

		perror("malloc()");
		exit(1);
	}
	newnode->product=p;
	newnode->Next=NULL;
	return newnode;
}


//商品链表添加数据
void appendProduct(Node **phead,Product *p){
	Node *newnode=Addspace(p);
	if(*phead==NULL){

		*phead=newnode;
	}else{
	
		Node *tail=*phead;
		while(tail->Next != NULL){

			tail=tail->Next;
		}
		tail->Next=newnode;
	}
}



//选择登录角色
void choserole() {
  bool ischose = true;
  do {
    puts("请选择你的登录角色");
    puts("1.顾客  2.管理员");
    char chose;
    scanf(" %s", &chose);
    switch (chose) {
    case '2':
      if (login()) {
        administrator();
      }
      break;
    case '1':
      customer();
      break;
    default:
      puts("请输入正确的选项");
      break;
    }
  } while (1);
}


//基于顾客的菜单
void customer(){
	bool isrun=true;
	puts("请选择功能");
	do{
		puts("-------------------------------------------");
		puts("1.查看购物车 2.继续购买 3.结账 4.退出");
		puts("-------------------------------------------");
		char opt;
		scanf(" %c",&opt);
		switch(opt){

			case '1':
				showShopping(sp);
				break;
			case '2':
				buyGoods(&sp,pt);
				break;
			case '3':
				if(payforShopping(sp)){
					puts("购买成功");
					isrun=false;
				}else{
					puts("购买失败");
				}
				break;
			case '4':
				isrun=false;
				break;
			default:
				puts("请输入正确选项");
		}
	}while(isrun);
}

//基于管理员的菜单
void administrator(){
	puts("请选择功能");
	bool isadmin=true;
	do{
		puts("--------------------------------------------");
		puts("1.查看所有商品 2.添加商品 3.修改商品信息");
		puts("4.删除商品     5.退出");
		puts("--------------------------------------------");
		char option;
		scanf(" %c",&option);
		switch(option){

			case '1':
				showAllproduction(pt);
				break;
			case '2':
				AddProduct(&pt);
				break;
			case '3':
				changeProduct(&pt);
				break;
			case '4':
				delProduct(&pt);
				break;
			case '5':
				isadmin=false;
				break;
			default:
				puts("请输入正确选项");
				break;
		}
	}while(isadmin);
}

//管理员查看所有商品
void showAllproduction(Node *phead){
	if(phead==NULL){
		puts("当前没有商品");
		return;
	}
	Node *tail=phead;
	while(tail !=NULL){
		printf("商品编号:%d 商品名称:%s 商品单价:%.2f 商品销量:%d\n",tail->product->id,tail->product->name,tail->product->price,tail->product->sales);
		tail=tail->Next;
	}
}


//添加商品
void AddProduct(Node **phead){
	Product *new=NULL;
	new=(Product *)malloc(sizeof(Product));
	puts("请输入商品编号");
	scanf(" %d",&new->id);
	puts("请输入商品名称");
	scanf(" %s",new->name);
	puts("请输入商品单价");
	scanf(" %f",&new->price);
	new->sales=0;
	FILE *appendfd=fopen("goods.txt","a+");
	if(appendfd==NULL){

		perror("AddProduct()");
		exit(1);
	}
	fprintf(appendfd,"%d %s %.2f %d\n",new->id,new->name,new->price,new->sales);
	appendProduct(phead,new);
	fclose(appendfd);
	appendfd=NULL;
}

//修改商品信息
void changeProduct(Node **phead){
	bool ischange=false;
	FILE *changefd=fopen("goods.txt","w+");
	if(changefd==NULL){

		perror("changeProduct()");
		exit(2);
	}
	Product *cp=NULL;//定义一个接收修改信息的结构体
	char newname[N];
	if(phead==NULL){
		puts("当前没有商品请先添加商品");
		return;
	}
	cp=(Product *)malloc(sizeof(Product));
	puts("请输入你要修改的商品名称");
	scanf(" %s",cp->name);
	Node *current=*phead;
	while(current!=NULL){
		if(strcmp(current->product->name,cp->name)==0){
			ischange=true;
			puts("请输入新的名称");
			scanf(" %s",current->product->name);
			puts("请输入修改的单价");
			scanf(" %f",&current->product->price);
		}
		fprintf(changefd,"%d %s %.2f %d",current->product->id,current->product->name,current->product->price,current->product->sales);
		current=current->Next;
	}
	fclose(changefd);
	changefd=NULL;
if(!ischange){
	puts("你要修改的商品不存在");
}
	return;
}



//删除商品
void delProduct(Node **phead){
	int delid;
	if(*phead==NULL){
		puts("当前不存在商品");
		return;
	}
	puts("请输入你要删除的商品编号");
	scanf(" %d",&delid);
	Node *Del=*phead;
	while(Del!=NULL){
		if(Del->product->id==delid){
			break;
		}
		Del=Del->Next;
	}
	Node *nextnode=Del;
	Del=Del->Next;
	free(nextnode);
	nextnode=NULL;
	FILE *delfd=fopen("goods.txt","w");
	if(delfd==NULL){
		perror("delProduct()");
		exit(3);
	}
/*	Node *current=*phead;
	while(current!=NULL){
		fprintf(delfd,"%d %s %.2f %d",current->product->id,current->product->name,current->product->price,current->product->sales);
		current=current->Next;
	}*/
	fclose(delfd);
	delfd=NULL;
	return;
}

//管理员登录功能
bool login(){
	char logname[N];
	char pass[N];
	printf("请输入你的用户名\n");
	scanf(" %s",logname);
	printf("请输入你的密码\n");
	scanf(" %s",pass);
	if(strcmp(user.username,logname)==0&&strcmp(user.password,pass)==0){
		puts("登陆成功");
		return true;
	}
	printf("用户名或密码不正确登录失败\n");
	return false;
}

//查看购物车
void showShopping(P_Node *phead){
	if(phead==NULL){
		puts("当前购物车没有商品");
		return;
	}
	P_Node *current=phead;
	while(current!=NULL){

		printf("商品:%s,数量:%d,总价:%f\n",current->shopping->goods,current->shopping->num,current->shopping->t_price);
		current=current->P_Next;
	}
	return;
}

//购买商品添加到购物车
void buyGoods(P_Node **sphead, Node *phead) {
  showAllproduction(phead); //查看所有商品信息
  Shopping *sp = (Shopping *)malloc(sizeof(Shopping));
  puts("请输入你要购买的商品编号");
  int buyid;
  scanf(" %d", &buyid);
  Node *current = phead;
  P_Node *tail = *sphead;
  bool isexsit = false;
  bool isshop = false;
  while (current != 0) {
    if (buyid == current->product->id) {
      current->product->sales++;
      isexsit = true;
      strcpy(sp->goods, current->product->name);
      break;
    }
    current = current->Next;
  }
  if (isexsit) {
    puts("请输入购买的数量");
    scanf(" %d", &sp->num);
    while (tail != NULL) {
      if (strcmp(tail->shopping->goods, sp->goods) == 0) {
        isshop = true;
        break;
      }
      tail = tail->P_Next;
    }
    if (isshop) {
      tail->shopping->num += sp->num;
      tail->shopping->t_price = tail->shopping->num * (current->product->price);
    } else {
      sp->t_price = sp->num * (current->product->price);
      appendshopping(sphead, sp);
    }
  } else {
    puts("你输入的编号不存在");
    return;
  }
  return;
}



//付款模块
bool payforShopping(P_Node *sphead){
float total_shopping=0;
P_Node *cur=sphead;
while(cur!=NULL){

	total_shopping+=cur->shopping->t_price;
	cur=cur->P_Next;
}
float paynum;
puts("请付款");
scanf(" %f",&paynum);
if(paynum-total_shopping>=0){
printf("付款成功,找您%.2f\n",(paynum-total_shopping));
return true;
}else{

	puts("付款失败,请重新付款\n");
	return false;
}
}



