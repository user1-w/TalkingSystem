#include "comment.h"

int num = 1;
int main()
{
    u_int32_t ip;
    inet_pton(AF_INET, "192.168.146.234", &ip);
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = ip;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7000);
    int clientsd = socket(AF_INET, SOCK_STREAM, 0);
    connect(clientsd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
    while (num)
    {
        printf("1.注册 2.登录 3.找回密码 4.退出\n");
        int ret;
        scanf("%d", &ret);
        switch (ret)
        {
        case 1:
            regc(clientsd);
            break;
        case 2:
            lgc(clientsd);
            break;
        default:
            num = 0;
            break;
        }
    }
    return 0;
}