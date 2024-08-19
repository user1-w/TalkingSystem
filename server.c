#include "comment.h"

 char clientaddrs[1000];
 int  clientindex = 0;

int main(int argc, const char argv[])
{
    // 服务器创建
    u_int32_t ip;
    inet_pton(AF_INET, "192.168.146.234", &ip);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = ip;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7000);
    int listensd = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(listensd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("error bind\n");
    }
    listen(listensd, 5);
    struct sockaddr_in clientaddr;
    fd_set set1, set2;
    FD_ZERO(&set1);
    FD_ZERO(&set2);
    FD_SET(listensd, &set1);
    set2 = set1;
    int maxFD = listensd;
    while (1)
    {
        int ret = select(maxFD + 1, &set1, 0, 0, 0); // 监测可读状态的套接字，将其他的变为0；
        if (ret == -1)
        {
            perror("");
        }
        else
        {
            for (int i = 3; i < maxFD + 1; i++)
            {
                int num = FD_ISSET(i, &set1); // 检查i在不在set1中
                if (num == 1)
                {
                    if (i == listensd) // 说明lisensd有可读事件能让accept来接受
                    {
                        struct sockaddr_in c_addr;
                        socklen_t clen = sizeof(c_addr);
                        int clientsd = accept(listensd, (struct sockaddr *)&c_addr, &clen);

                        maxFD = clientsd;        // 改变最大套接字
                        FD_SET(clientsd, &set2); // 用set2来保存原数据可以在select监测时用
                    }
                    else // 客户端连接了
                    {
                        SHIP ship;
                        if (0 < read(i, &ship, sizeof(ship)))
                        {
                            if (ship.type == regtp)
                            {
                                regs(ship, i);
                            }
                            else if (ship.type == logtp)
                            {
                                lgs(ship, i);
                            }
                            else if(ship.type ==QUNLIAO)
                            {
                                QUnLiao(ship,i);
                            }
                            else if (ship.type == DANLIAO)
                            {
                                DanLiao(ship, i);
                            }
                            else if (ship.type == ADD_FRIEND)
                            {
                                AddFriend(ship, i);
                            }
                            else if (ship.type == VIEW_HISTORY)
                            {
                                ViewHistory(ship, i);
                            }
                        }
                        else{
                            printf("error\n");
                            close(i);
                            FD_CLR(i,&set2);
                        }
                    }
                }
                set1 = set2; // 将set2的数据给set1，这样在select中就是所有的套接字了，而且set2还保存所有的。
            }
        }
    }
    return 0;
}
