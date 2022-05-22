#include <linux/module.h> // included for all kernel modules
#include <linux/kernel.h> // included for KERN_INFO
#include <linux/fs.h> // file_ops
#include <linux/init.h> // included for __init and __exit macros
#include <linux/uaccess.h> // copy_to_user copy_from_user
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/mutex.h>

#include "main.h"
#include "utils.h"

#define CHRDEV_NAME "stack_file_kek_chrdev"
#define INIT_STACK_LIMIT 256
#define SUCCESS 0

// Major number of the character device
static int MAJOR;

typedef struct Stack {
	// Points to start of memory allocated for the stack
	int* start;
	// Index of element after the end of the stack (max = max_size, min = 0)
	unsigned int sp;
	// Max number of elements in stack (also size of allocated memory in integers)
	unsigned int max_size;
} Stack;

// Stack containing saved data
static Stack stack_instance;

// Flag indicating that the device is opened
// static bool is_opened;

// Mutex for managing concurrent reads/writes
DEFINE_MUTEX(stack_mutex);

static void acquire_mutex(struct mutex* m, char* op_name) {
	printk(KERN_DEBUG "Acquiring mutex for '%s'...\n", op_name);
	mutex_lock(m);
	printk(KERN_DEBUG "Mutex for '%s' acquired.\n", op_name);
}

static void release_mutex(struct mutex* m, char* op_name) {
	printk(KERN_DEBUG "Releasing mutex for '%s'...\n", op_name);
	mutex_unlock(m);
	printk(KERN_DEBUG "Mutex for '%s' released.\n", op_name);
}

/* Updates size of the stack memory allocation,
 * copying content of the stack that fits into the
 * new allocation.
 *
 * Returns `-ENOMEM` if memory allocation with size
 * `new_size` fails, 0 on success.
 */
static int resize(uint new_size) {
	int* new_alloc;
	int copy_size;

	printk(KERN_DEBUG "Resizing stack to %d.\n", new_size);
	printk(KERN_DEBUG "Allocating new memory region.\n");
	new_alloc = kmalloc(sizeof(int)*new_size, GFP_KERNEL);
	if (new_alloc == NULL) {
		return -ENOMEM;
	}

	acquire_mutex(&stack_mutex, "resize");
	{
		copy_size = new_size;
		if (stack_instance.sp < new_size) {
			// Do not copy free stack memory
			copy_size = stack_instance.sp;
		}
		else {
			// Shrink the stack to fit into new region
			stack_instance.sp = copy_size;
		}

		// Update corresponding fields
		printk(KERN_DEBUG "Copying memory into the region.\n");
		memcpy(new_alloc, stack_instance.start, copy_size);
		printk(KERN_DEBUG "Freeing old region.\n");
		kfree(stack_instance.start);
		stack_instance.start = new_alloc;
		stack_instance.max_size = new_size;
	}
	release_mutex(&stack_mutex, "resize");

	printk(KERN_DEBUG "Finished resizing!\n");
	return SUCCESS;
}

/* Clear the stack (basically resets stack pointer)
 * to 0. Not expected to fail.
 */
static void clear(void) {
	acquire_mutex(&stack_mutex, "clear");
	stack_instance.sp = 0;
	release_mutex(&stack_mutex, "clear");
}

ssize_t stack_read(struct file *filp, char __user *buf, size_t count, loff_t *offset) {
	int item;
	char* str;
	int size_str;
	int uncopied;

	printk(KERN_DEBUG "Reading from stack.\n");

	acquire_mutex(&stack_mutex, "read");
	{
		if (stack_instance.sp == 0) {
			printk(KERN_INFO "No data in stack left.\n");
			release_mutex(&stack_mutex, "read");
			return SUCCESS;
		}
		// Pop off the stack
		item = stack_instance.start[stack_instance.sp-1];
		printk(KERN_DEBUG "%d is the head of stack", item);
		stack_instance.sp -= 1;

		// Convert to string and copy to buffer
		str = my_itos(item, count);
		size_str = my_strlen(str)+1;
		printk(KERN_DEBUG "Copying '%s' to user space", str);
		uncopied = copy_to_user(buf, str, size_str);
		kfree(str);

		if (uncopied > 0) {
			printk(KERN_WARNING "Could not copy %d/%d bytes\n", uncopied, count);
		}
		else {
			printk(KERN_DEBUG "Read %d successfully", item);
		}
	}
	release_mutex(&stack_mutex, "read");
	
	return size_str - uncopied;
}

ssize_t stack_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset) {
	// 12 to contain max int + surely check overflow
	// (as 11 may falsely not report it)
	char local_buf[13];
	int uncopied;
	int res;

	printk(KERN_DEBUG "Writing to stack.\n");

	acquire_mutex(&stack_mutex, "write");
	{
		if (stack_instance.sp >= stack_instance.max_size) {
			printk(KERN_WARNING "Stack is full.\n");
			release_mutex(&stack_mutex, "write");
			return -ENOSPC;
		}
		uncopied = copy_from_user(local_buf, buf, 12);
		local_buf[12] = '\0';
		if (uncopied >= 12) {
			printk(KERN_ERR "Couldn't copy data from userspace.\n");
			release_mutex(&stack_mutex, "write");
			return -EIO;
		}
		printk(KERN_DEBUG "Parsing and saving %s to stack", local_buf);
		res = kstrtoint(local_buf, 10, &stack_instance.start[stack_instance.sp]);
		if (res < 0) {
			if (res == -ERANGE) {
				printk(KERN_ERR "Provided number overflows int.\n");
			}
			else if (res == -EINVAL) {
				printk(KERN_ERR "Couldn't parse the number.\n");
			}
			release_mutex(&stack_mutex, "write");
			return res;
		}
		printk(KERN_DEBUG "%d is on stack", stack_instance.start[stack_instance.sp]);
		stack_instance.sp += 1;
	}
	release_mutex(&stack_mutex, "write");

	return count;
}

int stack_open (struct inode *inode, struct file *filp) {
	printk(KERN_DEBUG "Opening stack.\n");
	// Race condition?? toc/tou - pohui delete
	// if (is_opened == true) {
	// 	printk(KERN_WARNING "Could not open stack device: Already in use");
	// 	return -EBUSY;
	// }
	// is_opened = true;
	printk(KERN_DEBUG "Opened stack.\n");
	return SUCCESS;
}

int stack_release (struct inode *inode, struct file *filp) {
	printk(KERN_DEBUG "Releasing stack.\n");
	// is_opened = false;
	return SUCCESS;
}

/* Called on ioctl on device file.
 *
 * If the ioctl is write or read/write (meaning output 
 * is returned to the calling process), the ioctl call 
 * returns the output of this function.
 */
long device_ioctl(
	struct file *file,
	unsigned int ioctl_num,	   /* The number of the ioctl */
	unsigned long ioctl_param) /* The parameter to it */
{
	uint new_size;

	printk(KERN_INFO "Received ioctl %d!\n", ioctl_num);
    /* Update size of device's stack. */
	if (ioctl_num == IOCTL_CHANGE_SIZE) {
		new_size = (int)ioctl_param;
	    printk(KERN_INFO "Resizing the stack to %d\n", new_size);
		// mutex locks inside
        return resize(new_size);
	}
    /* Clear stack. */
	else if (ioctl_num == IOCTL_CLEAR) {
	    printk(KERN_INFO "Clearing the stack\n");
		// mutex locks inside
		clear();
	}
	else {
	    printk(KERN_WARNING "Unknown ioctl command number %d\n", ioctl_num);
        return -ENOTTY;
	}

	return SUCCESS;
}

/* 
 * Stack definitions, data structures, etc.
 */

struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = stack_read,
	.write = stack_write,
	.open = stack_open,
	.release = stack_release,
	.unlocked_ioctl = device_ioctl,
};

int __init stack_init(void) {
	printk(KERN_INFO "Registering character device...\n");
	MAJOR = register_chrdev(0, CHRDEV_NAME, &fops);
	
	if (MAJOR < 0) {
		printk(KERN_ALERT "Failed to register character device with error code %d\n", MAJOR);
		return MAJOR;
	}
	// is_opened = false;
	stack_instance.start = kmalloc(sizeof(int)*INIT_STACK_LIMIT, GFP_KERNEL);
	stack_instance.sp = 0;
	stack_instance.max_size = INIT_STACK_LIMIT;
	print_welcome(CHRDEV_NAME, MAJOR);
	return SUCCESS;
}

void __exit stack_cleanup(void) {
	printk(KERN_INFO "Cleaning up module.\n");
	printk(KERN_INFO "Freeing up memory.\n");
	kfree(stack_instance.start);
	printk(KERN_INFO "Unregistering character device...\n");
	unregister_chrdev(MAJOR, CHRDEV_NAME);
	printk(KERN_INFO "Unregistered character device.\n");
	return;
}


module_init(stack_init);
module_exit(stack_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("bragov4ik@hotmail.com");
MODULE_DESCRIPTION("A Simple character device module that represents a stack");

