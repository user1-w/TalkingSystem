#include "comment.h"

extern char clientaddrs[1000];
extern int clientindex;

void regs(SHIP ship, int sd)
{
    REG *zc = (REG *)ship.house;
    int fd = open("user.txt", O_RDWR | O_CREAT | O_APPEND, 0666);
    if (fd < 0)
    {
        perror("fd error\n");
    }
    REG zcread;
    while (0 < read(fd, &zcread, sizeof(REG)))
    {
        if (!strcmp(zcread.name, zc->name))
        {
            printf("注册失败\n");
            ship.type = NO;
            write(sd, &ship, sizeof(SHIP));
            close(fd);
            return;
        }
    }
    if (ship.type != NO)
    {
        ship.type = OK;
        write(sd, &ship, sizeof(SHIP));
        write(fd, zc, sizeof(REG));
        printf("注册成功\n");
        close(fd);
        return;
    }
}

void *work(void *sid) // 发给除了自己的客户端.
{
    int sd = *(int *)sid;
    SHIP ship;
    while (1)
    {
        for (int i = 0; i < clientindex; i++)
        {
            if (clientaddrs[i] != sd)
            {
                write(clientaddrs[i], &ship, sizeof(SHIP));
            }
        }
    }
}

void QUnLiao(SHIP ship,int sd)
{

    pthread_t pt;
    pthread_create(&pt, NULL, work, &sd);
    while (1)
    {
        read(sd,&ship,sizeof(SHIP));
        printf("message:%s\n", ship.house);
    }
}

void lgs(SHIP ship, int sd)
{
    LOG *dl = (LOG *)ship.house;
    int fd = open("user.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("fd error\n");
    }
    REG zcread;
    while (0 < read(fd, &zcread, sizeof(REG)))
    {
        if (!strcmp(dl->name, zcread.name) && !strcmp(dl->passwd, zcread.passwd))
        {
            ship.type = OK;
            printf("登录成功\n");
            write(sd, &ship, sizeof(SHIP)); // 把OK发回给客户端去判断。
            clientaddrs[clientindex] = sd;
            clientindex++;
            close(fd);
            return;
        }
    }
    if (ship.type != OK)
    {
        ship.type = NO;
        printf("登录失败\n");
        write(sd, &ship, sizeof(SHIP));
        close(fd);
        return;
    }
}
