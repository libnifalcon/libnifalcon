/** @file hdlErrors.h
** Error codes returned from HDAL
**
** Copyright 2005-2008 Novint Technologies, Inc. All rights reserved.
** Available only under license from Novint Technologies, Inc.
**
**/

/**
** HDAL API Errors
**
** Client application queries HDAL errors using hdlGetError().
** hdlGetError() returns an error type.
**/

#ifndef HDLERRORS_H
#define HDLERRORS_H

typedef int HDLError;

/* Possible future enhancements
#define HDL_MAJOR_ERROR_MASK 0x0000ffff
#define HDL_MINOR_ERROR_MASK 0xffff0000
 */

#define HDL_NO_ERROR 0x0                  /**< No errors on error stack */

#define HDL_ERROR_STACK_OVERFLOW 0x01     /**< Overflow of error stack */
#define HDL_ERROR_INTERNAL       0x02     /**< HDAL internal error> */

#define HDL_INIT_ERROR_MASK                               0x1F     /**< Mask for all initialization errors */
#define HDL_ERROR_INIT_FAILED                             0x10     /**< Device initialization error */
#define HDL_INIT_INI_NOT_FOUND                            0x11     /**< Could not find configuration file */
#define HDL_INIT_INI_DLL_STRING_NOT_FOUND                 0x12     /**< No DLL name in configuration file */
#define HDL_INIT_INI_MANUFACTURER_NAME_STRING_NOT_FOUND   0x13     /**< No MANUFACTURER_NAME value in configuration file */
#define HDL_INIT_DLL_LOAD_ERROR                           0x14     /**< Could not load driver DLL */
#define HDL_INIT_DEVICE_FAILURE                           0x15     /**< Failed to initilize device */
#define HDL_INIT_DEVICE_ALREADY_INITED                    0x16     /**< Device already initialized */
#define HDL_INIT_DEVICE_NOT_CONNECTED                     0x17     /**< Requested device not connected */

#define HDL_SERVO_START_ERROR                             0x18     /**< Could not start servo thread **/

#endif // HDLERRORS_H






