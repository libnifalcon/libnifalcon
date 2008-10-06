/*
** Copyright 2005 Novint Technologies, Inc. All rights reserved.
** Available only under license from Novint Technologies, Inc.
** (See LICENSE_NOVINT.txt)
**
** hdluExports.h
**
** Haptic Device Application Layer API
** Exports
*/
#ifndef HDLU_EXPORTS_H
#define HDLU_EXPORTS_H

#ifdef WIN32
#ifdef HDLU_EXPORTS
#define HDLUAPI __declspec(dllexport)
#else
#define HDLUAPI __declspec(dllimport)
#endif

#define HDLUAPIENTRY  __stdcall

#endif /* WIN32 */
#endif /* HDLU_EXPORTS_H */
