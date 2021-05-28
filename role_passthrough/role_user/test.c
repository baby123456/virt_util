#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h> 
#include "role.h"

int main(int argc, char **argv)
{
    int fd;
    int ret;
    fd = open("/dev/role0", O_RDWR);
    if (fd < 0) {
        perror("open");
        exit(-2);
    }                                        
    ret = ioctl(fd, ROLE_ON);
    if (ret) {
        perror("ioctl ROLE_ON:");
        exit(-3);
    }
    return 0;
}


