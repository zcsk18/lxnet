
/*
 * Copyright (C) lcinx
 * lcinx@163.com
 */

#include "crosslib.h"

#ifdef WIN32
#include <windows.h>

struct _tp_integer {
	int isinit;
	LARGE_INTEGER liCounter;
};

static struct _tp_integer s_larget_int = {0};
static __inline int64 __GetSecondCount__() {
	if (0 == s_larget_int.isinit) {
		QueryPerformanceFrequency(&s_larget_int.liCounter);
		s_larget_int.isinit = 1;
	}
	return s_larget_int.liCounter.QuadPart;
}

/* get current millisecond time */
int64 high_millisecond_() {
	double tmp;
	LARGE_INTEGER liCurrent;
	QueryPerformanceCounter(&liCurrent);
	tmp = (double)liCurrent.QuadPart / (double)__GetSecondCount__();
	return (int64)(tmp * 1000);
}

/* get current microsecond time */
int64 high_microsecond_() {
	double tmp;
	LARGE_INTEGER liCurrent;
	QueryPerformanceCounter(&liCurrent);
	tmp = (double)liCurrent.QuadPart / (double)__GetSecondCount__();
	return (int64)(tmp * 1000000);
}

/* get current nanosecond time */
int64 high_nanosecond_() {
	double tmp;
	LARGE_INTEGER liCurrent;
	QueryPerformanceCounter(&liCurrent);
	tmp = (double)liCurrent.QuadPart / (double)__GetSecondCount__();
	return (int64)(tmp * 1000000000);
}

void delay_delay(unsigned int millisecond) {
	Sleep(millisecond);
}

int get_cpu_num() {
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
}

#else

#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef __APPLE__

#include <mach/mach_time.h>
#define CLOCK_MONOTONIC 0

struct _base_time_info {
	int isinit;
	mach_timebase_info_data_t timebase;
	double ratio_v;
};

static struct _base_time_info s_basetime = {0};
static inline double get_timebase_ratio_v() {
	if (0 == s_basetime.isinit) {
		mach_timebase_info(&s_basetime.timebase);
		s_basetime.ratio_v = (double)s_basetime.timebase.numer / (double)s_basetime.timebase.denom;
		s_basetime.isinit = 1;
	}
	return s_basetime.ratio_v;
}

static int clock_gettime(int clk_id, struct timespec *t) {
	int64 time = mach_absolute_time();
	int64 all_nsec = (int64)((double)time * get_timebase_ratio_v());

	t->tv_sec = all_nsec / 1000000000;
	t->tv_nsec = all_nsec % 1000000000;
	return 0;
}

#endif


/* get current millisecond time */
int64 high_millisecond_() {
	struct timespec ts;
	int64 res;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	res = (int64)(ts.tv_sec * 1000) + (int64)(ts.tv_nsec / 1000000);
	return res;
}

/* get current microsecond time */
int64 high_microsecond_() {
	struct timespec ts;
	int64 res;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	res = (int64)(ts.tv_sec * 1000000) + (int64)(ts.tv_nsec / 1000);
	return res;
}

/* get current nanosecond time */
int64 high_nanosecond_() {
	struct timespec ts;
	int64 res;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	res = (int64)(ts.tv_sec * 1000000000) + (int64)ts.tv_nsec;
	return res;
}

void delay_delay(unsigned int millisecond) {
	usleep(millisecond * 1000);
}

int get_cpu_num() {
	return sysconf(_SC_NPROCESSORS_CONF);
}

#endif

