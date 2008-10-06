/** @file hdl.h
** Main API for HDAL services.
**
** Copyright 2005-2008 Novint Technologies, Inc. All rights reserved.
** Available only under license from Novint Technologies, Inc.
** (See LICENSE_NOVINT.txt)
**
** Haptic Device Abstraction Layer
** Low level, cross-platform, general purpose interface.
**/



#ifndef HDL_H
#define HDL_H

#include "hdlExports.h"
#include "hdlErrors.h"
#include "hdlConstants.h"

#define HDL_BUTTON_1 0x00000001   /**< Mask for button 1 */
#define HDL_BUTTON_2 0x00000002   /**< Mask for button 2 */
#define HDL_BUTTON_3 0x00000004   /**< Mask for button 3 */
#define HDL_BUTTON_4 0x00000008   /**< Mask for button 4 */
#define HDL_BUTTON_ANY 0xffffffff /**< Mask for any button */

#ifdef __cplusplus
extern "C" {
#endif

/** ID to differentiate between multiple installed devices
 */
/* Note: Must this be done for different manufacturers? */
typedef int HDLDeviceID;

/** Handle to differentiate between multiple installed devices.
 *  Handle is an abstraction returned by the initialization routine. 
 */
typedef int HDLDeviceHandle;

/** ID for the default haptic device (usually one installed).
 */
#define HDL_DEFAULT_DEVICE_ID 0

/** Handle indicating invalid device handle */
#define HDL_INVALID_HANDLE -1

/**  Return the current error code from the error stack.
 **  @return Error code on the top of the error stack.
 **  @n HDL_NO_ERROR if the error stack is empty.
 */
HDLAPI HDLError HDLAPIENTRY hdlGetError();


/** Set path to hdal.ini configuration file.
 **
 ** @param[in] configPath Path to hdal.ini configuration file
 ** @return Nothing
 ** @par
 ** Errors: None
 **/
HDLAPI void HDLAPIENTRY hdlSetConfigPath(const char configPath);


/** Initialize a haptic device
 **
 ** @param[in] deviceID  ID of haptic device
 ** @return Handle to haptic device
 ** @par
 ** Errors:
 **  <ul>
 **    <li> manufacturer specific
 **    <li> could not load device specific dll
 **  </ul>
 **
 ** @deprecated
 ** Only supports a single device, deviceID is ignored.
 ** Included to support older apps.  Use @ref hdlInitNamedDevice instead.
 **
 ** @see hdlInitNamedDevice
 */
HDLAPI HDLDeviceHandle HDLAPIENTRY hdlInitDevice(HDLDeviceID deviceID);


/** Initialize a specific named haptic device
 **
 ** @param[in] deviceName  Name of haptic device.
 ** @param[in] configPath  Path/file for ini file.
 **
 ** @par
 ** @e configPath search order:
 ** <ol> 
 **   <li> relative to executable directory </li>
 **   <li> relative to executable directory's parent if executable directory is Debug or Release </li>
 **   <li> config directory in path specified by NOVINT_DEVICE_SUPPORT </li>
 ** </ol>
 **     
 ** @return Handle to haptic device
 ** @par
 ** Errors:
 **  <ul>
 **    <li> manufacturer specific
 **    <li> could not load device specific dll
 **  </ul>
 **
 ** @note
 ** - Support multiple devices via deviceName string.
 **
 ** @note
 ** Setup sequence:
 ** @code
 ** hdlInitNamedDevice(...);
 ** hdlStart();
 ** hdlCreateServoOp(...);
 ** @endcode
 ** Teardown sequence:
 ** @code
 ** hdlDestroyServoOp(...);
 ** hdlStop();
 ** hdlUninitDevice(...);
 ** @endcode
 ** @see hdlInitIndexedDevice, hdlStart, hdlStop, 
 **      hdlCreateServoOp, hdlDestroyServoOp, hdlUninitDevice
 **
 */
HDLAPI HDLDeviceHandle HDLAPIENTRY hdlInitNamedDevice(const char* deviceName, 
                                                      const char* configPath = 0);


/** Initialize a specific indexed haptic device
 **
 ** @param[in] index       Index of haptic device.
 ** @param[in] configPath  Path/file for ini file.
 **
 ** @par
 ** @e configPath search order:
 ** @see hdlInitNamedDevice
 **     
 ** @return Handle to haptic device
 ** @par
 ** Errors:
 **  <ul>
 **    <li> manufacturer specific
 **    <li> could not load device specific dll
 **  </ul>
 **
 ** @note
 ** - Support only Falcon devices via index
 ** - Index refers to alphabetical sort order by serial number
 **
 ** @note
 ** Setup sequence:
 ** @code
 ** m = hdlCountDevices();
 ** hdlInitIndexedDevice(n); // 0 <= n < m
 ** hdlStart();
 ** hdlCreateServoOp(...);
 ** @endcode
 ** Teardown sequence:
 ** @code
 ** hdlDestroyServoOp(...);
 ** hdlStop();
 ** hdlUninitDevice(...);
 ** @endcode
 ** @see hdlInitNamedDevice, hdlStart, hdlStop, hdlCreateServoOp, 
 **      hdlDestroyServoOp, hdlUninitDevice
 **
 */
HDLAPI HDLDeviceHandle HDLAPIENTRY hdlInitIndexedDevice(const int index, 
                                                        const char* configPath = 0);

/** Start servo and all haptic devices.
 **
 ** @param None
 ** @return Handle to haptic device
 ** @par
 ** Errors:
 **  <ul>
 **    <li> manufacturer specific
 **    <li> servo could not start
 **  </ul>
 **
 ** @note
 ** Starts servo and all haptic devices.
 ** Call after all devices are initialized.
 ** Start is separated from hdlInitNamedDevice to allow all
 ** devices to be initialized before servo operations are
 ** started.  Currently, only one Falcon at a time is supported,
 ** but this restriction will be lifted in the future.  Other
 ** device types supported by HDAL may already allow multiple
 ** devices to be connected.
 ** @par
 ** @see hdlStop, hdlInitNamedDevice, hdlInitIndexedDevice
 */
HDLAPI void HDLAPIENTRY hdlStart();

/** Stop servo and all haptic devices.
 **
 ** @param None
 ** @return Nothing
 ** @par
 ** @see hdlStart, hdlInitNamedDevice, hdlInitIndexedDevice
 */
HDLAPI void HDLAPIENTRY hdlStop();

/** Query HDAL state.
 **
 ** @param None
 ** @return Nothing
 ** @par
 ** Errors: manufacturer specific
 ** 
 ** @note
 ** If return == HDAL_ISREADY, device is ready.
 ** Otherwise, test to see reason:\n
 **    return && XXXX != 0, where XXXX is\n
 **    HDAL_UNINITIALIZED      hdlInitNamedDevice failed earlier\n
 **    HDAL_SERVO_NOT_STARTED  hdlStart() not called earlier\n
 **    HDAL_NOT_CALIBRATED     needs autocalibration
 */
HDLAPI unsigned int HDLAPIENTRY hdlGetState();

/** Normal hdlGetStatus return code
 */
#define HDAL_ISREADY           0

/** hdlGetStatus code indicating HDAL not yet initialized
 */
#define HDAL_UNINITIALIZED     0x01

/** hdlGetStatus code indicating servo loop net yet started
 */
#define HDAL_SERVO_NOT_STARTED 0x02

/** hdlGetStatus code indicating motors not homed
 */
#define HDAL_NOT_CALIBRATED    0x04

/** Uninitializes a haptic device
 ** 
 ** @param[in] hHandle Handle of haptic device
 ** @return Nothing
 ** @par
 ** Errors: manufacturer specific
 ** @par
 ** @see hdlInitNamedDevice, hdlInitIndexedDevice
 */
HDLAPI void HDLAPIENTRY hdlUninitDevice(HDLDeviceHandle hHandle);

/** Make a specific haptic device current
 **   (Allows application to send forces to a specific device)
 ** 
 ** @param[in] hHandle Haptic device handle
 ** @return Nothing
 ** @par
 ** Errors:
 **  <ul>
 **    <li> manufacturer specific
 **    <li> hHandle invalid
 **  </ul>
 */
HDLAPI void HDLAPIENTRY hdlMakeCurrent(HDLDeviceHandle hHandle);

/** Retrieve the workspace of the device, measured in meters.
 **
 **   Call this function to retrieve the workspace of the current device.
 **   Since not all devices have the same physical workspace dimensions,
 **   the application must account for different device workspaces.
 **   The workspace is defined in the device reference coordinate frame.
 **   It is up to the user to transform positions in this coordinate frame
 **   into the application's coordinate frame.
 **   See @ref hdluGenerateHapticToAppWorkspaceTransform for a utility
 **   function to assist in this.
 **
 **   Dimension order: minx, miny, minz, maxx, maxy, maxy
 **                    (left, bottom, far, right, top, near)
 **   @n (minx, miny, minz) are the coordinates of the left-bottom-far
 **   corner of the device workspace.
 **   @n (maxx, maxy, maxz) are the coordinates of the right-top-near
 **   corner of the device workspace.
 **
 ** @param[out] workspaceDimensions See explanation above.
 ** @return Nothing
 ** @par
 ** Errors:
 **  <ul>
 **    <li> manufacturer specific
 **    <li> no current device
 **  </ul>
 ** @todo
 ** Using dimension order from HDAPI.
 ** Should the parameter list instead be (left, bottom, near, right, top, far)?
 ** It would make the call easier to read, and not significantly slower.
  */
HDLAPI void HDLAPIENTRY hdlDeviceWorkspace(double workspaceDimensions[6]);

/** Return the device model string
 ** @param None
 ** @return Device model string
 ** @par
 ** Errors: None
 */
HDLAPI const char* HDLAPIENTRY hdlDeviceModel();

/** Set the force to be generated by the device, measured in newtons.
 **   Forces are in device coordinates.
 **   Dimension order: x, y, z
 ** 
 ** @param[in]  force Measured in Newtons; x, y, z order
 ** @return Nothing
 ** @par
 ** Errors:
 **  <ul>
 **    <li> manufacturer specific
 **    <li> no current device
 **    <li> max force exceeded
 **  </ul>
 **
*/
HDLAPI void HDLAPIENTRY hdlSetToolForce(double force[3]);

/** Return current tool position
 **
 ** @param[out] position In x, y, z order, measured in meters.
 ** @return Nothing
 ** @par
 ** Errors: None
 */
HDLAPI void HDLAPIENTRY hdlToolPosition(double position[3]);

/** @cond SIMQUEST
 ** This function exists only for approved projects and only until a
 ** fully specified orientation API is developed
 */

/**  Return current tool orientation.
 **
 ** @param[out] transform 4x4 affine transform matrix
 ** @return Nothing
 ** @par
 ** Errors: None
 **
 ** Note: This is a special-use function and will not be supported
 ** once an end-effector API is available.
 */
HDLAPI void HDLAPIENTRY hdlToolOrientation_SQ(double transform[16]);
/** @endcond
 */

/** Return current state of tool button(s).
 ** For multi-button devices, if any button is pressed, 
 ** pButton* is set to true.
 **
 ** @param[out] pButton Pointer to bool to hold button state
 ** @return Nothing
 ** @par
 ** Errors: None
 */
HDLAPI void HDLAPIENTRY hdlToolButton(bool* pButton);

/** Return current state of tool buttons.
 ** Returned value is a bitmask of buttons, with the least significant
 ** bit associated with button "0".
 **
 ** @param[out] pButton Pointer to an int to hold button states
 ** @return Nothing
 ** @par
 ** Errors: None
 */
HDLAPI void HDLAPIENTRY hdlToolButtons(int* pButton);

/*
 *  Scheduling
 */
#define HDL_SERVOOP_EXIT 0        /**< Return code for exiting servo loop */
#define HDL_SERVOOP_CONTINUE 1    /**< Return code for continuing servo loop */

typedef int HDLServoOpExitCode;   /**< Type for Servo loop operation exit code */
typedef int HDLOpHandle;          /**< Type for Servo loop operation handle */

/** Prototype for Servo operation function
 **
 ** @param[out] pParam Pointer to data required by operation
 ** @return Exit code
 ** @par
 ** Errors: None
 */
typedef HDLServoOpExitCode HDLServoOp (void *pParam);

/** Schedule an operation (callback) to run in the servo loop.
 **   Operation is either blocking (client waits until completion)
 **   or non-blocking (client continues execution).
 **
 ** @param[in] pServoOp Pointer to servo operation function
 ** @param[in] pParam Pointer to data for servo operation function
 ** @param[in] bBlocking Flag to indicate whether servo loop blocks
 ** @return Handle to servo operation entry
 ** @par
 ** Errors: None
 ** @par
 ** @see hdlDestroyServoOp, hdlInitNamedDevice, hdlInitIndexedDevice
 */
HDLAPI HDLOpHandle HDLAPIENTRY hdlCreateServoOp(
    HDLServoOp pServoOp,  /* function pointer callback */
    void* pParam,         /* parameter to callback     */
    bool bBlocking        /* is callback blocking/non-blocking */
    );

/** Remove an operation (callback) from the servo loop.
 **
 ** @param[in] hServoOp Handle to servo op to remove
 ** @return Nothing
 ** @par
 ** Errors: None.
 ** @note
 ** hdlDestroyServoOp() should be called at application termination
 ** time for any servo operation that was added with bBlocking = false.
 ** @par
 ** @see hdlCreateServoOp, hdlInitNamedDevice, hdlIndexedDevice
 */
HDLAPI void HDLAPIENTRY hdlDestroyServoOp(
    HDLOpHandle hServoOp  /* handle to servo op */
    );


/** Get version information.
 **
 ** @param[in] requestType Type of version information requested
 ** @param[out] versionInfo Requested info
 ** @return Success or failure
 ** @par
 ** Errors: None.
 ** @par
 ** Typical usage:
 ** @par
 ** @code
 ** HDL_VERSION_INFO_TYPE deviceVersion;
 ** int                   deviceMajor;
 ** int                   deviceMinor;
 ** _int64                deviceSerialNumber;
 ** if (hdlGetVersion(HDL_DEVICE, &deviceVersion))
 ** {
 **     deviceMajor = HDL_MAJOR_VERSION(deviceVersion);
 **     deviceMinor = HDL_MINOR_VERSION(deviceVersion);
 **     deviceSerialNumber = HDL_BUILD_VERSION(deviceVersion);
 ** }
 ** @endcode
 */
HDLAPI bool HDLAPIENTRY hdlGetVersion(HDL_VERSION_REQUEST requestType, 
                                      HDL_VERSION_INFO_TYPE* versionInfo
                                      );

/** Return Major component of version struct
 **
 ** @param[in] versionInfo HDL_VERSION_INFO_TYPE struct returned from hdlGetVersion()
 ** @return Major component
 ** @see hdlGetVersion().
 ** Errors: None.
 */
HDLAPI int HDLAPIENTRY HDL_MAJOR_VERSION(HDL_VERSION_INFO_TYPE versionInfo);

/** Return Minor component of version struct
 **
 ** @param[in] versionInfo HDL_VERSION_INFO_TYPE struct returned from hdlGetVersion()
 ** @return Minor component
 ** @see hdlGetVersion().
 ** @par
 ** Errors: None.
 */
HDLAPI int HDLAPIENTRY HDL_MINOR_VERSION(HDL_VERSION_INFO_TYPE versionInfo);

/** Return Build component of version struct
 ** @param[in]   versionInfo   HDL_VERSION_INFO_TYPE struct returned from hdlGetVersion()
 ** @return Build component
 ** @see hdlGetVersion().
 ** @par
 ** Errors: None.
 */
HDLAPI long HDLAPIENTRY HDL_BUILD_VERSION(HDL_VERSION_INFO_TYPE versionInfo);

/** @cond REPEATED
 ** This function is documented in hdlu.h and is included here only for convenience
 */
HDLAPI void HDLAPIENTRY hdluGenerateHapticToAppWorkspaceTransform(
    double hapticWorkspace[6], 
    double gameWorkspace[6],
    bool useUniformScale,
    double transformMat[16]);
/** @endcond
 */

/** Count connected devices.
 **
 ** @param None
 ** @return Number of connected devices
 ** @par
 ** Errors: None
 **
 ** @note
 ** Only valid for Novint Falcon devices
 */
HDLAPI int HDLAPIENTRY hdlCountDevices();

}; /* extern "C" */
#endif /* HDL_H */

