#!/bin/sh
modprobe stack_device
sleep 0.5
mknod /dev/stack_file_kek_chrdev0 c 248 0
sleep 1
test_program

