#ifndef __COMMONT_H__
#define __COMMONT_H__

#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>




#define regtp 1
#define logtp 2
#define QUNLIAO 4
#define ADD_FRIEND 6
#define VIEW_HISTORY 7



#define OK 10
#define NO 11

typedef struct registered
{
    char name[128];
    char passwd[128];
    int tel;
} REG;

typedef struct login
{
    char name[128];
    char passwd[128];
} LOG;

typedef struct ship
{
    unsigned char type;
    char house[1024];
    int sd;
} SHIP;

typedef struct friend_request {
    char from[128];
    char to[128];
} FRIEND_REQ;

typedef struct chat_message {
    char from[128];
    char to[128];
    char content[1024];
    time_t timestamp;
} CHAT_MSG;


void regs(SHIP ship,int sd);//服务器注册判断函数
void lgs(SHIP ship,int sd);//服务器登录判断函数
void lgc(int sd);//客户端登录函数
void regc(int sd);
void usermenu(int sd);
void QUnLiao(SHIP ship,int sd);
#endif
