#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

struct DataExchange {
    int i1;
};

int main(int argc, char **argv)
{
    struct DataExchange *pRead, *pWrite;
    char sSharedMemReadName[100] = "_CODESYS_SharedMemoryTest_Write"; 
    char sSharedMemWriteName[100] = "_CODESYS_SharedMemoryTest_Read";

    int fdRead = shm_open(sSharedMemReadName, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
    printf("Shared Memory Read: %s %d\n\n",sSharedMemReadName, fdRead);
    ftruncate(fdRead, sizeof(*pRead));
    pRead = mmap(0, sizeof(*pRead), PROT_READ | PROT_WRITE, MAP_SHARED, fdRead, 0);
    close(fdRead);

    int fdWrite = shm_open(sSharedMemWriteName, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
    printf("Shared Memory Write: %s %d\n\n",sSharedMemWriteName, fdWrite);
    ftruncate(fdWrite, sizeof(*pWrite));
    pWrite = mmap(0, sizeof(*pWrite), PROT_READ | PROT_WRITE, MAP_SHARED, fdWrite, 0);
    close(fdWrite);

    do
    {
        sleep(1);
        printf("pRead->i1: %d pWrite->i1: %d \n", 
        pRead->i1, pWrite->i1);
        printf("Press 'Enter' to increment values or 'q' and then 'Enter' to quit\n");    
        pWrite->i1++;
    } while (getchar() != 'q');

    munmap(pRead, sizeof(*pRead));
    shm_unlink(sSharedMemReadName);

    munmap(pWrite, sizeof(*pWrite));
    shm_unlink(sSharedMemWriteName); 
    exit(0);
 
}
