#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <unistd.h>
#include <fcntl.h>

#define PAGE_SIZE  ((size_t)getpagesize())
#define PAGE_MASK ((uint64_t) (long)~(PAGE_SIZE - 1))

#define PL_MEMORY_START_ADDR 0x440000000
#define PL_MEMORY_SIZE 0x10000

void Xil_Out32(uint64_t phyaddr, uint32_t val)
{
        int fd;
        volatile uint8_t *map_base;
        uint64_t base = phyaddr & PAGE_MASK;
        uint64_t pgoffset = phyaddr & (~PAGE_MASK);

        if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1)
        {
                printf("open /dev/mem:\n");
                perror("open /dev/mem:");
        }

        map_base = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
                        fd, base);
        if(map_base == MAP_FAILED)
        {
                printf("mmap error\n");
                perror("mmap:");
        }
        //printf("start write!\n");
        *(volatile uint32_t *)(map_base + pgoffset) = val;
        //printf("end write!\n");
        close(fd);
        munmap((void *)map_base, PAGE_SIZE);
}

int Xil_In32(uint64_t phyaddr)
{
        int fd;
        uint32_t val;
        volatile uint8_t *map_base;
        uint64_t base = phyaddr & PAGE_MASK;
        uint64_t pgoffset = phyaddr & (~PAGE_MASK);
        //open /dev/mem
        if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1)
        {
                perror("open /dev/mem:");
        }
        //mmap
        map_base = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
                        fd, base);
        if(map_base == MAP_FAILED)
        {
                perror("mmap:");
        }
        val = *(volatile uint32_t *)(map_base + pgoffset);
        close(fd);
        munmap((void *)map_base, PAGE_SIZE);

        return val;
}

void rio_setreg32(unsigned int addrBase,unsigned int addrOffset,unsigned int value)
{
        Xil_Out32(addrBase+addrOffset, value);
}

int rio_getreg32(unsigned int addrBase,unsigned int addrOffset)
{
        int ans=0;
        ans=Xil_In32(addrBase+addrOffset);
        return ans;
}
int main(){

        int fd;
        volatile uint64_t *reg_base = NULL;     
        void* map_base = NULL;
        size_t i;

        if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1)
        {
                printf("open /dev/mem:\n");
                perror("open /dev/mem:");
        }

        map_base = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PL_MEMORY_START_ADDR);
        if(map_base == MAP_FAILED)
        {
                printf("mmap error\n");
                perror("mmap:");
        }
        reg_base=(uint64_t*)map_base;

        for(i=0;i<256;i++){
           reg_base[i] = (uint64_t)(i+1);
        }
        for(i=0;i<256;i++){
            //if( reg_base[i]!=(uint32_t)(i+1)){
                printf("%#x\n",reg_base[i]);
              //  break;
            //}
        }
        if(i==256)
            printf("success!\n");

        close(fd);
        munmap((void *)map_base, PAGE_SIZE);
        return 0;
}
