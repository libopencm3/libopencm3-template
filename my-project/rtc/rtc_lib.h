/* SPDX-License-Identifier: GPL-2.0 */

#ifndef _RTC_LIB_H
#define _RTC_LIB_H

#include <types.h>

struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

typedef s64 time64_t;

static inline char  is_leap_year(unsigned int year);
int rtc_month_days(unsigned int month, unsigned int year);
int rtc_year_days(unsigned int day, unsigned int month, unsigned int year);
void rtc_time64_to_tm(time64_t time, struct rtc_time *tm);
int rtc_valid_tm(struct rtc_time *tm);
time64_t mktime64(const unsigned int year0, const unsigned int mon0,
		const unsigned int day, const unsigned int hour,
		const unsigned int min, const unsigned int sec);
time64_t rtc_tm_to_time64(struct rtc_time *tm);

#endif /* _RTC_LIB_H */