#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <pthread.h> // concurrency tests
#include <stdbool.h>

#include "tests.h"
#include "utils.h"
#include "chrdev.h"

/*
Clear opened file. (basically pop until error/empty).

Returns 0 on success, `errno` on error.
*/
int __clear_file(int fd) {
    int read_bytes = 1;
    char abobus[255];
    while (read_bytes != 0) {
        read_bytes = read(fd, abobus, 255);
        if (read_bytes == -1) {
            printf("Finished clearing file (%s)\n", strerror(errno));
            return errno;
        }
    }
    printf("Finished clearing file (No more data left)\n");
    return 0;
}

/*
Clear file by name. (basically pop until error/empty).

Returns 0 on success, `errno` on error.
*/
int clear_file(const char* fname) {
    int fd;
    int res;

    fd = open(fname, O_RDWR);
    if (fd == -1) {
        return errno;
    }

    res = __clear_file(fd);
    if (res != 0) {
        close(fd);
        return res;
    }

    close(fd);
    return 0;
}

/*
Test inserting/popping 1 element.

Returns 0 on success, other number on failure
(3 if the test is not passed)
*/
int one_element(const char* fname) {
    const char* numbers[] = { "-31337" };
    return multiple_elements(fname, numbers, sizeof(numbers)/sizeof(numbers[0]), true);
}

/*
Test inserting/popping 1 negative element.

Returns 0 on success, other number on failure
(3 if the test is not passed)
*/
int negative_element(const char* fname) {
    const char* numbers[] = { "-31337" };
    return multiple_elements(fname, numbers, sizeof(numbers)/sizeof(numbers[0]), true);
}

/*
Try inserting/reading 10 numbers to check proper ordering

Returns 0 on success, other number on failure
(3 if the test is not passed)
*/
int ten_numbers(const char* fname) {
    const char* numbers[] = { "31337", "123", "456", "1", "2", "3", "-3", "-2", "-1", "0"};
    return multiple_elements(fname, numbers, sizeof(numbers)/sizeof(numbers[0]), true);
}

/*
Insert min and max int values.

Returns 0 on success, other number on failure
(3 if the test is not passed)
*/
int min_max_test(const char* fname) {
    const char* numbers[] = { "2147483647", "-2147483648" };
    return multiple_elements(fname, numbers, sizeof(numbers)/sizeof(numbers[0]), true);
}

/*
Try inserting 256 numbers and reading them (to test capacity)

Returns 0 on success, other number on failure
(3 if the test is not passed)
*/
int lots_of_numbers(const char* fname) {
    const char* numbers[] = {
        "-128", "-127", "-126", "-125", "-124", "-123", "-122", "-121", "-120",
        "-119", "-118", "-117", "-116", "-115", "-114", "-113", "-112", "-111", "-110",
        "-109", "-108", "-107", "-106", "-105", "-104", "-103", "-102", "-101", "-100",
        "-99", "-98", "-97", "-96", "-95", "-94", "-93", "-92", "-91", "-90",
        "-89", "-88", "-87", "-86", "-85", "-84", "-83", "-82", "-81", "-80",
        "-79", "-78", "-77", "-76", "-75", "-74", "-73", "-72", "-71", "-70",
        "-69", "-68", "-67", "-66", "-65", "-64", "-63", "-62", "-61", "-60",
        "-59", "-58", "-57", "-56", "-55", "-54", "-53", "-52", "-51", "-50",
        "-49", "-48", "-47", "-46", "-45", "-44", "-43", "-42", "-41", "-40",
        "-39", "-38", "-37", "-36", "-35", "-34", "-33", "-32", "-31", "-30",
        "-29", "-28", "-27", "-26", "-25", "-24", "-23", "-22", "-21", "-20",
        "-19", "-18", "-17", "-16", "-15", "-14", "-13", "-12", "-11", "-10",
        "-9", "-8", "-7", "-6", "-5", "-4", "-3", "-2", "-1", "0",
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
        "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
        "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
        "31", "32", "33", "34", "35", "36", "37", "38", "39", "40",
        "41", "42", "43", "44", "45", "46", "47", "48", "49", "50",
        "51", "52", "53", "54", "55", "56", "57", "58", "59", "60",
        "61", "62", "63", "64", "65", "66", "67", "68", "69", "70",
        "71", "72", "73", "74", "75", "76", "77", "78", "79", "80",
        "81", "82", "83", "84", "85", "86", "87", "88", "89", "90",
        "91", "92", "93", "94", "95", "96", "97", "98", "99", "100",
        "101", "102", "103", "104", "105", "106", "107", "108", "109", "110",
        "111", "112", "113", "114", "115", "116", "117", "118", "119", "120",
        "121", "122", "123", "124", "125", "126", "127" };
    return multiple_elements(fname, numbers, sizeof(numbers)/sizeof(numbers[0]), true);
}

/*
Insert `number_count` elements from `numbers` into opened file (`fd`).

Returns 0 on success, other number on failure.
*/
static int __insert_multiple(int fd, const char *const *numbers, int numbers_count) {
    for (int i = 0; i < numbers_count; ++i) {
        printf("Writing '%s' (%d bytes)\n", numbers[i], strlen(numbers[i])+1);
        if (write(fd, numbers[i], strlen(numbers[i])+1) == -1) {
            return errno;
        }
    }
    return 0;
}

/*
Read `number_count` elements from `fd`. Fails if there were less elements.

Returns 0 on success, other number on failure.
(3 number of elements was less than expected).
*/
static int __read_multiple_unchecked(int fd, int numbers_count) {
    // some buffer for ints
    char read_val[13];
    int res;

    read_val[12] = '\0';

    for (int i = numbers_count - 1; i >= 0; --i) {
        res = read(fd, read_val, sizeof(read_val)-1);
        if (res == 0) {
            return 3;
        }
        else if (res == -1) {
            return errno;
        }
        printf("Read '%s'\n", read_val);
    }
    return 0;
}

static int __read_multiple_checked(int fd, const char *const *numbers, int numbers_count) {
    // some buffer for ints
    char read_val[13];
    int res;

    read_val[12] = '\0';

    for (int i = numbers_count - 1; i >= 0; --i) {
        res = read(fd, read_val, sizeof(read_val)-1);
        if (res == 0) {
            return 3;
        }
        else if (res == -1) {
            return errno;
        }
        printf("Read '%s'\n", read_val);
        if (strcmp(numbers[i], read_val) != 0) {
            printf("Read value (%s) different than written (%s)\n", read_val, numbers[i]);
            __clear_file(fd);
            return 3;
        }
    }
    return 0;
}

/*
Test inserting/popping multiple elements.

If `compare_numbers` is set to `true`, it expects to
read inserted numbes in FIFO order. Otherwise, just reads them.

Returns 0 on success, other number on failure
(3 if the test is not passed).
*/
int multiple_elements(const char* fname, const char *const *numbers, int numbers_count, bool compare_numbers) {
    int fd;
    int res;

    fd = open(fname, O_RDWR);
    if (fd == -1) {
        return errno;
    }

    res = __insert_multiple(fd, numbers, numbers_count);
    if (res != 0) {
        close(fd);
        return res;
    }
    
    if (compare_numbers) {
        res = __read_multiple_checked(fd, numbers, numbers_count);
    }
    else {
        res = __read_multiple_unchecked(fd, numbers_count);
    }
    if (res != 0) {
        close(fd);
        return res;
    }
    close(fd);
    return 0;
}

/*
Test double opening chrdev file.

It should fail when opening the same file twice, but
allow opening 1 time each file simultaneously.

Returns 0 on success, other number on failure
(3 if the test is not passed)
*/
int double_open(const char* fname1, const char* fname2) {
    int file1 = open(fname1, O_APPEND);
    if (file1 == -1) {
        return errno;
    }
    int file2 = open(fname2, O_APPEND);
    // Allow both behaviours for now
    if (file2 == -1 && errno != EBUSY) {
        close(file1);
        return errno;
    }
    int file1_again = open(fname1, O_APPEND);
    if (file1_again != -1 || errno != EBUSY) {
        close(file1);
        close(file2);
        close(file1_again);
        if (file1_again == -1) {
            return errno;
        }
        return 3;
    }
    int file2_again = open(fname2, O_APPEND);
    if (file2_again != -1 || errno != EBUSY) {
        close(file1);
        close(file2);
        close(file2_again);
        if (file2_again == -1) {
            return errno;
        }
        return 3;
    }
    close(file1);
    close(file2);
    close(file1_again);
    close(file2_again);
    return 0;
}

int resize(const char* fname, int new_size) {
    int file = open(fname, O_RDWR);
    if (file == -1) {
        return errno;
    }
    int MAJOR = get_device_major("stack_file_kek_chrdev");
    if (ioctl(file, IOCTL_CHANGE_SIZE, new_size) != 0) {
        return errno;
    }
    close(file);
    return 0;
}

/*
Test resizing chrdev file.

Returns 0 on success, -errno on failure
(+3 if the test is not passed)
*/
int test_resize(const char* fname) {
    int res = resize(fname, 10);
    if (res != 0) {
        return res;
    }
    res = ten_numbers(fname);
    if (res != 0) {
        return res;
    }
    res = resize(fname, 9);
    if (res != 0) {
        return res;
    }
    res = ten_numbers(fname);
    if (res == 0) {
        return 3;
    }
    // Previous should have failed, so needs cleanup
    res = clear_file(fname);
    if (res != 0) {
        return res;
    }
    res = resize(fname, 256);
    if (res != 0) {
        return res;
    }
    return 0;
}

typedef struct ThreadArg {
    const char* fname;
    const char *const * numbers;
    int numbers_count;
} ThreadArg;

static void* concurrent_multiple(void* t) {
    ThreadArg* args = (ThreadArg*)t;
    return (void*)multiple_elements(args->fname, args->numbers, args->numbers_count, false);
}

int test_concurrent_multiple(const char* fname) {
    // Thread 1
    pthread_t th1;
    ThreadArg args1;
    args1.fname = fname;
    const char* numbers1[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    args1.numbers = numbers1;
    args1.numbers_count = sizeof(numbers1)/sizeof(numbers1[0]);

    // Thread 2
    pthread_t th2;
    ThreadArg args2;
    args2.fname = fname;
    const char* numbers2[] = {"10", "20", "30", "40", "50", "60", "70", "80", "90"};
    args2.numbers = numbers2;
    args2.numbers_count = sizeof(numbers2)/sizeof(numbers2[0]);

    pthread_create(&th1, NULL, concurrent_multiple, (void*)(&args1));
    pthread_create(&th2, NULL, concurrent_multiple, (void*)(&args2));

    void* ret_from_thread;
    int ri1;
    int ri2;
    pthread_join(th1, &ret_from_thread);
    ri1 = (int)ret_from_thread;
    pthread_join(th2, &ret_from_thread);
    ri2 = (int)ret_from_thread;
    
    printf("Thread 1 returned %d (errno %s)\n", ri1, strerror(ri1));
    printf("Thread 2 returned %d (errno %s)\n", ri2, strerror(ri2));
    if (ri1 != 0) {
        return ri1;
    }
    else if (ri2 != 0) {
        return ri2;
    }
    return 0;
}
