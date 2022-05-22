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
static char args_doc[] = "CHRDEV_FILE1 CHRDEV_FILE2";

/* The options we understand. */
static struct argp_option options[] = {
  {"output",   'o', "FILE", 0,
   "Output to FILE instead of standard output" },
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
  char *chrdev_file1;
  char *chrdev_file2;
  char *output_file;
};

/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state) {
  /* Get the input argument from argp_parse, which we
	 know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'o':
      arguments->output_file = arg;
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 2)
        /* Too many arguments. */
        argp_usage (state);
	  else if (state->arg_num == 0)
	  	/* First chrdev file */
      	arguments->chrdev_file1 = arg;
	  else if (state->arg_num == 1)
	  	/* Second chrdev file */
      	arguments->chrdev_file2 = arg;
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 0)
        /* Not enough arguments. */
        argp_usage (state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}
//Character device file to test

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };

#define RUN_TEST(NAME, FUNC) \
  printf("Running test %s\n", NAME); \
  tests_total++; \
  result = FUNC; \
  if (result == 3) { \
    printf("Test did not pass.\n"); \
  } else if (result != 0) { \
    printf("Error passing test: (errno %d) %s\n", result, strerror(result)); \
  } else { \
    tests_passed++; \
    printf("Test passed!\n"); \
  } \
  printf("\n"); \

int main (int argc, char **argv) {
  struct arguments arguments;

  /* Default values. */
  arguments.output_file = "-";
  arguments.chrdev_file1 = "/dev/stack_file_kek_chrdev0";
  arguments.chrdev_file2 = "/dev/stack_file_kek_chrdev1";

  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  printf ("Testing %s with %s\n", arguments.chrdev_file1, arguments.chrdev_file2);

  int result = 0;
  int tests_passed = 0;
  int tests_total = 0;
  RUN_TEST("One element", one_element(arguments.chrdev_file1));
  RUN_TEST("Negative element", negative_element(arguments.chrdev_file1));
  RUN_TEST("Ten elements", ten_numbers(arguments.chrdev_file1));
  RUN_TEST("Min max", min_max_test(arguments.chrdev_file1));
  RUN_TEST("256 numbers", lots_of_numbers(arguments.chrdev_file1));
  RUN_TEST("Double open", double_open(arguments.chrdev_file1, arguments.chrdev_file2));
  RUN_TEST("Resize", test_resize(arguments.chrdev_file1));

  printf("\nPassed %d/%d tests\n", tests_passed, tests_total);
  return 0;
}
