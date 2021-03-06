/*$Id: md.h,v 25.96 2006/08/28 05:45:51 al Exp $ -*- C++ -*-
 * Copyright (C) 2001 Albert Davis
 * Author: Albert Davis <aldavis@ieee.org>
 *
 * This file is part of "Gnucap", the Gnu Circuit Analysis Package
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *------------------------------------------------------------------
 * Machine dependent, configuration, and standard includes
 */
//testing=trivial 2006.07.17
#ifndef MD_H_INCLUDED
#define MD_H_INCLUDED
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#define _SCL_SECURE_NO_WARNINGS
/*--------------------------------------------------------------------------*/
/* autoconf stuff */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
/*--------------------------------------------------------------------------*/
/* std collection of includes */
// system
#include <new>
#include <cstdarg>
#include <cassert>
#include <cfloat>
#include <cmath>
#include <climits>
#include <limits>
#include <cstdio>
#include <cerrno>
#include <csetjmp>
#include <csignal>
#include <iostream>
// types
#include <complex>
#include <string>
// containers
#include <list>
#include <vector>
#include <queue>
#include <map>
// algorithms
#include <typeinfo>
#include <algorithm>
#include <ctime>
/* usual but non-standard (POSIX??) collection of includes */
//#include <unistd.h>	/* chdir, access, getcwd */
#include <fcntl.h>	/* old style unix files */

#define __func__ __FUNCTION__
#define M_PI 3.14159265
#define M_PI_2 3.14159265 * 2
#define M_PI_4 3.14159265 * 4
#define M_SQRT2 1.41421356
#define M_E 2.71828183
#define F_OK 0

/*--------------------------------------------------------------------------*/
/* constants related to memory size, word size, etc */
enum {
  BUFLEN = 256,
  BIGBUFLEN = 4096
};
/*--------------------------------------------------------------------------*/
/* user interface preferences */
#define I_PROMPT "gnucap> "
#define CKT_PROMPT ">"
#define ANTI_COMMENT "*>"
/*--------------------------------------------------------------------------*/
#if defined(__WIN32__)
#define	ENDDIR		"/\\"
#define PATHSEP		';'
#define SYSTEMSTARTFILE	"gnucap.rc"
#define SYSTEMSTARTPATH	OS::getenv("PATH")
#define USERSTARTFILE	"gnucap.rc"
#define	USERSTARTPATH	OS::getenv("HOME")
#define STEPFILE   	"/tmp/SXXXXXX"
#define PLOTFILE    	"gnucap.plot"
#define SHELL		OS::getenv("COMSPEC")
/*--------------------------------------------------------------------------*/
#else
#define	ENDDIR		"/"
#define PATHSEP		':'
#define SYSTEMSTARTFILE	"gnucap.rc"
#define SYSTEMSTARTPATH	OS::getenv("PATH")
#define USERSTARTFILE	".gnucaprc"
#define	USERSTARTPATH	OS::getenv("HOME")
#define STEPFILE   	"/tmp/SXXXXXX"
#define PLOTFILE    	"gnucap.plot"
#define SHELL		OS::getenv("SHELL")
#endif
/*--------------------------------------------------------------------------*/
/* machine and compiler patches */
#if 1//defined(__MINGW32__)
  #define SIGSETJMP_IS_BROKEN
#endif
/*--------------------------------------------------------------------------*/
/* my standard collection of includes */
#include "io_error.h"
#include "io_trace.h"
/*--------------------------------------------------------------------------*/
typedef std::complex<double> COMPLEX;
typedef std::pair<double,double> DPAIR;
typedef int OCTAL;

#if defined(NDEBUG)
  #define prechecked_cast static_cast
#else
  #define prechecked_cast dynamic_cast
#endif

#if defined(SIGSETJMP_IS_BROKEN)
  #undef sigjmp_buf
  #undef siglongjmp
  #undef sigsetjmp
  #define sigjmp_buf jmp_buf
  #define siglongjmp(a,b) longjmp(a,b)
  #define sigsetjmp(a,b) setjmp(a)
#endif

#if !defined(SIGNALARGS)
  #define SIGNALARGS int
#endif
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
#endif
