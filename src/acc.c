#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
struct acc_config
{
    int fd;
    void *data;
    unsigned long dz;
    void *reg;
    unsigned long rz;
    int rz_fd;
    unsigned long reg_phy_addr;
    unsigned long weight_phy_addr;
};

struct backend
{
    char target[64];
    struct acc_config acc;
};

struct backend yolo_backend;

#define ATTRIBUTE_MAX_SIZE (4096)

#define GET_ATTRIBUTE(path, attr, max_size) \
    do{ \
        int ret; \
        int fd; \
        void *buf; \
        buf = malloc(max_size); \
        fd = ret = open(path, O_RDONLY); \
        if (ret < 0) \
            goto ERROR_HANDLE; \
        ret = read(fd, buf, max_size); \
        if (ret < 0) \
            goto ERROR_HANDLE; \
        attr = strtoul(buf, NULL, 0); \
        close(fd); \
        free(buf); \
    }while(0)

void acc_init(struct acc_config *acc)
{
    void *buf;
    int ret;
   
 
    GET_ATTRIBUTE("/sys/class/uio/uio15/maps/map0/size", acc->rz, ATTRIBUTE_MAX_SIZE);
    GET_ATTRIBUTE("/sys/class/uio/uio15/maps/map1/size", acc->dz, ATTRIBUTE_MAX_SIZE);
    GET_ATTRIBUTE("/sys/class/uio/uio15/maps/map0/addr", acc->reg_phy_addr, ATTRIBUTE_MAX_SIZE);
    GET_ATTRIBUTE("/sys/class/uio/uio15/maps/map1/addr", acc->weight_phy_addr, ATTRIBUTE_MAX_SIZE);


    printf("rs = 0x%x, phy addr = 0x%x\n", acc->rz, acc->reg_phy_addr);
    printf("ws = 0x%x, phy addr = 0x%x\n", acc->dz, acc->weight_phy_addr);
    
    ret = acc->fd = open("/dev/uio15", O_RDWR);
    acc->reg = mmap(NULL, acc->rz, PROT_READ | PROT_WRITE, MAP_SHARED, acc->fd, 0); 
    acc->data = mmap(NULL, acc->dz, PROT_READ | PROT_WRITE, MAP_SHARED, acc->fd, 1 * getpagesize()); 
    
    return;

ERROR_HANDLE:

    if(ret < 0)
    {
        perror("the following error occur");
        exit(-1);
    }

}

void validate_acc()
{
    unsigned long *data;
    acc_init(&yolo_backend.acc);

    data = (unsigned long *) yolo_backend.acc.data;
    printf("[Lucas] &(%p) = 0x%x\n", data, data[0]);
    munmap(yolo_backend.acc.reg, yolo_backend.acc.rz);
    munmap(yolo_backend.acc.data, yolo_backend.acc.dz);
    close(yolo_backend.acc.fd);
    return;
}

void main()
{
    int i;
    for(i = 0 ; i< 1; i++)
        validate_acc();
}
