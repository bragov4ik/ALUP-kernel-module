#include <linux/module.h> // included for all kernel modules
#include <linux/slab.h> // kmalloc
#include <linux/kernel.h> // snprintf

#include "utils.h"

// Including \0 symbol
#define MAX_INT_LEN 12

char* my_itos(int num, uint max_len) {
    char* result = kmalloc(MAX_INT_LEN, GFP_KERNEL);
    int char_num_would = snprintf(result, MAX_INT_LEN, "%d", num);
    if (char_num_would > MAX_INT_LEN) {
        printk("Could not fit number into a buffer %d bytes required, buffer size - %d", char_num_would, MAX_INT_LEN);
    }
    return result;
}

int my_strlen(char* string) {
    char* aboba = string;
    int len = 0;
    while (*aboba != '\0') {
        len++;
        aboba++;
    }
    return len;
}

/* Print welcoming message with some instructions
 * and stuff.
 */
void print_welcome(char* chrdev_name, int major) {
	printk(KERN_INFO "Successfully registered character device with major %d\n", major);
	printk(KERN_INFO "To talk to the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s0 c %d 0'.\n", chrdev_name, major);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("bragov4ik@hotmail.com");
MODULE_DESCRIPTION("A utilities module for Stack module");