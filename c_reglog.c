#include "comment.h"
void regc(int sd)
{
    REG zc;
    SHIP ship;
    printf("请输入账号-密码-电话:\n");
    scanf("%s%s%d", zc.name, zc.passwd, &zc.tel);
    memcpy(ship.house, &zc, sizeof(zc));
    ship.type = regtp;
    write(sd, &ship, sizeof(ship));
    read(sd, &ship, sizeof(SHIP));
    if (ship.type == OK)
    {
        printf("注册成功\n");
    }
    else
    {
        printf("注册失败，用户名已存在\n");
    }
}

void lgc(int sd)
{
    LOG dl;
    SHIP ship;
    printf("请输入账号-密码\n");
    scanf("%s%s", dl.name, dl.passwd);
    memcpy(ship.house, &dl, sizeof(dl));
    ship.type = logtp;
    write(sd, &ship, sizeof(SHIP));
    read(sd, &ship, sizeof(SHIP));
    if (ship.type == OK)
    {
        printf("登录成功\n");
        usermenu(sd);
    }
    else
    {
        printf("登录失败，账号或密码错误\n");
        printf("%d\n",ship.type);
    }
}

void *work(void *sid)
{
    int sd = *(int *)sid;
    while (1)
    {
        SHIP ship;
        read(sd, &ship, sizeof(SHIP));
        printf("收到的消息为：%s\n", ship.house);
    }
}

void usermenu(int sd)
{
    int run = 1;
    while (run)
    {
        SHIP ship;
        system("clear");
        printf("1.群聊 2.单聊 3.查看聊天记录 4.添加好友 5.退出\n");
        int num;
        scanf("%d", &num);
        switch (num)
        {
        case 1:
        {
            SHIP ship;
            ship.type = QUNLIAO;
            pthread_t pt;
            pthread_create(&pt, NULL, work, &sd);
            while (1)
            {
                scanf("%s", ship.house);
                write(sd, &ship, sizeof(SHIP));
                if (strcmp(ship.house, "quit") == 0)
                {
                    pthread_cancel(pt);
                    break;
                }
            }
        }
            // ... existing code ...
case 2:
{
    SHIP ship;
    ship.type = DANLIAO;
    printf("请输入好友名称: ");
    scanf("%s", ship.house);
    write(sd, &ship, sizeof(SHIP));
    
    pthread_t pt;
    pthread_create(&pt, NULL, work, &sd);
    
    while (1)
    {
        CHAT_MSG msg;
        printf("输入消息 (输入 'quit' 退出): ");
        scanf("%s", msg.content);
        if (strcmp(msg.content, "quit") == 0)
        {
            pthread_cancel(pt);
            break;
        }
        strcpy(msg.to, ship.house);
        memcpy(ship.house, &msg, sizeof(CHAT_MSG));
        write(sd, &ship, sizeof(SHIP));
    }
     }
    break;
    case 3:
    {
        SHIP ship;
        ship.type = VIEW_HISTORY;
        write(sd, &ship, sizeof(SHIP));
        // 接收并显示聊天记录
        read(sd, &ship, sizeof(SHIP));
        printf("聊天记录:\n%s\n", ship.house);
    }
    break;
    case 4:
    {
        SHIP ship;
        ship.type = ADD_FRIEND;
        printf("请输入要添加的好友名称: ");
        scanf("%s", ship.house);
        write(sd, &ship, sizeof(SHIP));
        read(sd, &ship, sizeof(SHIP));
        if (ship.type == OK)
        {
            printf("好友添加请求已发送\n");
        }
        else
        {
            printf("好友添加请求发送失败\n");
        }
    }
    break;

        break;
        case 5:
            run = 0;
            break;
        default:
            break;
        }
    }
}
