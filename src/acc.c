#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

struct acc_config
{
    int fd;
    void *data;
    unsigned long dz;
    void *reg;
    unsigned long rz;
};

struct backend
{
    char target[64];
    struct acc_config acc;
};

struct backend yolo_backend;


void acc_init(struct acc_config *acc)
{
    /*FIXME*/
    acc->rz = 0x1000;
    acc->dz = 0x1000;
    /*get mapping*/
    acc->fd = open("/dev/uio15", O_RDWR);
    acc->reg = mmap(NULL, acc->rz, PROT_READ | PROT_WRITE, MAP_SHARED, acc->fd, 0); 
    acc->data = mmap(NULL, acc->dz, PROT_READ | PROT_WRITE, MAP_SHARED, acc->fd, 1 * getpagesize()); 
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
    for(i = 0 ; i< 10; i++)
        validate_acc();
}
