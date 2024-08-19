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

void save_chat_history(CHAT_MSG *msg)
{
    int fd = open("chat_history.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd >= 0)
    {
        char buffer[1024];
        char time_str[64];
        struct tm *timeinfo = localtime(&msg->timestamp);
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", timeinfo);
        int len = snprintf(buffer, sizeof(buffer), "[%s] %s to %s: %s\n", time_str, msg->from, msg->to, msg->content);
        write(fd, buffer, len);
        close(fd);
    }
}

void read_chat_history(const char *username, char *history)
{
    int fd = open("chat_history.txt", O_RDONLY);
    if (fd >= 0)
    {
        char line[1024];
        int bytes_read;
        int pos = 0;
        while ((bytes_read = read(fd, line + pos, sizeof(line) - pos - 1)) > 0)
        {
            pos += bytes_read;
            line[pos] = '\0';
            
            char *start = line;
            char *end;
            while ((end = strchr(start, '\n')) != NULL)
            {
                *end = '\0';
                if (strstr(start, username))
                {
                    strcat(history, start);
                    strcat(history, "\n");
                }
                start = end + 1;
            }
            
            if (start < line + pos)
            {
                memmove(line, start, line + pos - start);
                pos = line + pos - start;
            }
            else
            {
                pos = 0;
            }
        }
        close(fd);
    }
}
void AddFriend(SHIP ship, int sd)
{
    FRIEND_REQ *req = (FRIEND_REQ *)ship.house;
    int target_sd = find_user_sd(req->to);
    if (target_sd > 0)
    {
        write(target_sd, &ship, sizeof(SHIP));
        ship.type = OK;
    }
    else
    {
        ship.type = NO;
    }
    write(sd, &ship, sizeof(SHIP));
}

void ViewHistory(SHIP ship, int sd)
{
    // 从文件中读取聊天记录
    char history[1024] = {0};
    read_chat_history(ship.house, history);
    strcpy(ship.house, history);
    write(sd, &ship, sizeof(SHIP));
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
