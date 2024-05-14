/***********************************************************************************
# File Name:    timel.c
# Author:       wenpingxiang
# Mail:         wenpingxiang@gmail.com
# Created Time: 2024年04月28日 星期日 11时51分42秒
 **********************************************************************************/

#include <stdio.h>
#include<string.h> 
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>

int main(){

time_t now;
//time(&now)
//通过time拿到time_t类型的数据
now=time(NULL);
//通过localtime将time_t转为一个包含时间的结构体并返回指针
struct tm *nowtime=localtime(&now);
//printf("月份：%d 天数:%d %d:%d:%d 星期%d",nowtime->tm_mon+1,nowtime->tm_mday,nowtime->tm_hour,nowtime->tm_min,nowtime->tm_sec,nowtime->tm_wday);

char timestr[40];
strftime(timestr,sizeof(timestr)-1,"%Y年%m月%d日 %H:%M:%S",nowtime);
timestr[39]='\0';//字符串数组最后一个用\0补充
//定制星期几的数组，利用结构体wdy来生成星期几字符串，合并到时间字符串里
char *weekday[]={"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};
strcat(timestr,weekday[nowtime->tm_wday]);
puts(timestr);
//计算两个时间的差值
//生命一个新的时间结构体变量来表示另一个时间 使用mktime 将该结构体转为time_t类型数据从而可以计算时间差值

struct tm startTime;
startTime.tm_year=2024-1900;//从1900年开始计算所以减去1900
startTime.tm_mon=7;
startTime.tm_mday=8;
startTime.tm_hour=8;
startTime.tm_min=40;
startTime.tm_sec=40;
//将结构体·转为time_t类型
time_t start=mktime(&startTime);
//获取两个时间差值
double distance=difftime(start,now);
printf("%f\n",distance);

	

    return 0;
}
