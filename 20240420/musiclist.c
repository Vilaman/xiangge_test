/***********************************************************************************
# File Name:    musiclist.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月30日 星期二 19时43分31秒
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
#define N 50

typedef struct Song{

	char name[30];
	char singer[16];
}Song;


typedef struct Musiclist{

	Song *song;
	int length;
	int capacity;
}Mulist;


void menu();
void showSonglist(Mulist *list);
Mulist initlist();
void Addcapacity(Mulist *list);
void Appendlist(Mulist *list);
void DeleteSong(Mulist *list);
int size=5;//定义初始长度
int line=0;//定义文件初始行数
Mulist playlist;


int main(){
playlist=initlist();
menu();	


    return 0;
}
void menu(){
bool isRun=true;
printf("欢迎来到xxxxxx音乐\n");
do{
printf("----------------------------------------\n");
printf("1.添加音乐到列表 2.在列表中删除音乐\n");
printf("3.查看当前列表   4.退出 \n");
printf("----------------------------------------\n");
char chose;
printf("请输入你要选择的功能\n");
scanf(" %c",&chose);
switch(chose){

	case '1':
		Appendlist(&playlist);
		break;
	case '2':
		DeleteSong(&playlist);
		break;
	case '3':
		showSonglist(&playlist);
		break;
	case '4':
		isRun=false;
		break;
	default:
		puts("请输入正确选项\n");
		break;
}
}while(isRun);


}





//初始化顺序列表
Mulist initlist() {
  int num;
  Mulist list = {0};
  FILE *infd = fopen("playlist.txt", "r+");
  if (infd == NULL) {

    perror("fopen()");
    exit(1);
  }
  char buf[N];
  while (fgets(buf, N, infd) != NULL) {
    if (buf[strlen(buf) - 1] == '\n')
      line++;
  }
  fseek(infd, 0, SEEK_SET);
  if (line > size) {
    size += line;
  }
Song *space = (Song *)malloc(size * sizeof(Song));
if (space == NULL) {
  perror("malloc()");
  exit(1);
}
for (int i = 0; i < line; i++) {

  fscanf(infd, "%d %s %s\n", &num, space[i].name, space[i].singer);
}

list.song = space;
list.length = line;
list.capacity = size;
return list;
}
//当前列表已满时扩容
void Addcapacity(Mulist *list){

	list->capacity*=2;
	Song *newspace=(Song *)malloc(list->capacity*sizeof(Song));
	if(newspace==NULL){
		perror("malloc()");
		exit(1);
	}
	for(int i=0;i<list->length;i++){
	newspace[i]=list->song[i];
	}
	free(list->song);
	list->song=newspace;
	printf("扩容成功，当前容量为%d\n",list->capacity);
	return;
}

//尾部插入
void Appendlist(Mulist *list){
	Song song={0};
	printf("请输入你要添加的歌曲名\n");
	scanf(" %s",song.name);
	printf("请输入歌手名\n");
	scanf(" %s",song.singer);

	if(list->length==list->capacity){
		Addcapacity(list);
	}
	list->song[list->length]=song;
	list->length+=1;
	return;
}

//删除音乐
void DeleteSong(Mulist *list){
	int index;
	printf("请输入你要删除的歌曲序列号\n");
	scanf(" %d",&index);

	if(index<1||index>list->length){

		return;
	}
	for(int i=index-1;i<list->length;i++){

		list->song[i]=list->song[i+1];
	}
	memset(&list->song[list->length-1],0,sizeof(Song));
	list->length-=1;
	printf("删除成功，删除后的列表为\n");
	showSonglist(list);
}


//查看播放列表并创建播放列表playlist文件
void showSonglist(Mulist *list) {
  FILE *fd = fopen("playlist.txt", "w+");
  if (fd == NULL) {

    perror("fopen()");
    exit(1);
  }

  for (int i = 0; i <= list->length; i++) {
   if(strcmp(list->song[i].name,"")!=0){
	fprintf(fd, "%d.%s %s\n", i + 1, list->song[i].name,list->song[i].singer);
    printf("%d.%s %s\n", i + 1, list->song[i].name, list->song[i].singer);
   }
  }
  fclose(fd);
  fd = NULL;

  return;
}



