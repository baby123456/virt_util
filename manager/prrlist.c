#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>

#include <unistd.h>
#include <fcntl.h>

#define PAGE_SIZE  ((size_t)getpagesize())
#define PAGE_MASK ((uint64_t) (long)~(PAGE_SIZE - 1))

#define PL_MEMORY_START_ADDR 0x81005000
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
        struct timeval tv;
        size_t i = 0;
        printf("\tprr0\tprr1\tprr2\tprr3\ntaskID\t");
        for(i=0;i<4;i++){
            //sleep(1);
            //gettimeofday(&tv, NULL);
            //printf("second: %ld\n", tv.tv_sec);
            //printf("ms: %ld\n", tv.tv_sec * 1000 + tv.tv_usec / 1000); 
            //unsigned long us = tv.tv_sec * 1000000 + tv.tv_usec;          
            //printf("us: %lu\n", (uint32_t)us); 
            //Xil_Out32(PL_MEMORY_START_ADDR+0x4*i,(uint32_t)us);
            //Xil_Out32(PL_MEMORY_START_ADDR+0x4*i,0);
            //printf("0x%08x\n\n",Xil_In32(PL_MEMORY_START_ADDR+0x4*i));    
            printf("%lu\t",Xil_In32(PL_MEMORY_START_ADDR+0x4*i));
        }
        printf("\n");
        /*
        Xil_Out32(PL_MEMORY_START_ADDR+0x4,0x0);
        printf("val=0x%08x\n",Xil_In32(PL_MEMORY_START_ADDR+0x4));
        Xil_Out32(PL_MEMORY_START_ADDR+0x8,0x2);
        printf("val=0x%08x\n",Xil_In32(PL_MEMORY_START_ADDR+0x8));
        Xil_Out32(PL_MEMORY_START_ADDR+0xc,0x2);
        printf("val=0x%08x\n",Xil_In32(PL_MEMORY_START_ADDR+0xc));
    */
        return 0;
}
