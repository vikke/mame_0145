/***************************************************************************

    astring.h

    Allocated string manipulation functions.

****************************************************************************

    Copyright Aaron Giles
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
          notice, this list of conditions and the following disclaimer in
          the documentation and/or other materials provided with the
          distribution.
        * Neither the name 'MAME' nor the names of its contributors may be
          used to endorse or promote products derived from this software
          without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY AARON GILES ''AS IS'' AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL AARON GILES BE LIABLE FOR ANY DIRECT,
    INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
    STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
    IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.

***************************************************************************/

#pragma once

#ifndef __ASTRING_H__
#define __ASTRING_H__

#include <stdarg.h>
#include <ctype.h>
#include "osdcomm.h"



//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// derived class for C++
class astring
{
public:
	// simple construction/destruction
	astring() { init(); }
	~astring();

	// construction with copy
	astring(const char *string) { init().cpy(string); }
	astring(const char *string, int length) { init().cpy(string, length); }
	astring(const char *str1, const char *str2) { init().cpy(str1).cat(str2); }
	astring(const char *str1, const char *str2, const char *str3) { init().cpy(str1).cat(str2).cat(str3); }
	astring(const char *str1, const char *str2, const char *str3, const char *str4) { init().cpy(str1).cat(str2).cat(str3).cat(str4); }
	astring(const char *str1, const char *str2, const char *str3, const char *str4, const char *str5) { init().cpy(str1).cat(str2).cat(str3).cat(str4).cat(str5); }
	astring(const astring &string) { init().cpy(string); }
	astring(const astring &string, int start, int count = -1) { init().cpysubstr(string, start, count); }

	// assignment operators
	astring &operator=(const char *string) { return cpy(string); }
	astring &operator=(const astring &string) { return cpy(string); }

	// concatenation operators
	astring& operator+=(const astring &string) { return cat(string); }
	friend astring operator+(const astring &lhs, const astring &rhs) { return astring(lhs) += rhs; }
	friend astring operator+(const astring &lhs, const char *rhs) { return astring(lhs) += rhs; }
	friend astring operator+(const char *lhs, const astring &rhs) { return astring(lhs) += rhs; }

	// comparison operators
	bool operator==(const char *string) const { return (cmp(string) == 0); }
	bool operator==(const astring &string) const { return (cmp(string) == 0); }
	bool operator!=(const char *string) const { return (cmp(string) != 0); }
	bool operator!=(const astring &string) const { return (cmp(string) != 0); }
	bool operator<(const char *string) const { return (cmp(string) < 0); }
	bool operator<(const astring &string) const { return (cmp(string) < 0); }
	bool operator<=(const char *string) const { return (cmp(string) <= 0); }
	bool operator<=(const astring &string) const { return (cmp(string) <= 0); }
	bool operator>(const char *string) const { return (cmp(string) > 0); }
	bool operator>(const astring &string) const { return (cmp(string) > 0); }
	bool operator>=(const char *string) const { return (cmp(string) >= 0); }
	bool operator>=(const astring &string) const { return (cmp(string) >= 0); }

	// character access operators
	char operator[](int index) const { return (index < len()) ? m_text[index] : 0; }

	// implicit boolean conversion operators
	operator bool() { return m_text[0] != 0; }
	operator bool() const { return m_text[0] != 0; }

	// C string conversion operators and helpers
	operator const char *() const { return m_text; }
	const char *cstr() const { return m_text; }
	char *stringbuffer(int size) { ensure_room(size); return m_text; }

	// buffer management
	astring &reset() { return cpy(""); }
	astring &expand(int length) { ensure_room(length); return *this; }

	// length query
	int len() const { return strlen(m_text); }

	// copy helpers
	astring &cpy(const char *src, int count);
	astring &cpysubstr(const astring &src, int start, int count = -1);
	astring &cpy(const astring &src) { return cpy(src.cstr(), src.len()); }
	astring &cpy(const char *src) { return cpy(src, strlen(src)); }

	// insertion helpers
	astring &ins(int insbefore, const char *src, int count);
	astring &inssubstr(int insbefore, const astring &src, int start, int count = -1);
	astring &ins(int insbefore, const astring &src) { return ins(insbefore, src.cstr(), src.len()); }
	astring &ins(int insbefore, const char *src) { return ins(insbefore, src, strlen(src)); }

	// concatenation helpers (== insert at end)
	astring &cat(const char *src, int count) { return ins(-1, src, count); }
	astring &catsubstr(const astring &src, int start, int count = -1) { return inssubstr(-1, src, start, count); }
	astring &cat(const astring &src) { return ins(-1, src.cstr(), src.len()); }
	astring &cat(const char *src) { return ins(-1, src, strlen(src)); }
	astring &cat(char ch) { return ins(-1, &ch, 1); }

	// substring helpers
	astring &substr(int start, int count = -1);
	astring &del(int start, int count = -1);

	// formatted string helpers
	int vprintf(const char *format, va_list args);
	int catvprintf(const char *format, va_list args);
	int printf(const char *format, ...) { va_list ap; va_start(ap, format); int result = this->vprintf(format, ap); va_end(ap); return result; }
	int catprintf(const char *format, ...) { va_list ap; va_start(ap, format); int result = catvprintf(format, ap); va_end(ap); return result; }
	astring &format(const char *format, ...) { va_list ap; va_start(ap, format); this->vprintf(format, ap); va_end(ap); return *this; }
	astring &catformat(const char *format, ...) { va_list ap; va_start(ap, format); catvprintf(format, ap); va_end(ap); return *this; }

	// comparison helpers
	int cmp(const char *str2, int count) const;
	int cmpsubstr(const astring &str2, int start, int count = -1) const;
	int cmp(const astring &str2) const { return cmp(str2.cstr(), str2.len()); }
	int cmp(const char *str2) const { return cmp(str2, strlen(str2)); }

	// case-insensitive comparison helpers
	int icmp(const char *str2, int count) const;
	int icmpsubstr(const astring &str2, int start, int count = -1) const;
	int icmp(const astring &str2) const { return icmp(str2.cstr(), str2.len()); }
	int icmp(const char *str2) const { return icmp(str2, strlen(str2)); }

	// character searching helpers
	int chr(int start, int ch) const;
	int rchr(int start, int ch) const;

	// string searching/replacing helpers
	int find(int start, const char *search) const;
	int find(const char *search) const { return find(0, search); }
	int replace(int start, const char *search, const char *replace);
	int replace(const char *search, const char *_replace) { return replace(0, search, _replace); }

	// misc utilities
	astring &delchr(int ch);
	astring &replacechr(int ch, int newch);
	astring &makeupper();
	astring &makelower();
	astring &trimspace();

private:
	// internal helpers
	astring &init();
	char *safe_string_base(int start) const;
	bool ensure_room(int length);
	void normalize_substr(int &start, int &count, int length) const;

	// internal state
	char *			m_text;
	int				m_alloclen;
	char			m_smallbuf[64];
};


#endif /* __ASTRING_H__ */
