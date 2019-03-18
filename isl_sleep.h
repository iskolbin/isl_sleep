/*

 isl_sleep - v.0.0.1 - public domain cross-platform sleep function
                       with millisecond precision 

 author: Ilya Kolbin (iskolbin@gmail.com)
 url: github.com/iskolbin/isl_sleep

 This code is based on implementation by cyi823:
 https://www.c-plusplus.net/forum/topic/109539/usleep-unter-windows
 and raylib:
 https://github.com/raysan5/raylib/blob/master/src/core.c#L2978
 
 LICENSE

 See end of file for license information.

*/

#ifndef ISL_SLEEP_H_
#define ISL_SLEEP_H_

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__) || defined(__EMSCRIPTEN__)
#include <sys/time.h>           // Required for: timespec, nanosleep(), select() - POSIX
#elif defined(__APPLE__)
#include <unistd.h>             // Required for: usleep()
#else
#include <time.h>
#endif

#ifndef ISL_SLEEP_STATIC
#define ISL_SLEEP_API extern
#else
#define ISL_SLEEP_API static
#endif

ISL_SLEEP_API void isl_sleep(unsigned int ms);

#ifdef ISL_SLEEP_IMPLEMENTATION

void isl_sleep(unsigned int ms) {
#if defined(_WIN32)
	HANDLE timer;
	LARGE_INTEGER ft;
	due.QuadPart = -(10000 * (__int64)ms);
	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
#elif defined(__linux__) || defined(__EMSCRIPTEN__)
	struct timespec req = {
		.tv_sec = ms/1000;
		.tv_nsec = (ms%1000)*1000000L;
	};
	while (nanosleep(&req, &req) == -1) continue;
#elif defined(__APPLE__)
	usleep(1000*ms);
#else
	clock_t finish_clocks = clock() + (ms * CLOCKS_PER_SEC / 1000);
	while (clock() < finish_clocks) continue;
#endif
}

#endif // ISL_SLEEP_IMPLEMENTATION
#endif // ISL_SLEEP_H

/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2019 Ilya Kolbin
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/
