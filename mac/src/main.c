#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <errno.h>
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>

#define    MAC_LEN                 6
#define    IPMB_TYPE               0x08ff
#define    IPMB_TYPE_HIGH_POS      13
#define    IPMB_TYPE_LOW_POS       12

struct sockaddr_ll g_soll;
char g_txbuf[1540];
char g_rxbuf[1540];
char imx6ull_dmac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
char imx6ull_smac[6] = {0};
int g_psf = -1;
int g_prf = -1;

typedef struct {
    char dest_addr;
    char dest_fn:6;
    char dest_lun:2;
    char chksum1;
    char src_addr:6;
    char src_lun:2;
    char src_cmd;
    char src_seq;
    char data[1];  // chksum2
} sIpmbFrame, *pIpmbFrame;

typedef struct {
    unsigned short uLen;
    sIpmbFrame data;
} IpmbMsg, *pIpmbMsg;

int createRecvProc(const char *dev)
{
    int sockfd = -1;
    sockfd = socket(AF_PACKET, SOCK_RAW, IPMB_TYPE);
    if (sockfd < 0) {
        printf ("socket: %s\n", strerror(errno));
        return -1;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, dev, strlen(dev) + 1) == -1) {
        printf ("setsockopt: %s\n", strerror(errno));
        return -1;
    }
    return sockfd;
}

int getEthMac(const char *dev, char *buf)
{
    struct ifreq ifr;
    int sockfd;
    int i;

    // 创建Socket，实际上这个Socket并不会被使用，只是用来获取MAC地址信息
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 设置接口名
    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, dev);

    // 获取MAC地址
    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1) {
        perror("ioctl");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    memcpy(buf, ifr.ifr_hwaddr.sa_data, MAC_LEN);

    printf ("%s MAC: ", dev);
    for (i = 0; i < MAC_LEN; i++) {
        printf ("0X%X ", buf[i]);
    }

    printf ("\n");

    close(sockfd);
    return 0;
}


int sendMsg(char *buf, int len)
{
    int ret;
    ret = sendto(g_psf, buf, len, 0, (struct sockaddr *)&g_soll, sizeof(g_soll));
    if (ret != len) {
        printf ("len: 0x%x, send: 0x%x\n", len, ret);
        return -1;
    }
    return 0;
}

int sendIpmbMsg(int ulSlot, int ulFruid, char *buf, int ulLen)
{
    pIpmbMsg pMsg;
    pIpmbFrame pFrame;
    int i;
    char chksum;

    pMsg = (pIpmbMsg) malloc(sizeof(IpmbMsg) + ulLen);
    pFrame = &pMsg->data;

    // 数据的长度
    pMsg->uLen = sizeof(sIpmbFrame) + ulLen;

    pFrame->dest_addr = ulSlot;
    pFrame->src_addr = 0x20;
    pFrame->dest_fn = 0x6;
    pFrame->src_cmd = 0x80;
    pFrame->src_lun = 0;
    pFrame->dest_lun = ulFruid;
//    pFrame->data[0] = ulFruid;
    pFrame->src_seq = 0x1e;

    memcpy(pFrame->data, buf, ulLen);

    chksum += ((char *)(pFrame))[0];
    chksum += ((char *)(pFrame))[1];
    pFrame->chksum1 = (char)0 - chksum;

    chksum = 0;

    for (i = 3; i < sizeof(sIpmbFrame) + ulLen - 1; i++) {
        chksum += ((char *)(pFrame))[i];
    }

    ((char *)(pFrame))[i] = (char)0 - chksum;

    memcpy(g_txbuf, imx6ull_dmac, MAC_LEN);
    memcpy(&g_txbuf[MAC_LEN], imx6ull_smac, MAC_LEN);

    g_txbuf[IPMB_TYPE_LOW_POS] = 0x07;
    g_txbuf[IPMB_TYPE_HIGH_POS] = 0xff;

    memcpy(&g_txbuf[MAC_LEN + MAC_LEN + 2], pMsg, sizeof(IpmbMsg) + ulLen);

    sendMsg(g_txbuf, MAC_LEN + MAC_LEN + 2 + sizeof(IpmbMsg) + ulLen);

    free(pMsg);

    return 0;
}

int InitEthCommon(void)
{
    g_psf = socket(AF_PACKET, SOCK_RAW, 0);
    if (g_psf < 0) {
        printf ("InitEthCommon error: %s\n", strerror(errno));
    }

    g_soll.sll_family = AF_PACKET;
    g_soll.sll_halen = MAC_LEN;
    memcpy(g_soll.sll_addr, imx6ull_dmac, MAC_LEN);
    g_soll.sll_ifindex = if_nametoindex("ens37");

    // 获取 网口 mac 地址

    g_prf = createRecvProc("ens37");

    getEthMac("ens37", imx6ull_smac);

    return 0;
}

int main(void)
{
    InitEthCommon();
    sendIpmbMsg(0xa, 0, "123", 3);
}
