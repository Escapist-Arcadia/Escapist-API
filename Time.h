#pragma once
#include"Preprocessor.h"
#include<ctime>
#include<cassert>

class Date
{
private:

protected:
	int year, month, day;
	bool isLeapYear;

public:
	static const int MaximumMonthDays[2][13];
	static const int MaximumYearDays[2][13];
	static bool IsLeapYear(const int& inputYear) {
		return (inputYear % 4 == 0 && inputYear % 100 != 0) || (inputYear % 400 == 0);
	}
	static int GetDayInWeek(const int& inputYear, const int& inputMonth, const int& inputDay) {
		return (inputDay + 2 * inputMonth + 3 * (inputMonth + 1) / 5 + inputYear + inputYear / 4 - inputYear / 100 + inputYear / 400 + 1) % 7;
	}

	Date() = delete;
	Date(const int& inputYear, const int& inputMonth, const int& inputDay) {
		assert(month >= 1 && month <= 12);
		assert(day >= 1 && day <= Date::MaximumMonthDays[isLeapYear][month]);
	}
	Date(time_t ts, bool haveTimeZone = true) {
		if (ts)
		{
			if (haveTimeZone)
			{
				long tz(0);
				int isDST(0);
				::_tzset();
				::_get_timezone(&tz);
				ts -= tz;
				::_get_dstbias(&tz);
				ts -= tz;
			}

			ts /= 86400;

			year = 1970 + (4 * (ts / 1461));
			ts %= 1461;

			isLeapYear = Date::IsLeapYear(year);
			int skipDays = (isLeapYear ? 366 : 365);
			while (ts > skipDays)
			{
				++year;
				ts -= skipDays;

				isLeapYear = Date::IsLeapYear(year);
				skipDays = (isLeapYear ? 366 : 365);
			}

			day = ts;
			month = 1;
			while (day > Date::MaximumMonthDays[isLeapYear][month])
			{
				++month;
				day -= Date::MaximumMonthDays[isLeapYear][month];
			}
		}
	}
	Date(const Date& other) :year(other.year), month(other.month), day(other.day) {

	}

	int GetYear()const {
		return year;
	}
	int GetMonth()const {
		return month;
	}
	int GetDay()const {
		return day;
	}
	bool IsLeapYear()const {
		return isLeapYear;
	}

	void SetYear(const int& input) {
		year = input;
		isLeapYear = Date::IsLeapYear(year);
	}
	void SetMonth(const int& input) {
		assert(input >= 1 && input <= 12);
		month = input;
	}
	void SetDay(const int& input) {
		assert(input >= 1 && input <= Date::MaximumMonthDays[isLeapYear][month]);
		day = input;
	}
	void AddDays(const short& newDay)
	{
		day += newDay;

		while (day > Date::MaximumMonthDays[isLeapYear][month])
		{
			day -= Date::MaximumMonthDays[isLeapYear][month];
			++month;

			if (month == 13)
			{
				month = 1;
				++year;
				isLeapYear = Date::IsLeapYear(year);
			}
		}
	}
	void AddMonths(const short& newMonth)
	{
		month += newMonth;

		while (month > 12)
		{
			month -= 12;
			++year;
			isLeapYear = Date::IsLeapYear(year);
		}

		if (day > Date::MaximumMonthDays[isLeapYear][month])
			day = Date::MaximumMonthDays[isLeapYear][month];
	}
	void AddYears(const short& newYear)
	{
		year += newYear;
		isLeapYear = Date::IsLeapYear(year);

		if (!isLeapYear && month == 2 && day == 29)
			day = 28;
	}

	void SubtractDays(const int& days)
	{
		day -= days;

		while (day <= 0)
		{
			--month;
			if (month == 0)
			{
				month = 12;
				--year;
				isLeapYear = Date::IsLeapYear(year);
			}

			day += Date::MaximumMonthDays[isLeapYear][month];
		}
	}
	void SubtractMonths(const int& newMonth)
	{
		month -= newMonth;

		while (month <= 0)
		{
			month += 12;
			--year;
			isLeapYear = Date::IsLeapYear(year);
		}

		if (day > Date::MaximumMonthDays[isLeapYear][month])
			day = Date::MaximumMonthDays[isLeapYear][month];
	}
	void SubtractYears(const int& newYears)
	{
		year -= newYears;
		isLeapYear = Date::IsLeapYear(year);

		if (!isLeapYear && month == 2 && day == 29)
			day = 28;
	}

	int Compare(const int& leftYear, const int& leftMonth, const int& leftDay, const int& rightYear, const int& rightMonth, const int& rightDay)
	{
		if (leftYear == rightYear)
			if (leftMonth == rightMonth)
				if (leftDay == rightDay)
					return 0;
				else return leftDay < rightDay ? -1 : 1;
			else return leftMonth < rightMonth ? -1 : 1;
		else return leftYear < rightYear ? -1 : 1;
	}
	int Compare(const Date& left, const Date& right)
	{
		return Date::Compare(
			left.year, left.month, left.day,
			right.year, right.month, right.day
		);
	}
	int CompareTo(const int& otherYear, const int& otherMonth, const int& otherDay, const int& otherHour, const int& otherMinute, const int& otherSecond)
	{
		return Date::Compare(
			year, month, day,
			otherYear, otherMonth, otherDay
		);
	}
	int CompareTo(const Date& other)
	{
		return Date::Compare(
			year, month, day,
			other.year, other.month, other.day
		);
	}
};

const int Date::MaximumMonthDays[2][13] =
{
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
};

const int Date::MaximumYearDays[2][13] =
{
	{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
	{0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}
};

class Time
{
private:

protected:
	int hour, minute, second;

public:
	Time() = delete;
	Time(const int& inputHour, const int& inputMinute, const int& inputSecond)
		:hour(inputHour), minute(inputMinute), second(inputSecond)
	{
		assert(hour >= 0 && hour <= 23);
		assert(minute >= 0 && minute <= 59);
		assert(second >= 0 && second <= 59);
	}

	Time(time_t ts, bool haveTimeZone)
	{
		if (ts)
		{
			if (haveTimeZone)
			{
				long tz(0);
				int isDST(0);
				::_tzset();
				::_get_timezone(&tz);
				ts -= tz;
				::_get_dstbias(&tz);
				ts -= tz;
			}

			second = ts % 60;
			ts /= 60;

			minute = ts % 60;
			ts /= 60;

			hour = ts % 24;
			ts /= 24;
		}
	}

	Time(const Time& other)
		:hour(other.hour), minute(other.minute), second(other.second) {

	}

	inline int GetHour() const { return hour; }

	inline int GetMinute() const { return minute; }

	inline int GetSecond() const { return second; }

	inline void SetHour(const int& input) { hour = input; assert(hour >= 0 && hour <= 23); }

	inline void SetMinute(const int& input) { minute = input; assert(minute >= 0 && minute <= 59); }

	inline void SetSecond(const int& input) { second = input; assert(second >= 0 && second <= 59); }

	void AddSeconds(const int& seconds)
	{
		second += seconds;
		AddMinutes(second / 60);
		second %= 60;
	}
	void AddMinutes(const int& minutes)
	{
		minute += minutes;
		AddHours(minute / 60);
		minute %= 60;
	}
	void AddHours(const int& hours)
	{
		hour += hours;
		hour %= 24;
	}

	void SubtractSeconds(const int& seconds)
	{
		second = seconds - second;
		SubtractMinutes(second / 60 + 1);
		second = 60 - (second % 60);
	}
	void SubtractMinutes(const int& minutes)
	{
		minute = minutes - minute;
		SubtractHours(minute / 60 + 1);
		minute = 60 - (minute % 60);
	}
	void SubtractHours(const int& hours)
	{
		hour = hours - hour;
		hour = 24 - (hour % 24);
	}

	int Compare(const int& leftHour, const int& leftMinute, const int& leftSecond, const int& rightHour, const int& rightMinute, const int& rightSecond)
	{
		if (leftHour == rightHour)
			if (leftMinute == rightMinute)
				if (leftSecond == rightSecond)
					return 0;
				else return leftSecond < rightSecond ? -1 : 1;
			else return leftMinute < rightMinute ? -1 : 1;
		else return leftHour < rightHour ? -1 : 1;
	}
	int Compare(const Time& left, const Time& right)
	{
		return Compare(
			left.hour, left.minute, left.second,
			right.hour, right.minute, right.second
		);
	}
	int CompareTo(const int& otherHour, const int& otherMinute, const int& otherSecond)
	{
		return Compare(
			hour, minute, second,
			otherHour, otherMinute, otherSecond
		);
	}
	int CompareTo(const Time& other)
	{
		return Compare(
			hour, minute, second,
			other.hour, other.minute, other.second
		);
	}
};

class Clock
{
private:

protected:
	int hour, minute, second;

public:
	Clock() = delete;
	Clock(const int& inputHour, const int& inputMinute, const int& inputSecond)
		:hour(inputHour), minute(inputMinute), second(inputSecond)
	{
		assert(minute >= 0 && minute <= 59);
		assert(second >= 0 && second <= 59);
	}

	Clock(const Clock& other)
		:hour(other.hour), minute(other.minute), second(other.second)
	{
	}

	inline int GetHour() const { return hour; }

	inline int GetMinute() const { return minute; }

	inline int GetSecond() const { return second; }

	inline void SetHour(const int& input) { hour = input; }

	inline void SetMinute(const int& input) { minute = input; assert(minute >= 0 && minute <= 59); }

	inline void SetSecond(const int& input) { second = input; assert(second >= 0 && second <= 59); }

	void AddSeconds(const int& seconds)
	{
		second += seconds;
		AddMinutes(second / 60);
		second %= 60;
	}
	void AddMinutes(const int& minutes)
	{
		minute += minutes;
		AddHours(minute / 60);
		minute %= 60;
	}
	void AddHours(const int& hours)
	{
		hour += hours;
	}

	void SubtractSeconds(const int& seconds)
	{
		second = seconds - second;
		SubtractMinutes(second / 60 + 1);
		second = 60 - (second % 60);
	}
	void SubtractMinutes(const int& minutes)
	{
		minute = minutes - minute;
		SubtractHours(minute / 60 + 1);
		minute = 60 - (minute % 60);
	}
	void SubtractHours(const int& hours)
	{
		hour = hours - hour;
	}

	int Compare(const int& leftHour, const int& leftMinute, const int& leftSecond, const int& rightHour, const int& rightMinute, const int& rightSecond)
	{
		if (leftHour == rightHour)
			if (leftMinute == rightMinute)
				if (leftSecond == rightSecond)
					return 0;
				else return leftSecond < rightSecond ? -1 : 1;
			else return leftMinute < rightMinute ? -1 : 1;
		else return leftHour < rightHour ? -1 : 1;
	}
	int Compare(const Clock& left, const Clock& right)
	{
		return Compare(
			left.hour, left.minute, left.second,
			right.hour, right.minute, right.second
		);
	}
	int CompareTo(const int& otherHour, const int& otherMinute, const int& otherSecond)
	{
		return Compare(
			hour, minute, second,
			otherHour, otherMinute, otherSecond
		);
	}
	int CompareTo(const Clock& other)
	{
		return Compare(
			hour, minute, second,
			other.hour, other.minute, other.second
		);
	}
};

class Calendar
{
private:

protected:
	int year, month, day, hour, minute, second;
	bool isLeapYear;

public:
	Calendar() = delete;
	Calendar(const int& inputYear, const int& inputMonth, const int& inputDay, const int& inputHour, const int& inputMinute, const int& inputSecond)
		:year(inputYear), month(inputMonth), day(inputDay), hour(inputHour), minute(inputMinute), second(inputSecond), isLeapYear(Date::IsLeapYear(year))
	{
		assert(month >= 1 && month <= 12);
		assert(day >= 1 && day <= Date::MaximumMonthDays[isLeapYear][month]);
		assert(hour >= 0 && hour <= 23);
		assert(minute >= 0 && minute <= 59);
		assert(second >= 0 && second <= 59);
	}

	Calendar(time_t ts, bool haveTimeZone)
	{
		if (ts)
		{
			if (haveTimeZone)
			{
				long tz(0);
				int isDST(0);
				::_tzset();
				::_get_timezone(&tz);
				ts -= tz;
				::_get_dstbias(&tz);
				ts -= tz;
			}

			second = ts % 60;
			ts /= 60;

			minute = ts % 60;
			ts /= 60;

			hour = ts % 24;
			ts /= 24;

			year = 1970 + (4 * (ts / 1461));
			ts %= 1461;

			isLeapYear = Date::IsLeapYear(year);
			int skipDays = (isLeapYear ? 366 : 365);
			while (ts > skipDays)
			{
				++year;
				ts -= skipDays;

				isLeapYear = Date::IsLeapYear(year);
				skipDays = (isLeapYear ? 366 : 365);
			}

			day = ts;
			month = 1;
			while (day > Date::MaximumMonthDays[isLeapYear][month])
			{
				++month;
				day -= Date::MaximumMonthDays[isLeapYear][month];
			}
		}
	}

	Calendar(const Calendar& other)
		:year(other.year), month(other.month), day(other.day), hour(other.hour), minute(other.minute), second(other.second)
	{}

	inline int GetYear() const { return year; }

	inline int GetMonth() const { return month; }

	inline int GetDay() const { return day; }

	inline int GetHour() const { return hour; }

	inline int GetMinute() const { return minute; }

	inline int GetSecond() const { return second; }

	inline bool IsLeapYear() const { return isLeapYear; }

	inline void SetYear(const int& input) { year = input; isLeapYear = Date::IsLeapYear(year); }

	inline void SetMonth(const int& input) { month = input; assert(month >= 1 && month <= 12); }

	inline void SetDay(const int& input) { day = input; assert(day >= 1 && day <= Date::MaximumMonthDays[isLeapYear][month]); }

	inline void SetHour(const int& input) { hour = input; assert(hour >= 0 && hour <= 23); }

	inline void SetMinute(const int& input) { minute = input; assert(minute >= 0 && minute <= 59); }

	inline void SetSecond(const int& input) { second = input; assert(second >= 0 && second <= 59); }

	void AddSeconds(const int& seconds)
	{
		second += seconds;
		AddMinutes(second / 60);
		second %= 60;
	}
	void AddMinutes(const int& minutes)
	{
		minute += minutes;
		AddHours(minute / 60);
		minute %= 60;
	}
	void AddHours(const int& hours)
	{
		hour += hours;
		AddDays(hour / 24);
		hour %= 24;
	}
	void AddDays(const short& newDay)
	{
		day += newDay;

		while (day > Date::MaximumMonthDays[isLeapYear][month])
		{
			day -= Date::MaximumMonthDays[isLeapYear][month];
			++month;

			if (month == 13)
			{
				month = 1;
				++year;
				isLeapYear = Date::IsLeapYear(year);
			}
		}
	}
	void AddMonths(const short& newMonth)
	{
		month += newMonth;

		while (month > 12)
		{
			month -= 12;
			++year;
			isLeapYear = Date::IsLeapYear(year);
		}

		if (day > Date::MaximumMonthDays[isLeapYear][month])
			day = Date::MaximumMonthDays[isLeapYear][month];
	}
	void AddYears(const short& newYear)
	{
		year += newYear;
		isLeapYear = Date::IsLeapYear(year);

		if (!isLeapYear && month == 2 && day == 29)
			day = 28;
	}
	void SubtractSeconds(const int& seconds)
	{
		second = seconds - second;
		SubtractMinutes(second / 60 + 1);
		second = 60 - (second % 60);
	}
	void SubtractMinutes(const int& minutes)
	{
		minute = minutes - minute;
		SubtractHours(minute / 60 + 1);
		minute = 60 - (minute % 60);
	}
	void SubtractHours(const int& hours)
	{
		hour = hours - hour;
		SubtractDays(hour / 24 + 1);
		hour = 24 - (hour % 24);
	}
	void SubtractDays(const int& days)
	{
		day -= days;

		while (day <= 0)
		{
			--month;
			if (month == 0)
			{
				month = 12;
				--year;
				isLeapYear = Date::IsLeapYear(year);
			}

			day += Date::MaximumMonthDays[isLeapYear][month];
		}
	}
	void SubtractMonths(const int& newMonth)
	{
		month -= newMonth;

		while (month <= 0)
		{
			month += 12;
			--year;
			isLeapYear = Date::IsLeapYear(year);
		}

		if (day > Date::MaximumMonthDays[isLeapYear][month])
			day = Date::MaximumMonthDays[isLeapYear][month];
	}
	void SubtractYears(const int& newYears)
	{
		year -= newYears;
		isLeapYear = Date::IsLeapYear(year);

		if (!isLeapYear && month == 2 && day == 29)
			day = 28;
	}

	int Compare(const int& leftYear, const int& leftMonth, const int& leftDay, const int& leftHour, const int& leftMinute, const int& leftSecond, const int& rightYear, const int& rightMonth, const int& rightDay, const int& rightHour, const int& rightMinute, const int& rightSecond)
	{
		if (leftYear == rightYear)
			if (leftMonth == rightMonth)
				if (leftDay == rightDay)
					if (leftHour == rightHour)
						if (leftMinute == rightMinute)
							if (leftSecond == rightSecond)
								return 0;
							else return leftSecond < rightSecond ? -1 : 1;
						else return leftMinute < rightMinute ? -1 : 1;
					else return leftHour < rightHour ? -1 : 1;
				else return leftDay < rightDay ? -1 : 1;
			else return leftMonth < rightMonth ? -1 : 1;
		else return leftYear < rightYear ? -1 : 1;
	}
	int Compare(const Calendar& left, const Calendar& right)
	{
		return Compare(
			left.year, left.month, left.day, left.hour, left.minute, left.second,
			right.year, right.month, right.day, right.hour, right.minute, right.second
		);
	}
	int CompareTo(const int& otherYear, const int& otherMonth, const int& otherDay, const int& otherHour, const int& otherMinute, const int& otherSecond)
	{
		return Compare(
			year, month, day, hour, minute, second,
			otherYear, otherMonth, otherDay, otherHour, otherMinute, otherSecond
		);
	}
	int CompareTo(const Calendar& other)
	{
		return Compare(
			year, month, day, hour, minute, second,
			other.year, other.month, other.day, other.hour, other.minute, other.second
		);
	}
};