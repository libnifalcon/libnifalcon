/** @file hdlConstants.h
** Constants for  HDAL
**
** Copyright 2005-2008 Novint Technologies, Inc. All rights reserved.
** Available only under license from Novint Technologies, Inc.
** (See LICENSE_NOVINT.txt)
*/

#ifndef HDL_CONSTANTS_H
#define HDL_CONSTANTS_H

/*
** Version
*/
#define HDL_VERSION_INVALID        -1    /**< vesion is invalid            */
#define HDL_VERSION_NOT_APPLICABLE -2    /**< version field not applicable */
#define HDL_VERSION_UNAVAILABLE    -3    /**< version field not available  */

/** Enumeration of version request types.
**
** @see hdlGetVersion
*/
typedef enum
{
    HDL_HDAL           = 0x11,  /**< version of HDAL                           */

    HDL_DEVICE         = 0x21,  /**< device hardware in current device context */
    HDL_DEVICE_SDK     = 0x22,  /**< SDK version of current device             */
    HDL_DEVICE_COMMS   = 0x23,  /**< communications version of current device  */
    HDL_DEVICE_OS      = 0x24,  /**< version of device OS                      */

    HDL_GRIP           = 0x33,  /**< grip in current device context            */

} HDL_VERSION_REQUEST;

/**  Structure returned by hdlGetVersion
**
**  @see hdlGetVersion
*/
typedef long HDL_VERSION_INFO_TYPE;

#endif // HDL_CONSTANTS_H
