#ifndef MAIN_HEADER
#define MAIN_HAEDER

#define CHRDEV_NAME "stack_file_kek_chrdev"

// NOTE: requires to introduce MAJOR in scope beforehand (major number of the device)

/* 
 * Control messages
 */

// Change size of stack allocation
#define CHANGE_SIZE_NUM 1
#define IOCTL_CHANGE_SIZE _IOW(MAJOR, CHANGE_SIZE_NUM, unsigned int)
// Empty the stack
#define CLEAR_NUM 2
#define IOCTL_CLEAR _IO(MAJOR, CLEAR_NUM)


#endif