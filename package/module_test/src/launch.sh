#!/bin/sh
modprobe stack_device
sleep 0.5

module="stack_file_kek_chrdev"
major=$(awk -v mod=$module '$2==mod{print $1}' /proc/devices)

mknod /dev/stack_file_kek_chrdev0 c $major 0
sleep 1
./test_program

