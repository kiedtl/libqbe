#include <stdio.h>

#include "libqbe.h"
#include "test.h"

#define TEST
#include "add.c"
#include "himul.c"
#undef TEST

const char *TMP_FILE = ".test_output";

FILE *f_err        = NULL;
u32 total_asserts  = 0;
u32 failed_asserts = 0;
u32 total_tests    = 0;
u32 failed_tests   = 0;

struct {
	char *name;
	TestFunc *set;
	u32 count;
} test_func_sets[] = {
#define SET(name, fnset) { name, (TestFunc *)&fnset, ARRAY_LEN(fnset) }
	SET("add.c",   add_test_funcs),
	SET("himul.c", himul_test_funcs),
#undef SET
};

int
main(void)
{
	f_err = fopen(TMP_FILE, "w");
	for (u32 i = 0; i < ARRAY_LEN(test_func_sets); ++i) {
		printf("-- Beginning test suit %s (%u tests)\n",
				test_func_sets[i].name, test_func_sets[i].count);
		printf("  ");

		for (u32 j = 0; j < test_func_sets[i].count; ++j) {
			++total_tests;

			u32 prv_failed = failed_asserts;
			(test_func_sets[i].set[j])();
			if (prv_failed < failed_asserts) {
				++failed_tests;
				printf("F");
			} else {
				printf(".");
			}
		}

		printf("\n");
	}

	printf("---- Tests finished with %u assertions ----\n", total_asserts);
	printf("  %u total, %u passed, %u failed.\n\n",
			total_tests, total_tests - failed_tests, failed_tests);

	fclose(f_err);
	f_err = fopen(TMP_FILE, "r");
	int c;
	while ((c = getc(f_err)) != EOF)
		putchar(c);
	fclose(f_err);
	remove(TMP_FILE);
}

// -----------------------------------------------------------------------------


void
eq_u32(char *file, int line, const char *func, u32 expected, u32 got)
{
	++total_asserts;

	if (expected != got) {
		++failed_asserts;
		fprintf(f_err, "FAIL: %s:%s:%d: Expected %u, got %u\n",
				file, func, line, expected, got);
	}
}

void
eq_u64(char *file, int line, const char *func, u64 expected, u64 got)
{
	++total_asserts;

	if (expected != got) {
		++failed_asserts;
		fprintf(f_err, "FAIL: %s:%s:%d: Expected %llu, got %llu\n",
				file, func, line, expected, got);
	}
}
