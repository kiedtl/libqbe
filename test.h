#ifndef LIBQBE_TEST_H
#define LIBQBE_TEST_H

typedef void (*TestFunc)(void);

#define TEST_BEGIN()
#define TEST_END()

#define test(func, ...) \
	(func(__FILE__, __LINE__, __func__, __VA_ARGS__))

bool eq_u32(char *file, int line, const char *func, u32 expected, u32 got);
bool eq_u64(char *file, int line, const char *func, u64 expected, u64 got);
bool truthy(char *file, int line, const char *func, bool boolean);

#endif
