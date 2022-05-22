# Task 4 report

Task 3 is already completed, so let's go through the checkboxes of task 4:

## Checkboxes

### Use your USB device (flash drive, mouse etc.) as an `electronic` key for you chardev.

Added flag inside kernel module that is activated when usb device is connected. We ensure that only a particular device is allowed through table of supported devices (`struct usb_device_id stack_table []`).

### So you chardev device from TASK3 must appear only when USB device is inserted.
```console
// usb device connected
root@ubuntu# echo 123 > /dev/stack_file_kek_chrdev0 
root@ubuntu# cat /dev/stack_file_kek_chrdev0 
123
root@ubuntu# echo 123 > /dev/stack_file_kek_chrdev0
root@ubuntu# echo 123 > /dev/stack_file_kek_chrdev0
root@ubuntu# echo 123 > /dev/stack_file_kek_chrdev0
root@ubuntu# echo 1223 > /dev/stack_file_kek_chrdev0
// usb device disconnected
root@ubuntu# cat /dev/stack_file_kek_chrdev0 
cat: /dev/stack_file_kek_chrdev0: Permission denied
root@ubuntu# cat /dev/stack_file_kek_chrdev0 
cat: /dev/stack_file_kek_chrdev0: Permission denied
// usb device connected
root@ubuntu# cat /dev/stack_file_kek_chrdev0 
1223123123123
root@ubuntu# 
```
*(changed a little bit for readability)*

*my comments after `//`*

[Syslog for the example](./syslog4.txt)

As you can see, the character device is accessible only when the USB drive is plugged in.

### Stack itself should not be destroyed when USB device is removed.

Demonstrated in the previous example.

## Set up steps (just in case)

Module is compiled and placed in `/lib/modules/5.13.0-41-generic/external/`
```console
abeba@ubuntu$ sudo modprobe -r uas
abeba@ubuntu$ sudo modprobe -r usb_storage
abeba@ubuntu$ sudo modprobe -r stack_device
abeba@ubuntu$ sudo depmod
abeba@ubuntu$ sudo rm /dev/stack_file_kek_chrdev0
```
Running tests with and without USB drive
```
// device is disconnected
abeba@ubuntu$ sudo ./launch.sh 
[sudo] password for abeba: 
Testing /dev/stack_file_kek_chrdev0
Preparing the file..
File should be ready
Running test One element
Error passing test: (errno 13) Permission denied

Running test Negative element
Error passing test: (errno 13) Permission denied

Running test Ten elements
Error passing test: (errno 13) Permission denied

Running test Min max
Error passing test: (errno 13) Permission denied

Running test 256 numbers
Error passing test: (errno 13) Permission denied

Running test Resize
Error passing test: (errno 13) Permission denied

Running test Concurrency
Thread 1 returned 13 (errno Permission denied)
Thread 2 returned 13 (errno Permission denied)
Error passing test: (errno 13) Permission denied


Passed 0/7 tests

// device is connected
abeba@ubuntu$ sudo ./launch.sh 
mknod: /dev/stack_file_kek_chrdev0: File exists
Testing /dev/stack_file_kek_chrdev0
Preparing the file..
Finished clearing file (No more data left)
Getting device major number for stack_file_kek_chrdev.
Found number: 236.
File should be ready
Running test One element
Writing '-31337' (7 bytes)
Read '-31337'
Test passed!

Running test Negative element
Writing '-31337' (7 bytes)
Read '-31337'
Test passed!

Running test Ten elements
Writing '31337' (6 bytes)
...

...
Read '60'
Read '50'
Read '40'
Read '30'
Read '20'
Read '10'
Thread 1 returned 0 (errno Success)
Thread 2 returned 0 (errno Success)
Test passed!

Finished clearing file (No more data left)
Getting device major number for stack_file_kek_chrdev.
Found number: 236.

Passed 7/7 tests

// device is disconnected
abeba@ubuntu$ sudo ./launch.sh 
mknod: /dev/stack_file_kek_chrdev0: File exists
Testing /dev/stack_file_kek_chrdev0
Preparing the file..
File should be ready
Running test One element
Error passing test: (errno 13) Permission denied

Running test Negative element
Error passing test: (errno 13) Permission denied

Running test Ten elements
Error passing test: (errno 13) Permission denied

Running test Min max
Error passing test: (errno 13) Permission denied

Running test 256 numbers
Error passing test: (errno 13) Permission denied

Running test Resize
Error passing test: (errno 13) Permission denied

Running test Concurrency
Thread 1 returned 13 (errno Permission denied)
Thread 2 returned 13 (errno Permission denied)
Error passing test: (errno 13) Permission denied


Passed 0/7 tests
```
*(changed a little bit for readability)*

*my comments after `//`*
