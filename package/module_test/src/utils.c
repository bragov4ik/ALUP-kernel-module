#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

int get_device_major(char* name) {
    printf("Getting device major number for %s.\n", name);
    FILE* fd = fopen("/proc/devices", "r");
    char line[256];
    char* number_str;
    char* next_name;
    while(1) {
        if (fgets(line, 255, fd) == NULL) {
            break;
        }
        line[255] = '\0';
        number_str = strtok(line, " ");
        next_name = strtok(NULL, "\n");
        if (strcmp(next_name, name) == 0) {
            printf("Found number: %s.\n", number_str);
            return atoi(number_str);
        }
    }
    return -1;
}