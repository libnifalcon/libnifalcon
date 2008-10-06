/** @file hdlu.h
** Utility functions for HDAL applications.
**
** Copyright 2005-2008 Novint Technologies, Inc. All rights reserved.
** Available only under license from Novint Technologies, Inc.
** (See LICENSE_NOVINT.txt)
**
** Haptic Device Abstraction Layer
** Low level, cross-platform, general purpose interface.
** 
*/

#ifndef HDLU_H
#define HDLU_H

#include "hdlExports.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Generate transform for mapping between haptic and game workspace.
**
** Inputs specify the minimum and maximum coordinate values of 
** rectangular paralleliped bounding boxes, measured in meters.
** The function computes and returns (in transformMat) the
** transform matrix that will convert the device position into
** workspace coordinates.  See @ref hdlDeviceWorkspace for the HDAL
** function to retrieve the device's workspace.
**
** @param[in] hapticWorkspace minx, miny, minz, maxx, maxy, maxz
** @param[in] gameWorkspace   minx, miny, minz, maxx, maxy, maxz
** @param[in] useUniformScale If true, scale uniformly across the workspace
** @param[out] tranformMat    Transformation from haptic to game workspace
**
** @return Nothing
**
** Errors: None
**
** @todo
** Change notation from "game" to "app".  At this level, the interface is
** not specific to games.
**
** @todo
** Fix spelling of transformMat
*/
HDLAPI void HDLAPIENTRY hdluGenerateHapticToAppWorkspaceTransform(
    double hapticWorkspace[6], 
    double gameWorkspace[6],
    bool useUniformScale,
    double tranformMat[16]
    );

/** Compute a precise time based on CPU high performance timer
** @param None
** @return Current system time, in seconds from start of epoch
**
** @todo Add facility for other platforms in addition to Windows
*/
HDLAPI double hdluGetSystemTime( void );

}; /* extern "C" */
#endif // HDLU_H

