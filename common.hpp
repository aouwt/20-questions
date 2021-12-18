#ifndef COMMON_HPP
#define COMMON_HPP
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdint.h>


	#define CERR(func, code) \
		{ \
			err_t status = (code); \
			if (status) { \
				fprintf (stderr, "ERROR: %s exited with error code %i (errno %i). Details below:\n", (func), (code), errno); \
				perror ((func)); \
				exit (1); \
			} \
		}
	
	#define LEN(arr) (sizeof ((arr)) / sizeof ((arr [0])))


	typedef uint_least8_t u8;	typedef int_least8_t s8;
	typedef uint_least16_t u16;	typedef int_least16_t s16;
	typedef uint_least32_t u32;	typedef int_least32_t s32;
	typedef int err_t;
#endif
