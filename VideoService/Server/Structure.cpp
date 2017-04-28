//
// Created by parallels on 4/27/17.
//

#include "Structure.h"
FILE *flog;

struct shared_package * get_shared_package(){
    int shmid;
    shmid = shmget(960828, sizeof(struct shared_package), 0666 | IPC_CREAT);
    struct shared_package * s = (struct shared_package *) shmat(shmid, NULL, 0);
    printf("shmid:%d, p:%lx\n", shmid, s);
    return s;
}
void printlog(const char *format,...)
{
    va_list args;

    va_start(args ,format);
    vprintf(format ,args);
    va_end(args);

    va_start(args, format);
    vfprintf(flog, format, args);
    va_end(args);
    fflush(flog);
}