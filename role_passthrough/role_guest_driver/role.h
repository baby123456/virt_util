#ifndef _ROLE_H_
#define _ROLE_H_

#define     IOC_MAGIC       'c'
#define     ROLE_ON         _IO(IOC_MAGIC ,0)
#define     ROLE_OFF        _IO(IOC_MAGIC ,1)
#define     ROLE_FREQ       _IO(IOC_MAGIC ,2)
#define     IOC_MAXNR       3

const char CHRDEV_NAME[] = "role";
#define LAST_ROLE           3
#define ERROR           -1

#endif
