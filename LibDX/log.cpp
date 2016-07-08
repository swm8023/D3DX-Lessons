#include <stdarg.h>
#include <stdio.h>
#include <Windows.h>

#include "log.h"

using namespace dx;

void dx::Message(const char *fmt, ...) {
    char buf[4096];

    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buf, 4096, fmt, args);
    buf[n] = 0;
    va_end(args);
    
    ::MessageBoxA(0, buf, 0, 0);
}

void dx::Console(const char*fmt, ...) {
    char buf[4096];

    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(buf, 4096, fmt, args);
    buf[n] = 0;
    va_end(args);

    AllocConsole();
    FILE *stream = NULL;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    printf("%s\n", buf);
}