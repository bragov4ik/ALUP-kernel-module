#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include <string.h>

#include "main.h"
#include "tests.h"
#include "utils.h"

/* Program documentation. */
static char doc[] =
    "Tests for character device kernel module. Performed on a two device file (for testing double-read correctness).";

/* A description of the arguments we accept. */
static char args_doc[] = "CHRDEV_FILE1";

/* The options we understand. */
static struct argp_option options[] = {
    {"output", 'o', "FILE", 0,
     "Output to FILE instead of standard output"},
    {0}};

/* Used by main to communicate with parse_opt. */
struct arguments
{
    char *chrdev_file;
    char *output_file;
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
    struct arguments *arguments = state->input;

    switch (key)
    {
    case 'o':
        arguments->output_file = arg;
        break;

    case ARGP_KEY_ARG:
        if (state->arg_num >= 1)
            /* Too many arguments. */
            argp_usage(state);
        else if (state->arg_num == 0)
            /* First chrdev file */
            arguments->chrdev_file = arg;
        break;

    case ARGP_KEY_END:
        if (state->arg_num < 0)
            /* Not enough arguments. */
            argp_usage(state);
        break;

    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}
// Character device file to test

/* Our argp parser. */
static struct argp argp = {options, parse_opt, args_doc, doc};

#define RUN_TEST(NAME, FUNC)                                                     \
    printf("Running test %s\n", NAME);                                           \
    tests_total++;                                                               \
    result = FUNC;                                                               \
    if (result == 3)                                                             \
    {                                                                            \
        printf("Test did not pass.\n");                                          \
    }                                                                            \
    else if (result != 0)                                                        \
    {                                                                            \
        printf("Error passing test: (errno %d) %s\n", result, strerror(result)); \
    }                                                                            \
    else                                                                         \
    {                                                                            \
        tests_passed++;                                                          \
        printf("Test passed!\n");                                                \
    }                                                                            \
    printf("\n");

static int reset_file(char *fname)
{
    clear_file(fname);
    resize(fname, 256);
}

int main(int argc, char **argv)
{
    struct arguments arguments;

    /* Default values. */
    arguments.output_file = "-";
    arguments.chrdev_file = "/dev/stack_file_kek_chrdev0";

    /* Parse our arguments; every option seen by parse_opt will
       be reflected in arguments. */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    printf("Testing %s\n", arguments.chrdev_file);

    printf("Preparing the file..");
    reset_file(arguments.chrdev_file);
    printf("File should be ready");

    int result = 0;
    int tests_passed = 0;
    int tests_total = 0;
    RUN_TEST("One element", one_element(arguments.chrdev_file));
    RUN_TEST("Negative element", negative_element(arguments.chrdev_file));
    RUN_TEST("Ten elements", ten_numbers(arguments.chrdev_file));
    RUN_TEST("Min max", min_max_test(arguments.chrdev_file));
    RUN_TEST("256 numbers", lots_of_numbers(arguments.chrdev_file));
    RUN_TEST("Resize", test_resize(arguments.chrdev_file));
    RUN_TEST("Concurrency", test_concurrent_multiple(arguments.chrdev_file));

    // Clear & reset size just in case some info left after the tests.
    reset_file(arguments.chrdev_file);
    printf("\nPassed %d/%d tests\n", tests_passed, tests_total);
    return 0;
}
