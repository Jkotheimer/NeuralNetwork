#include <cstdarg>
#include <stdio.h>
#include <iostream>

int err_code = 1;

void _err(const char* msg, ...) {
	va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
	std::cerr << std::endl;
    va_end(args);
	exit(err_code);
}
