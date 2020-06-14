/*
 * wpa_supplicant/hostapd / Empty OS specific functions
 * Copyright (c) 2005-2006, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 *
 * This file can be used as a starting point when adding a new OS target. The
 * functions here do not really work as-is since they are just empty or only
 * return an error value. os_internal.c can be used as another starting point
 * or reference since it has example implementation of many of these functions.
 */

#include "includes.h"
#include <kernel/scheduler.h>
#include "os.h"

int abs (int i)
{
	return i < 0 ? -i : i;
}

int isprint (int c)
{
	return ' ' <= c && c <= 126;
}

char *strrchr (const char *s, int c)
{
	while (*s)
	{
		s++;
	}

	while (*--s)
	{
		if (*s == c)
		{
			return (char *) s;
		}
	}

	return 0;
}

char *strdup (const char *s)
{
	size_t size = strlen (s) + 1;

	char *p = (char *) malloc (size);
	// assert (p != 0);

	return strcpy (p, s);
}

int snprintf (char *str, size_t size, const char *format, ...)
{
	va_list ap;
	va_start (ap, format);

	int res = vsnprintf (str, size, format, ap);

	va_end (ap);

	return res;
}

int vsnprintf (char *str, size_t size, const char *format, va_list ap)
{
	c_string_t *Msg = get_new_cstring("");
	cstring_formatv(Msg, format, ap);


	strncpy (str, (const char *) Msg, size);
	str[size-1] = '\0';
	unsigned int len = get_length(Msg);
	return len < size-1 ? len : size-1;
}

int printf (const char *format, ...)
{
	va_list ap;
	va_start (ap, format);

	int res = vprintf (format, ap);

	va_end (ap);

	return res;
}

int vprintf (const char *format, va_list ap)
{
	c_string_t *Msg = get_new_cstring("");
	cstring_formatv(Msg, format, ap);

	char Buffer[1000];
	strncpy (Buffer, (const char *) Msg, sizeof Buffer);
	Buffer[sizeof Buffer-1] = '\0';

	char *pLine;
	char *pLineEnd;
	for (pLine = Buffer; (pLineEnd = strchr (pLine, '\n')) != 0; pLine = pLineEnd+1)
	{
		*pLineEnd = '\0';

		if (*pLine != '\0')
		{
			printf("%s\n", pLine);
		}
	}

	if (*pLine != '\0')
	{
		printf("%s\n", pLine);
	}

	return get_length(Msg);
}

// TODO: this is not quick sort
void qsort (void *base, size_t nmemb, size_t size, int (*compare) (const void *, const void *))
{
	if (nmemb <= 1)
	{
		return;
	}

	// assert (base != 0);
	u8 *array = (u8 *) base;

	// assert (size > 0);
	u8 tmp[size];

	// assert (compare != 0);
	for (unsigned i = 0; i < nmemb-1; i++)
	{
		for (unsigned j = i+1; j < nmemb; j++)
		{
#define ELEM(n)	&array[(n)*size]
			if ((*compare) (ELEM (i), ELEM (j)) > 0)
			{
				memcpy (tmp, ELEM (i), size);
				memcpy (ELEM (i), ELEM (j), size);
				memcpy (ELEM (j), tmp, size);
			}
		}
	}
}

void abort (void)
{
// #ifndef ARM_ALLOW_MULTI_CORE
// 	halt ();
// #else
// 	CMultiCoreSupport::HaltAll ();
// #endif
}

int os_get_random (unsigned char *buf, size_t len)
{

	while (len >= sizeof (u32))
	{
		*(u32 *) buf = get_random_int();

		buf += sizeof (u32);
		len -= sizeof (u32);
	}

	if (len > 0)
	{
		u32 num = get_random_int();

		switch (len)
		{
		case 3:
			buf[2] = (num >> 16) & 0xFF;
			// fall through

		case 2:
			buf[1] = (num >> 8) & 0xFF;
			// fall through

		case 1:
			buf[0] = num & 0xFF;
			break;

		default:
			assert (0);
			break;
		}
	}

	return 0;
}




int os_daemonize(const char *pid_file)
{
	return -1;
}


void os_daemonize_terminate(const char *pid_file)
{
}

size_t os_strlcpy(char *dest, const char *src, size_t siz)
{
	const char *s = src;
	size_t left = siz;

	if (left) {
		/* Copy string up to the maximum size of the dest buffer */
		while (--left != 0) {
			if ((*dest++ = *s++) == '\0')
				break;
		}
	}

	if (left == 0) {
		/* Not enough room for the string; force NUL-termination */
		if (siz != 0)
			*dest = '\0';
		while (*s++)
			; /* determine total src string length */
	}

	return s - src - 1;
}

int os_get_time (struct os_time *t)
{
	unsigned sec, usec;
	// CTimer::Get ()->GetLocalTime (&sec, &usec);

	t->sec = 0;
	t->usec = 0;

	return 0;
}

void os_sleep (os_time_t sec, os_time_t usec)
{
	if (sec)
	{
		task_sleep(sec);
	}

	if (usec)
	{
		task_us_sleep(usec);
	}
}


void * os_zalloc (size_t size)
{
	void *p = malloc (size);
	// assert (p != 0);

	memset (p, 0, size);

	return p;
}

void os_hexdump (const char *title, const void *p, size_t len)
{
// #ifndef NDEBUG
// 	debug_hexdump (p, len, title);
// #endif
}
