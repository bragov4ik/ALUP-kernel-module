# Stack character device for ALUP course

This device works as stack for integer values. Writing is equal to push operation and reading to pop.

It is built as external buildroot module, thus configs in the root project folder.

Also there are tests for the module (`module_test`) that use the module in various way to see if it works as expected. You can use `launch.sh` in the home folder after starting the OS, which will initialize the module and start tests automatically.

## Adding the project into buildroot

The simplest way is to run
```make BR2_EXTERNAL=/path/to/project menuconfig```
and select corresponding option in the config.

More details can be found there: https://buildroot.org/downloads/manual/manual.html#outside-br-custom

## Building the project with buildroot

To build the project simply use `make`. 

To rebuild after changes were made write `make <package_name>-rebuild`; i.e. if we want to rebuild `bragov4ik` it's `make bragov4ik-rebuild`. Then run `make` again.

## Features

* Write operation - push
* Read operation - pop
* IOCTL stack size change
* IOCTL stack clear
