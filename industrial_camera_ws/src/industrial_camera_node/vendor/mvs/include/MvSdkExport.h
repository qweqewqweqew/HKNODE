#ifndef _MV_SDK_EXPORT_H__
#define _MV_SDK_EXPORT_H__

#include "MvErrorDefine.h"
#include "CameraParams.h"
#include "ObsoleteCamParams.h"

#ifndef MVSDKEXPORT_API

    #ifdef _WIN32
        #if defined(MVSDKEXPORT_EXPORTS)
            #define MVSDKEXPORT_API __declspec(dllexport)
        #else
            #define MVSDKEXPORT_API __declspec(dllimport)
        #endif
    #else
        #ifndef __stdcall
            #define __stdcall
        #endif

        #ifndef MVSDKEXPORT_API
            #define  MVSDKEXPORT_API
        #endif
    #endif

#endif

#ifndef IN
    #define IN
#endif

#ifndef OUT
    #define OUT
#endif

#ifdef __cplusplus
extern "C" {
#endif
    
/************************************************************************/
/* 璁惧鐨勫熀鏈寚浠ゅ拰鎿嶄綔                                         		*/
/* Camera basic instructions and operations                             */
/************************************************************************/
/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇SDK鐗堟湰鍙?
 *  @return 杩斿洖4瀛楄妭鐗堟湰鍙?
            |涓?   |娆?   |淇  |  娴嬭瘯|
             8bits  8bits  8bits  8bits
 *  @remarks 姣斿杩斿洖鍊间负0x01000001锛屽嵆SDK鐗堟湰鍙蜂负V1.0.0.1銆?
 
 *  @~english
 *  @brief  Get SDK Version
 *  @return Always return 4 Bytes of version number 
            |Main    |Sub    |Rev  |  Test|
             8bits  8bits  8bits  8bits
 *  @remarks For example, if the return value is 0x01000001, the SDK version is V1.0.0.1.
 ************************************************************************/
MVSDKEXPORT_API unsigned int __stdcall MV_CC_GetSDKVersion();

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇鏀寔鐨勪紶杈撳眰
 *  @return 鏀寔鐨勪紶杈撳眰缂栧彿 

 *  @~english
 *  @brief  Get supported Transport Layer
 *  @return Supported Transport Layer number
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_EnumerateTls();

/********************************************************************//**
 *  @~chinese
 *  @brief  鏋氫妇璁惧
 *  @param  nTLayerType                 [IN]            鏋氫妇浼犺緭灞?
 *  @param  pstDevList                  [OUT]           璁惧鍒楄〃
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璁惧鍒楄〃鐨勫唴瀛樻槸鍦⊿DK鍐呴儴鍒嗛厤鐨勶紝澶氱嚎绋嬭皟鐢ㄨ鎺ュ彛鏃朵細杩涜璁惧鍒楄〃鍐呭瓨鐨勯噴鏀惧拰鐢宠锛孿n
             寤鸿灏介噺閬垮厤澶氱嚎绋嬫灇涓炬搷浣溿€?

 *  @~english
 *  @brief  Enumerate Device
 *  @param  nTLayerType                 [IN]            Enumerate TLs
 *  @param  pstDevList                  [OUT]           Device List
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks The memory of the device list is allocated within the SDK. When the interface is invoked by multiple threads, the memory of the device list will be released and applied.\n
             It is recommended to avoid multithreaded enumeration operations as much as possible.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_EnumDevices(IN unsigned int nTLayerType, IN OUT MV_CC_DEVICE_INFO_LIST* pstDevList);

/********************************************************************//**
 *  @~chinese
 *  @brief  鏍规嵁鍘傚晢鍚嶅瓧鏋氫妇璁惧
 *  @param  nTLayerType                 [IN]            鏋氫妇浼犺緭灞?
 *  @param  pstDevList                  [OUT]           璁惧鍒楄〃
 *  @param  strManufacturerName           [IN]            鍘傚晢鍚嶅瓧
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璁惧鍒楄〃鐨勫唴瀛樻槸鍦⊿DK鍐呴儴鍒嗛厤鐨勶紝澶氱嚎绋嬭皟鐢ㄨ鎺ュ彛鏃朵細杩涜璁惧鍒楄〃鍐呭瓨鐨勯噴鏀惧拰鐢宠锛孿n
             寤鸿灏介噺閬垮厤澶氱嚎绋嬫灇涓炬搷浣溿€?

 *  @~english
 *  @brief  Enumerate device according to manufacture name
 *  @param  nTLayerType                 [IN]            Transmission layer of enumeration
 *  @param  pstDevList                  [OUT]           Device list
 *  @param  strManufacturerName           [IN]            Manufacture Name
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks The memory of the device list is allocated within the SDK. When the interface is invoked by multiple threads, the memory of the device list will be released and applied.\n
             It is recommended to avoid multithreaded enumeration operations as much as possible.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_EnumDevicesEx(IN unsigned int nTLayerType, IN OUT MV_CC_DEVICE_INFO_LIST* pstDevList, IN const char* strManufacturerName);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁惧鏄惁鍙揪
 *  @param  pstDevInfo                  [IN]            璁惧淇℃伅缁撴瀯浣?
 *  @param  nAccessMode                 [IN]            璁块棶鏉冮檺
 *  @return 鍙揪锛岃繑鍥瀟rue锛涗笉鍙揪锛岃繑鍥瀎alse
 *  @remarks 璇诲彇璁惧CCP瀵勫瓨鍣ㄧ殑鍊硷紝鍒ゆ柇褰撳墠鐘舵€佹槸鍚﹀叿鏈夋煇绉嶈闂潈闄愩€?\n
             濡傛灉璁惧涓嶆敮鎸丮V_ACCESS_ExclusiveWithSwitch銆丮V_ACCESS_ControlWithSwitch銆丮V_ACCESS_ControlSwitchEnableWithKey杩欎笁绉嶆ā寮忥紝鎺ュ彛杩斿洖false銆傜洰鍓嶈澶囦笉鏀寔杩?绉嶆姠鍗犳ā寮忥紝鍥介檯涓婁富娴佺殑鍘傚晢鐨勮澶囦篃閮芥殏涓嶆敮鎸佽繖3绉嶆ā寮忋€?\n
             璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧銆?
 
 *  @~english
 *  @brief  Is the device accessible
 *  @param  pstDevInfo                  [IN]            Device Information Structure
 *  @param  nAccessMode                 [IN]            Access Right
 *  @return Access, return true. Not access, return false
 *  @remarks Read device CCP register value and determine current access permission.\n 
             Return false if the device does not support the modes MV_ACCESS_ExclusiveWithSwitch, MV_ACCESS_ControlWithSwitch, MV_ACCESS_ControlSwitchEnableWithKey. Currently the device does not support the 3 preemption modes, neither do the devices from other mainstream manufacturers. \n
             This API is not supported by CameraLink device. 
 ************************************************************************/
MVSDKEXPORT_API bool __stdcall MV_CC_IsDeviceAccessible(IN MV_CC_DEVICE_INFO* pstDevInfo, IN unsigned int nAccessMode);

/********************************************************************//**
 *  @~chinese
 *  @brief  鍒涘缓璁惧鍙ユ焺
 *  @param  handle                      [OUT]           璁惧鍙ユ焺
 *  @param  pstDevInfo                  [IN]            璁惧淇℃伅缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 鏍规嵁杈撳叆鐨勮澶囦俊鎭紝鍒涘缓搴撳唴閮ㄥ繀椤荤殑璧勬簮鍜屽垵濮嬪寲鍐呴儴妯″潡銆傞€氳繃璇ユ帴鍙ｅ垱寤哄彞鏌勶紝璋冪敤SDK鎺ュ彛锛屼細榛樿鐢熸垚SDK鏃ュ織鏂囦欢锛屽鏋滀笉闇€瑕佺敓鎴愭棩蹇楁枃浠讹紝鍙互閫氳繃MV_CC_CreateHandleWithoutLog鍒涘缓鍙ユ焺銆?
 
 *  @~english
 *  @brief  Create Device Handle
 *  @param  handle                      [OUT]           Device handle
 *  @param  pstDevInfo                  [IN]            Device Information Structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Create required resources within library and initialize internal module according to input device information. Create handle and call SDK interface through this interface, and SDK log file will be created by default. Creating handle through MV_CC_CreateHandleWithoutLog will not generate log files.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_CreateHandle(OUT void ** handle, IN const MV_CC_DEVICE_INFO* pstDevInfo);

/********************************************************************//**
 *  @~chinese
 *  @brief  鍒涘缓璁惧鍙ユ焺锛屼笉鐢熸垚鏃ュ織
 *  @param  handle                      [OUT]           璁惧鍙ユ焺
 *  @param  pstDevInfo                  [IN]            璁惧淇℃伅缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 鏍规嵁杈撳叆鐨勮澶囦俊鎭紝鍒涘缓搴撳唴閮ㄥ繀椤荤殑璧勬簮鍜屽垵濮嬪寲鍐呴儴妯″潡銆傞€氳繃璇ユ帴鍙ｅ垱寤哄彞鏌勶紝璋冪敤SDK鎺ュ彛锛屼笉浼氶粯璁ょ敓鎴怱DK鏃ュ織鏂囦欢锛屽鏋滈渶瑕佺敓鎴愭棩蹇楁枃浠跺彲浠ラ€氳繃MV_CC_CreateHandle鍒涘缓鍙ユ焺锛屾棩蹇楁枃浠惰嚜鍔ㄧ敓鎴愩€?
 
 *  @~english
 *  @brief  Create Device Handle without log
 *  @param  handle                      [OUT]           Device handle
 *  @param  pstDevInfo                  [IN]            Device Information Structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Create required resources within library and initialize internal module according to input device information. Create handle and call SDK interface through this interface, and SDK log file will not be created. To create logs, create handle through MV_CC_CreateHandle, and log files will be automatically generated.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_CreateHandleWithoutLog(OUT void ** handle, IN const MV_CC_DEVICE_INFO* pstDevInfo);

/********************************************************************//**
 *  @~chinese
 *  @brief  閿€姣佽澶囧彞鏌?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @~english
 *  @brief  Destroy Device Handle
 *  @param  handle                      [IN]            Device handle
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_DestroyHandle(IN void * handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  鎵撳紑璁惧
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nAccessMode                 [IN]            璁块棶鏉冮檺
 *  @param  nSwitchoverKey              [IN]            鍒囨崲璁块棶鏉冮檺鏃剁殑瀵嗛挜
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 鏍规嵁璁剧疆鐨勮澶囧弬鏁帮紝鎵惧埌瀵瑰簲鐨勮澶囷紝杩炴帴璁惧銆俓n 
             璋冪敤鎺ュ彛鏃跺彲涓嶄紶鍏AccessMode鍜宯SwitchoverKey锛屾鏃堕粯璁よ澶囪闂ā寮忎负鐙崰鏉冮檺銆傜洰鍓嶈澶囨殏涓嶆敮鎸丮V_ACCESS_ExclusiveWithSwitch銆丮V_ACCESS_ControlWithSwitch銆丮V_ACCESS_ControlSwitchEnable銆丮V_ACCESS_ControlSwitchEnableWithKey杩欏洓绉嶆姠鍗犳ā寮忋€俓n 
             瀵逛簬U3V璁惧锛宯AccessMode銆乶SwitchoverKey杩欎袱涓弬鏁版棤鏁堛€?
 
 *  @~english
 *  @brief  Open Device
 *  @param  handle                      [IN]            Device handle
 *  @param  nAccessMode                 [IN]            Access Right
 *  @param  nSwitchoverKey              [IN]            Switch key of access right
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Find specific device and connect according to set device parameters. \n
             When calling the interface, the input of nAccessMode and nSwitchoverKey is optional, and the device access mode is exclusive. Currently the device does not support the following preemption modes: MV_ACCESS_ExclusiveWithSwitch, MV_ACCESS_ControlWithSwitch, MV_ACCESS_ControlSwitchEnableWithKey.\n 
             For USB3Vision device, nAccessMode, nSwitchoverKey are invalid. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_OpenDevice(IN void* handle, IN unsigned int nAccessMode, IN unsigned short nSwitchoverKey);

/********************************************************************//**
 *  @~chinese
 *  @brief  鍏抽棴璁惧
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 閫氳繃MV_CC_OpenDevice杩炴帴璁惧鍚庯紝鍙互閫氳繃璇ユ帴鍙ｆ柇寮€璁惧杩炴帴锛岄噴鏀捐祫婧?
 
 *  @~english
 *  @brief  Close Device
 *  @param  handle                      [IN]            Device handle
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After connecting to device through MV_CC_OpenDevice, use this interface to disconnect and release resources.
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_CloseDevice(IN void* handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  鍒ゆ柇璁惧鏄惁澶勪簬杩炴帴鐘舵€?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @return 璁惧澶勪簬杩炴帴鐘舵€侊紝杩斿洖true锛涙病杩炴帴鎴栧け鍘昏繛鎺ワ紝杩斿洖false
 
 *  @~english
 *  @brief  Is The Device Connected
 *  @param  handle                      [IN]            Device handle
 *  @return Connected, return true. Not Connected or DIsconnected, return false
 ***********************************************************************/
MVSDKEXPORT_API bool __stdcall MV_CC_IsDeviceConnected(IN void* handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  娉ㄥ唽鍥惧儚鏁版嵁鍥炶皟
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  cbOutput                    [IN]            鍥炶皟鍑芥暟鎸囬拡
 *  @param  pUser                       [IN]            鐢ㄦ埛鑷畾涔夊彉閲?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 閫氳繃璇ユ帴鍙ｅ彲浠ヨ缃浘鍍忔暟鎹洖璋冨嚱鏁帮紝鍦∕V_CC_CreateHandle涔嬪悗鍗冲彲璋冪敤銆?\n
             鍥惧儚鏁版嵁閲囬泦鏈変袱绉嶆柟寮忥紝涓ょ鏂瑰紡涓嶈兘澶嶇敤锛歕n
             鏂瑰紡涓€锛氳皟鐢∕V_CC_RegisterImageCallBackEx璁剧疆鍥惧儚鏁版嵁鍥炶皟鍑芥暟锛岀劧鍚庤皟鐢∕V_CC_StartGrabbing寮€濮嬮噰闆嗭紝閲囬泦鐨勫浘鍍忔暟鎹湪璁剧疆鐨勫洖璋冨嚱鏁颁腑杩斿洖銆俓n
             鏂瑰紡浜岋細璋冪敤MV_CC_StartGrabbing寮€濮嬮噰闆嗭紝鐒跺悗鍦ㄥ簲鐢ㄥ眰寰幆璋冪敤MV_CC_GetOneFrameTimeout鑾峰彇鎸囧畾鍍忕礌鏍煎紡鐨勫抚鏁版嵁锛岃幏鍙栧抚鏁版嵁鏃朵笂灞傚簲鐢ㄧ▼搴忛渶瑕佹牴鎹抚鐜囨帶鍒跺ソ璋冪敤璇ユ帴鍙ｇ殑棰戠巼銆?\n
             璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧銆?
 
 *  @~english
 *  @brief  Register the image callback function
 *  @param  handle                      [IN]            Device handle
 *  @param  cbOutput                    [IN]            Callback function pointer
 *  @param  pUser                       [IN]            User defined variable
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After MV_CC_CreateHandle, call this interface to set image data callback function.\n 
             There are two available image data acquisition modes, and cannot be used together: \n
             Mode 1: Call MV_CC_RegisterImageCallBack to set image data callback function, and then callMV_CC_StartGrabbing to start acquiring. The acquired image data will return in the set callback function.\n
             Mode 2: Call MV_CC_StartGrabbing to start acquiring, and then call MV_CC_GetOneFrameTimeout repeatedly in application layer to get frame data of specified pixel format. When getting frame data, the frequency of calling this interface should be controlled by upper layer application according to frame rate. \n
             This API is not supported by CameraLink device. 
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_RegisterImageCallBackEx(void* handle, 
                                                         void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser),
                                                         void* pUser);

/********************************************************************//**
 *  @~chinese
 *  @brief  娉ㄥ唽鍥惧儚鏁版嵁鍥炶皟锛孯GB
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  cbOutput                    [IN]            鍥炶皟鍑芥暟鎸囬拡
 *  @param  pUser                       [IN]            鐢ㄦ埛鑷畾涔夊彉閲?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 閫氳繃璇ユ帴鍙ｅ彲浠ヨ缃浘鍍忔暟鎹洖璋冨嚱鏁帮紝鍦∕V_CC_CreateHandle涔嬪悗鍗冲彲璋冪敤銆?\n
             鍥惧儚鏁版嵁閲囬泦鏈変袱绉嶆柟寮忥紝涓ょ鏂瑰紡涓嶈兘澶嶇敤锛歕n
             鏂瑰紡涓€锛氳皟鐢∕V_CC_RegisterImageCallBackForRGB璁剧疆RGB24鏍煎紡鍥惧儚鏁版嵁鍥炶皟鍑芥暟锛岀劧鍚庤皟鐢∕V_CC_StartGrabbing寮€濮嬮噰闆嗭紝閲囬泦鐨勫浘鍍忔暟鎹湪璁剧疆鐨勫洖璋冨嚱鏁颁腑杩斿洖銆俓n
             鏂瑰紡浜岋細璋冪敤MV_CC_StartGrabbing寮€濮嬮噰闆嗭紝鐒跺悗鍦ㄥ簲鐢ㄥ眰寰幆璋冪敤MV_CC_GetImageForRGB鑾峰彇RGB24鏍煎紡鐨勫抚鏁版嵁锛岃幏鍙栧抚鏁版嵁鏃朵笂灞傚簲鐢ㄧ▼搴忛渶瑕佹牴鎹抚鐜囨帶鍒跺ソ璋冪敤璇ユ帴鍙ｇ殑棰戠巼銆?\n
             璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧銆?
 
 *  @~english
 *  @brief  register image data callback, RGB
 *  @param  handle                      [IN]            Device handle
 *  @param  cbOutput                    [IN]            Callback function pointer
 *  @param  pUser                       [IN]            User defined variable
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Before calling this API to set image data callback function, you should call this API MV_CC_CreateHandle. \n
             There are two image acquisition modes, the two modes cannot be reused: \n
             Mode 1: Call MV_CC_RegisterImageCallBackForRGB to set RGB24 format image data callback function, and then call MV_CC_StartGrabbing to start acquisition, the collected image data will be returned in the configured callback function.\n
             Mode 2: Call MV_CC_StartGrabbing to start acquisition, and the call MV_CC_GetImageForRGB repeatedly in application layer to get frame data with RGB24 format. When getting frame data, the upper application program should control the frequency of calling this API according to frame rate. \n
             This API is not supported by CameraLink device. 
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_RegisterImageCallBackForRGB(void* handle, 
                                                         void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser),
                                                         void* pUser);

/********************************************************************//**
 *  @~chinese
 *  @brief  娉ㄥ唽鍥惧儚鏁版嵁鍥炶皟锛孊GR
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  cbOutput                    [IN]            鍥炶皟鍑芥暟鎸囬拡
 *  @param  pUser                       [IN]            鐢ㄦ埛鑷畾涔夊彉閲?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 閫氳繃璇ユ帴鍙ｅ彲浠ヨ缃浘鍍忔暟鎹洖璋冨嚱鏁帮紝鍦∕V_CC_CreateHandle涔嬪悗鍗冲彲璋冪敤銆俓n
             鍥惧儚鏁版嵁閲囬泦鏈変袱绉嶆柟寮忥紝涓ょ鏂瑰紡涓嶈兘澶嶇敤锛歕n
             鏂瑰紡涓€锛氳皟鐢∕V_CC_RegisterImageCallBackForBGR璁剧疆BGR24鍥惧儚鏁版嵁鍥炶皟鍑芥暟锛岀劧鍚庤皟鐢∕V_CC_StartGrabbing寮€濮嬮噰闆嗭紝閲囬泦鐨勫浘鍍忔暟鎹湪璁剧疆鐨勫洖璋冨嚱鏁颁腑杩斿洖銆俓n
             鏂瑰紡浜岋細璋冪敤MV_CC_StartGrabbing寮€濮嬮噰闆嗭紝鐒跺悗鍦ㄥ簲鐢ㄥ眰寰幆璋冪敤MV_CC_GetImageForBGR鑾峰彇BGR24鏍煎紡鐨勫抚鏁版嵁锛岃幏鍙栧抚鏁版嵁鏃朵笂灞傚簲鐢ㄧ▼搴忛渶瑕佹牴鎹抚鐜囨帶鍒跺ソ璋冪敤璇ユ帴鍙ｇ殑棰戠巼銆?\n
             璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧銆?
 
 *  @~english
 *  @brief  register image data callback, BGR
 *  @param  handle                      [IN]            Device handle
 *  @param  cbOutput                    [IN]            Callback function pointer
 *  @param  pUser                       [IN]            User defined variable
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Before calling this API to set image data callback function, you should call this API MV_CC_CreateHandle. \n
             There are two image acquisition modes, the two modes cannot be reused: \n
             Mode 1: Call MV_CC_RegisterImageCallBackForBGR to set RGB24 format image data callback function, and then call MV_CC_StartGrabbing to start acquisition, the collected image data will be returned in the configured callback function.\n
             Mode 2: Call MV_CC_StartGrabbing to start acquisition, and the call MV_CC_GetImageForBGR repeatedly in application layer to get frame data with BGR24 format. When getting frame data, the upper application program should control the frequency of calling this API according to frame rate.\n 
             This API is not supported by CameraLink device. 
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_RegisterImageCallBackForBGR(void* handle, 
                                                         void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser),
                                                         void* pUser);

/********************************************************************//**
 *  @~chinese
 *  @brief  寮€濮嬪彇娴?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧銆?
 
 *  @~english
 *  @brief  Start Grabbing
 *  @param  handle                      [IN]            Device handle
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This API is not supported by CameraLink device. 
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_StartGrabbing(IN void* handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  鍋滄鍙栨祦
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧銆?
 
 *  @~english
 *  @brief  Stop Grabbing
 *  @param  handle                      [IN]            Device handle
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This API is not supported by CameraLink device. 
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_StopGrabbing(IN void* handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇涓€甯GB鏁版嵁锛屾鍑芥暟涓烘煡璇㈠紡鑾峰彇锛屾瘡娆¤皟鐢ㄦ煡璇㈠唴閮?
            缂撳瓨鏈夋棤鏁版嵁锛屾湁鏁版嵁鍒欒幏鍙栨暟鎹紝鏃犳暟鎹繑鍥為敊璇爜
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pData                       [OUT]           鍥惧儚鏁版嵁鎺ユ敹鎸囬拡
 *  @param  nDataSize                   [IN]            鎺ユ敹缂撳瓨澶у皬
 *  @param  pstFrameInfo                [OUT]           鍥惧儚淇℃伅缁撴瀯浣?
 *  @param  nMsec                       [IN]            绛夊緟瓒呮椂鏃堕棿
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 姣忔璋冪敤璇ユ帴鍙ｏ紝灏嗘煡璇㈠唴閮ㄧ紦瀛樻槸鍚︽湁鏁版嵁锛屽鏋滄湁鏁版嵁鍒欒浆鎹㈡垚RGB24鏍煎紡杩斿洖锛屽鏋滄病鏈夋暟鎹垯杩斿洖閿欒鐮併€傚洜涓哄浘鍍忚浆鎹㈡垚RGB24鏍煎紡鏈夎€楁椂锛屾墍浠ュ綋鏁版嵁甯х巼杩囬珮鏃惰鎺ュ彛鍙兘浼氬鑷翠涪甯с€俓n 
             璋冪敤璇ユ帴鍙ｈ幏鍙栧浘鍍忔暟鎹抚涔嬪墠闇€瑕佸厛璋冪敤MV_CC_StartGrabbing鍚姩鍥惧儚閲囬泦銆傝鎺ュ彛涓轰富鍔ㄥ紡鑾峰彇甯ф暟鎹紝涓婂眰搴旂敤绋嬪簭闇€瑕佹牴鎹抚鐜囷紝鎺у埗濂借皟鐢ㄨ鎺ュ彛鐨勯鐜囥€?\n
             璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧銆?
 
 *  @~english
 *  @brief  Get one frame of RGB data, this function is using query to get data
            query whether the internal cache has data, get data if there has, return error code if no data
 *  @param  handle                      [IN]            Device handle
 *  @param  pData                       [OUT]           Image data receiving buffer
 *  @param  nDataSize                   [IN]            Buffer size
 *  @param  pstFrameInfo                [OUT]           Image information structure
 *  @param  nMsec                       [IN]            Waiting timeout
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Each time the API is called, the internal cache is checked for data. If there is data, it will be transformed as RGB24 format for return, if there is no data, return error code. As time-consuming exists when transform the image to RGB24 format, this API may cause frame loss when the data frame rate is too high. \n
             Before calling this API to get image data frame, call MV_CC_StartGrabbing to start image acquisition. This API can get frame data actively, the upper layer program should control the frequency of calling this API according to the frame rate. \n
             This API is not supported by CameraLink device.
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetImageForRGB(IN void* handle, IN OUT unsigned char * pData , IN unsigned int nDataSize, IN OUT MV_FRAME_OUT_INFO_EX* pstFrameInfo, int nMsec);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇涓€甯GR鏁版嵁锛屾鍑芥暟涓烘煡璇㈠紡鑾峰彇锛屾瘡娆¤皟鐢ㄦ煡璇㈠唴閮?
            缂撳瓨鏈夋棤鏁版嵁锛屾湁鏁版嵁鍒欒幏鍙栨暟鎹紝鏃犳暟鎹繑鍥為敊璇爜
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pData                       [OUT]           鍥惧儚鏁版嵁鎺ユ敹鎸囬拡
 *  @param  nDataSize                   [IN]            鎺ユ敹缂撳瓨澶у皬
 *  @param  pstFrameInfo                [OUT]           鍥惧儚淇℃伅缁撴瀯浣?
 *  @param  nMsec                       [IN]            绛夊緟瓒呮椂鏃堕棿
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 姣忔璋冪敤璇ユ帴鍙ｏ紝灏嗘煡璇㈠唴閮ㄧ紦瀛樻槸鍚︽湁鏁版嵁锛屽鏋滄湁鏁版嵁鍒欒浆鎹㈡垚BGR24鏍煎紡杩斿洖锛屽鏋滄病鏈夋暟鎹垯杩斿洖閿欒鐮併€傚洜涓哄浘鍍忚浆鎹㈡垚BGR24鏍煎紡鏈夎€楁椂锛屾墍浠ュ綋鏁版嵁甯х巼杩囬珮鏃惰鎺ュ彛鍙兘浼氬鑷翠涪甯с€?\n
             璋冪敤璇ユ帴鍙ｈ幏鍙栧浘鍍忔暟鎹抚涔嬪墠闇€瑕佸厛璋冪敤MV_CC_StartGrabbing鍚姩鍥惧儚閲囬泦銆傝鎺ュ彛涓轰富鍔ㄥ紡鑾峰彇甯ф暟鎹紝涓婂眰搴旂敤绋嬪簭闇€瑕佹牴鎹抚鐜囷紝鎺у埗濂借皟鐢ㄨ鎺ュ彛鐨勯鐜囥€俓n 
             璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧銆?
 
 *  @~english
 *  @brief  Get one frame of BGR data, this function is using query to get data
            query whether the internal cache has data, get data if there has, return error code if no data
 *  @param  handle                      [IN]            Device handle
 *  @param  pData                       [OUT]           Image data receiving buffer
 *  @param  nDataSize                   [IN]            Buffer size
 *  @param  pstFrameInfo                [OUT]           Image information structure
 *  @param  nMsec                       [IN]            Waiting timeout
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Before calling this API to set image data callback function, you should call this API MV_CC_CreateHandle. \n
             There are two image acquisition modes, the two modes cannot be reused: \n
             Mode 1: Call MV_CC_RegisterImageCallBackForBGR to set RGB24 format image data callback function, and then call MV_CC_StartGrabbing to start acquisition, the collected image data will be returned in the configured callback function.\n
             Mode 2: Call MV_CC_StartGrabbing to start acquisition, and the call MV_CC_GetImageForBGR repeatedly in application layer to get frame data with BGR24 format. When getting frame data, the upper application program should control the frequency of calling this API according to frame rate. \n
             This API is not supported by CameraLink device.
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetImageForBGR(IN void* handle, IN OUT unsigned char * pData , IN unsigned int nDataSize, IN OUT MV_FRAME_OUT_INFO_EX* pstFrameInfo, int nMsec);

/********************************************************************//**
 *  @~chinese
 *  @brief  浣跨敤鍐呴儴缂撳瓨鑾峰彇涓€甯у浘鐗囷紙涓嶮V_CC_Display涓嶈兘鍚屾椂浣跨敤锛?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstFrame                    [OUT]           鍥惧儚鏁版嵁鍜屽浘鍍忎俊鎭?
 *  @param  nMsec                       [IN]            绛夊緟瓒呮椂鏃堕棿锛岃緭鍏NFINITE鏃惰〃绀烘棤闄愮瓑寰咃紝鐩村埌鏀跺埌涓€甯ф暟鎹垨鑰呭仠姝㈠彇娴?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璋冪敤璇ユ帴鍙ｈ幏鍙栧浘鍍忔暟鎹抚涔嬪墠闇€瑕佸厛璋冪敤MV_CC_StartGrabbing鍚姩鍥惧儚閲囬泦銆傝鎺ュ彛涓轰富鍔ㄥ紡鑾峰彇甯ф暟鎹紝涓婂眰搴旂敤绋嬪簭闇€瑕佹牴鎹抚鐜囷紝鎺у埗濂借皟鐢ㄨ鎺ュ彛鐨勯鐜囥€傝鎺ュ彛鏀寔璁剧疆瓒呮椂鏃堕棿锛孲DK鍐呴儴绛夊緟鐩村埌鏈夋暟鎹椂杩斿洖锛屽彲浠ュ鍔犲彇娴佸钩绋虫€э紝閫傚悎鐢ㄤ簬瀵瑰钩绋虫€ц姹傝緝楂樼殑鍦哄悎銆?\n
             璇ユ帴鍙ｄ笌MV_CC_FreeImageBuffer閰嶅浣跨敤锛屽綋澶勭悊瀹屽彇鍒扮殑鏁版嵁鍚庯紝闇€瑕佺敤MV_CC_FreeImageBuffer鎺ュ彛灏唒Frame鍐呯殑鏁版嵁鎸囬拡鏉冮檺杩涜閲婃斁銆?\n
             璇ユ帴鍙ｄ笌MV_CC_GetOneFrameTimeout鐩告瘮锛屾湁鐫€鏇撮珮鐨勬晥鐜囥€備笖鍏跺彇娴佺紦瀛樼殑鍒嗛厤鏄敱sdk鍐呴儴鑷姩鍒嗛厤鐨勶紝鑰孧V_CC_GetOneFrameTimeout鎺ュ彛鏄渶瑕佸鎴疯嚜琛屽垎閰嶃€俓n 
             璇ユ帴鍙ｅ湪璋冪敤MV_CC_Display鍚庢棤娉曞彇娴併€?\n
             璇ユ帴鍙ｅ浜嶶3V銆丟IGE璁惧鍧囧彲鏀寔銆?\n
             璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧銆?
 
 *  @~english
 *  @brief  Get a frame of an image using an internal cache
 *  @param  handle                      [IN]            Device handle
 *  @param  pstFrame                    [OUT]           Image data and image information
 *  @param  nMsec                       [IN]            Waiting timeout
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Before calling this API to get image data frame, you should call MV_CC_StartGrabbing to start image acquisition. This API can get frame data actively, the upper layer program should control the frequency of calling this API according to the frame rate. This API support setting timeout, and SDK will wait to return until data appears. This function will increase the streaming stability, which can be used in the situation with high stability requirement. \n
             This API and MV_CC_FreeImageBuffer should be called in pairs, after processing the acquired data, you should call MV_CC_FreeImageBuffer to release the data pointer permission of pFrame. \n
             This interface is more efficient than MV_CC_GetOneFrameTimeout. The allocation of the stream cache is automatically allocated within the SDK.The MV_CC_GetOneFrameTimeout interface needs to be allocated by customers themselves. \n
             This API cannot be called to stream after calling MV_CC_Display. \n
             This API is not supported by CameraLink device. \n
             This API is supported by both USB3 vision camera and GigE camera. \n
 **********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetImageBuffer(IN void* handle, OUT MV_FRAME_OUT* pstFrame, IN int nMsec);

/********************************************************************//**
 *  @~chinese
 *  @brief  閲婃斁鍥惧儚缂撳瓨(姝ゆ帴鍙ｇ敤浜庨噴鏀句笉鍐嶄娇鐢ㄧ殑鍥惧儚缂撳瓨锛屼笌MV_CC_GetImageBuffer閰嶅浣跨敤)
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstFrame                    [IN]            鍥惧儚鏁版嵁鍜屽浘鍍忔暟鎹?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｄ笌MV_CC_GetImageBuffer閰嶅浣跨敤锛屼娇鐢∕V_CC_GetImageBuffer鎺ュ彛鍙栧埌鐨勫浘鍍忔暟鎹畃Frame锛岄渶瑕佺敤MV_CC_FreeImageBuffer鎺ュ彛杩涜鏉冮檺閲婃斁銆?\n
             璇ユ帴鍙ｅ浜庡彇娴佹晥鐜囬珮浜嶨etOneFrameTimeout鎺ュ彛锛屼笖GetImageBuffer鍦ㄤ笉杩涜Free鐨勬儏鍐典笅锛屾渶澶ф敮鎸佽緭鍑虹殑鑺傜偣鏁颁笌SetImageNode鎺ュ彛鎵€璁剧疆鐨勮妭鐐规暟鐩哥瓑锛岄粯璁よ妭鐐规暟鏄?銆俓n 
             璇ユ帴鍙ｅ浜嶶3V銆丟IGE璁惧鍧囧彲鏀寔銆?\n
             璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧銆?
 
 *  @~english
 *  @brief  Free image buffer(this interface can free image buffer, used with MV_CC_GetImageBuffer)
 *  @param  handle                      [IN]            Device handle
 *  @param  pstFrame                    [IN]            Image data and image information
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This API and MV_CC_GetImageBuffer should be called in pairs, before calling MV_CC_GetImageBuffer to get image data pFrame, you should call MV_CC_FreeImageBuffer to release the permission. \n
             Compared with API MV_CC_GetOneFrameTimeout, this API has higher efficiency of image acquisition. The max. number of nodes can be outputted is same as the "nNum" of API MV_CC_SetImageNodeNum, default value is 1. \n
             This API is not supported by CameraLink device. \n
             This API is supported by both USB3 vision camera and GigE camera. 
 **********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_FreeImageBuffer(IN void* handle, IN MV_FRAME_OUT* pstFrame);

/********************************************************************//**
 *  @~chinese
 *  @brief  閲囩敤瓒呮椂鏈哄埗鑾峰彇涓€甯у浘鐗囷紝SDK鍐呴儴绛夊緟鐩村埌鏈夋暟鎹椂杩斿洖
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pData                       [OUT]           鍥惧儚鏁版嵁鎺ユ敹鎸囬拡
 *  @param  nDataSize                   [IN]            鎺ユ敹缂撳瓨澶у皬
 *  @param  pstFrameInfo                [OUT]           鍥惧儚淇℃伅缁撴瀯浣?
 *  @param  nMsec                       [IN]            绛夊緟瓒呮椂鏃堕棿
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璋冪敤璇ユ帴鍙ｈ幏鍙栧浘鍍忔暟鎹抚涔嬪墠闇€瑕佸厛璋冪敤MV_CC_StartGrabbing鍚姩鍥惧儚閲囬泦銆傝鎺ュ彛涓轰富鍔ㄥ紡鑾峰彇甯ф暟鎹紝涓婂眰搴旂敤绋嬪簭闇€瑕佹牴鎹抚鐜囷紝鎺у埗濂借皟鐢ㄨ鎺ュ彛鐨勯鐜囥€傝鎺ュ彛鏀寔璁剧疆瓒呮椂鏃堕棿锛孲DK鍐呴儴绛夊緟鐩村埌鏈夋暟鎹椂杩斿洖锛屽彲浠ュ鍔犲彇娴佸钩绋虫€э紝閫傚悎鐢ㄤ簬瀵瑰钩绋虫€ц姹傝緝楂樼殑鍦哄悎銆俓n 
             璇ユ帴鍙ｅ浜嶶3V銆丟IGE璁惧鍧囧彲鏀寔銆俓n 
             璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧銆?
 
 *  @~english
 *  @brief  Timeout mechanism is used to get image, and the SDK waits inside until the data is returned
 *  @param  handle                      [IN]            Device handle
 *  @param  pData                       [OUT]           Image data receiving buffer
 *  @param  nDataSize                   [IN]            Buffer size
 *  @param  pstFrameInfo                [OUT]           Image information structure
 *  @param  nMsec                       [IN]            Waiting timeout
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Before calling this API to get image data frame, call MV_CC_StartGrabbing to start image acquisition. This API can get frame data actively, the upper layer program should control the frequency of calling this API according to the frame rate. This API supports setting timeout, SDK will wait to return until data appears. This function will increase the streaming stability, which can be used in the situation with high stability requirement. \n
             Both the USB3Vision and GIGE camera can support this API. \n
             This API is not supported by CameraLink device.
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetOneFrameTimeout(IN void* handle, IN OUT unsigned char * pData , IN unsigned int nDataSize, IN OUT MV_FRAME_OUT_INFO_EX* pstFrameInfo, unsigned int nMsec);

/********************************************************************//**
 *  @~chinese
 *  @brief  娓呴櫎鍙栨祦鏁版嵁缂撳瓨
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｅ厑璁哥敤鎴峰湪涓嶅仠姝㈠彇娴佺殑鏃跺€欙紝灏辫兘娓呴櫎缂撳瓨涓笉闇€瑕佺殑鍥惧儚銆俓n
             璇ユ帴鍙ｅ湪杩炵画妯″紡鍒囪Е鍙戞ā寮忓悗锛屽彲浠ユ竻闄ゅ巻鍙叉暟鎹€?
 
 *  @~english
 *  @brief  if Image buffers has retrieved the data锛孋lear them
 *  @param  handle                      [IN]            Device handle
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This interface allows user to clear the unnecessary images from the buffer memory without stopping acquisition. \n
             This interface allows user to clear previous data after switching from continuous mode to trigger mode. 
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_ClearImageBuffer(IN void* handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  鏄剧ず涓€甯у浘鍍?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstDisplayInfo              [IN]            鍥惧儚淇℃伅
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｅ浜嶶3V銆丟IGE璁惧鍧囧彲鏀寔銆俓n 
             璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧銆?
 
 *  @~english
 *  @brief  Display one frame image
 *  @param  handle                      [IN]            Device handle
 *  @param  pstDisplayInfo              [IN]            Frame Info
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This API is valid for USB3Vision camera and GIGE camera. \n
             This API is not supported by CameraLink device. 
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_DisplayOneFrame(IN void* handle, IN MV_DISPLAY_FRAME_INFO* pstDisplayInfo);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆SDK鍐呴儴鍥惧儚缂撳瓨鑺傜偣涓暟锛屽ぇ浜庣瓑浜?锛屽湪鎶撳浘鍓嶈皟鐢?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nNum                        [IN]            缂撳瓨鑺傜偣涓暟
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璋冪敤璇ユ帴鍙ｅ彲浠ヨ缃甋DK鍐呴儴鍥惧儚缂撳瓨鑺傜偣涓暟锛屽湪璋冪敤MV_CC_StartGrabbing寮€濮嬫姄鍥惧墠璋冪敤銆俓n
             鍦⊿DK涓粯璁ゆ槸1涓妭鐐广€俓n
             璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧銆?
 
 *  @~english
 *  @brief  Set the number of the internal image cache nodes in SDK, Greater than or equal to 1, to be called before the capture
 *  @param  handle                      [IN]            Device handle
 *  @param  nNum                        [IN]            Image Node Number
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Call this interface to set the number of SDK internal image buffer nodes. The interface should be called before calling MV_CC_StartGrabbing for capturing. \n
             This API is not supported by CameraLink device. 
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetImageNodeNum(IN void* handle, unsigned int nNum);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆鍙栨祦绛栫暐
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  enGrabStrategy              [IN]            绛栫暐鏋氫妇鍊?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｅ畾涔変簡鍥涚鍙栨祦绛栫暐锛岀敤鎴峰彲浠ユ牴鎹疄闄呴渶姹傝繘琛岄€夋嫨銆傚叿浣撴弿杩板涓嬶細
             -OneByOne:浠庢棫鍒版柊涓€甯т竴甯х殑浠庤緭鍑虹紦瀛樺垪琛ㄤ腑鑾峰彇鍥惧儚锛屾墦寮€璁惧鍚庨粯璁や负璇ョ瓥鐣?
             -LatestImagesOnly:浠呬粠杈撳嚭缂撳瓨鍒楄〃涓幏鍙栨渶鏂扮殑涓€甯у浘鍍忥紝鍚屾椂娓呯┖杈撳嚭缂撳瓨鍒楄〃
             -LatestImages:浠庤緭鍑虹紦瀛樺垪琛ㄤ腑鑾峰彇鏈€鏂扮殑OutputQueueSize甯у浘鍍忥紝鍏朵腑OutputQueueSize鑼冨洿涓?-ImageNodeNum锛屽彲鐢∕V_CC_SetOutputQueueSize鎺ュ彛璁剧疆锛孖mageNodeNum榛樿涓?锛屽彲鐢∕V_CC_SetImageNodeNum鎺ュ彛璁剧疆 OutputQueueSize璁剧疆鎴?绛夊悓浜嶭atestImagesOnly绛栫暐锛孫utputQueueSize璁剧疆鎴怚mageNodeNum绛夊悓浜嶰neByOne绛栫暐
             -UpcomingImage:鍦ㄨ皟鐢ㄥ彇娴佹帴鍙ｆ椂蹇界暐杈撳嚭缂撳瓨鍒楄〃涓墍鏈夊浘鍍忥紝骞剁瓑寰呰澶囧嵆灏嗙敓鎴愮殑涓€甯у浘鍍忋€傝绛栫暐鍙敮鎸丟igE璁惧锛屼笉鏀寔U3V璁惧
 
 *  @~english
 *  @brief  Set Grab Strategy
 *  @param  handle                      [IN]            Device handle
 *  @param  enGrabStrategy              [IN]            The value of Grab Strategy
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This interface is set by four image acquisition approaches, the user may choose one as needed. Specific details are as followed: 
             -OneByOne:Obtain image from output cache list frame by frame in order, this function is default strategy when device is on.
             -LatestImagesOnly:Obtain the latest image from output cache list only, meanwhile clear output cache list.
             -LatestImages:Obtain the latest OutputQueueSize image from output cache list, the range of OutputQueueSize is 1-ImageNodeNum, the user may set the value of MV_CC_SetOutputQueueSizeinterface,the default value of ImageNodeNum is 1,If the user usesMV_CC_SetImageNodeNuminterface to set up OutputQueueSize,when the value of OutputQueueSize is set to be 1, the function will be same as LatestImagesOnly; if the value of OutputQueueSize is set to be ImageNodeNum, the function will be same as OneByOne.
             -UpcomingImage:Ignore all images in output cache list when calling image acuiqisiotn interface, wait the next upcoming image generated. Support for GigE camera only, not support for U3V camera.
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetGrabStrategy(IN void* handle, IN MV_GRAB_STRATEGY enGrabStrategy);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆杈撳嚭缂撳瓨涓暟锛堝彧鏈夊湪MV_GrabStrategy_LatestImages绛栫暐涓嬫墠鏈夋晥锛岃寖鍥达細1-ImageNodeNum锛?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nOutputQueueSize            [IN]            杈撳嚭缂撳瓨涓暟
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｉ渶涓嶭atestImages鍙栨祦绛栫暐閰嶅璋冪敤锛岀敤浜庤缃甃atestImages绛栫暐涓嬫渶澶氬厑璁哥紦瀛樺浘鍍忕殑涓暟銆傚彲浠ュ湪鍙栨祦杩囩▼涓姩鎬佽皟鑺傝緭鍑虹紦瀛樹釜鏁?
 
 *  @~english
 *  @brief  Set The Size of Output Queue(Only work under the strategy of MV_GrabStrategy_LatestImages锛宺ang锛?-ImageNodeNum)
 *  @param  handle                      [IN]            Device handle
 *  @param  nOutputQueueSize            [IN]            The Size of Output Queue
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This interface must be used with LatestImages Grab strategy, it is used for setting the maximum allowance queue size of the image under the LatestImages strategy. The user may change the output queue size while grabbing images.
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetOutputQueueSize(IN void* handle, IN unsigned int nOutputQueueSize);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇璁惧淇℃伅锛屽彇娴佷箣鍓嶈皟鐢?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstDevInfo                  [IN][OUT]       杩斿洖缁欒皟鐢ㄨ€呮湁鍏宠澶囦俊鎭粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 鏀寔鐢ㄦ埛鍦ㄦ墦寮€璁惧鍚庤幏鍙栬澶囦俊鎭€俓n 
             鑻ヨ璁惧鏄疓igE璁惧锛屽垯璋冪敤璇ユ帴鍙ｅ瓨鍦ㄩ樆濉為闄╋紝鍥犳涓嶅缓璁湪鍙栨祦杩囩▼涓皟鐢ㄨ鎺ュ彛銆?
 
 *  @~english
 *  @brief  Get device information
 *  @param  handle                      [IN]            Device handle
 *  @param  pstDevInfo                  [IN][OUT]       Structure pointer of device information
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks The API support users to access device information after opening the device. \n
             If the device is a GigE camera, there is a blocking risk in calling the interface, so it is not recommended to call the interface during the fetching process. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetDeviceInfo(IN void * handle, IN OUT MV_CC_DEVICE_INFO* pstDevInfo);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇鍚勭绫诲瀷鐨勪俊鎭?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstInfo                     [IN][OUT]       杩斿洖缁欒皟鐢ㄨ€呮湁鍏宠澶囧悇绉嶇被鍨嬬殑淇℃伅缁撴瀯浣撴寚閽?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 鎺ュ彛閲岄潰杈撳叆闇€瑕佽幏鍙栫殑淇℃伅绫诲瀷锛堟寚瀹歁V_ALL_MATCH_INFO缁撴瀯浣撲腑鐨刵Type绫诲瀷锛夛紝鑾峰彇瀵瑰簲鐨勪俊鎭紙鍦∕V_ALL_MATCH_INFO缁撴瀯浣撲腑pInfo閲岃繑鍥烇級銆?\n
             璇ユ帴鍙ｇ殑璋冪敤鍓嶇疆鏉′欢鍙栧喅浜庢墍鑾峰彇鐨勪俊鎭被鍨嬶紝鑾峰彇GigE璁惧鐨凪V_MATCH_TYPE_NET_DETECT淇℃伅闇€鍦ㄥ紑鍚姄鍥句箣鍚庤皟鐢紝鑾峰彇U3V璁惧鐨凪V_MATCH_TYPE_USB_DETECT淇℃伅闇€鍦ㄦ墦寮€璁惧涔嬪悗璋冪敤銆?\n
             璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧銆?
 
 *  @~english
 *  @brief  Get various type of information
 *  @param  handle                      [IN]            Device handle
 *  @param  pstInfo                     [IN][OUT]       Structure pointer of various type of information
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Input required information type (specify nType in structure MV_ALL_MATCH_INFO) in the interface and get corresponding information (return in pInfo of structure MV_ALL_MATCH_INFO). \n
             The calling precondition of this interface is determined by obtained information type. Call after enabling capture to get MV_MATCH_TYPE_NET_DETECT information of GigE device, and call after starting device to get MV_MATCH_TYPE_USB_DETECT information of USB3Vision device. \n
             This API is not supported by CameraLink device. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetAllMatchInfo(IN void* handle, IN OUT MV_ALL_MATCH_INFO* pstInfo);


/************************************************************************/
/* 璁剧疆鍜岃幏鍙栬澶囧弬鏁扮殑涓囪兘鎺ュ彛                                         */
/* General interface for getting and setting camera parameters          */
/************************************************************************/
/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇Integer灞炴€у€?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strKey                      [IN]            灞炴€ч敭鍊硷紝濡傝幏鍙栧搴︿俊鎭垯涓?Width"
 *  @param  pstIntValue                 [IN][OUT]       杩斿洖缁欒皟鐢ㄨ€呮湁鍏宠澶囧睘鎬х粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 杩炴帴璁惧涔嬪悗璋冪敤璇ユ帴鍙ｅ彲浠ヨ幏鍙杋nt绫诲瀷鐨勬寚瀹氳妭鐐圭殑鍊笺€?
 
 *  @~english
 *  @brief  Get Integer value
 *  @param  handle                      [IN]            Device handle
 *  @param  strKey                      [IN]            Key value, for example, using "Width" to get width
 *  @param  pstIntValue                 [IN][OUT]       Structure pointer of camera features
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks You can call this API to get the value of camera node with integer type after connecting the device. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetIntValueEx(IN void* handle,IN const char* strKey,OUT MVCC_INTVALUE_EX *pstIntValue);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆Integer鍨嬪睘鎬у€?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strKey                      [IN]            灞炴€ч敭鍊硷紝濡傝幏鍙栧搴︿俊鎭垯涓?Width"
 *  @param  nValue                      [IN]            鎯宠璁剧疆鐨勮澶囩殑灞炴€у€?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 杩炴帴璁惧涔嬪悗璋冪敤璇ユ帴鍙ｅ彲浠ヨ缃甶nt绫诲瀷鐨勬寚瀹氳妭鐐圭殑鍊笺€?
 
 *  @~english
 *  @brief  Set Integer value
 *  @param  handle                      [IN]            Device handle
 *  @param  strKey                      [IN]            Key value, for example, using "Width" to set width
 *  @param  nValue                      [IN]            Feature value to set
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks You can call this API to get the value of camera node with integer type after connecting the device. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetIntValueEx(IN void* handle,IN const char* strKey,IN int64_t nValue);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇Enum灞炴€у€?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strKey                      [IN]            灞炴€ч敭鍊硷紝濡傝幏鍙栧儚绱犳牸寮忎俊鎭垯涓?PixelFormat"
 *  @param  pstEnumValue                [IN][OUT]       杩斿洖缁欒皟鐢ㄨ€呮湁鍏宠澶囧睘鎬х粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 杩炴帴璁惧涔嬪悗璋冪敤璇ユ帴鍙ｅ彲浠ヨ幏鍙朎num绫诲瀷鐨勬寚瀹氳妭鐐圭殑鍊笺€?
 
 *  @~english
 *  @brief  Get Enum value
 *  @param  handle                      [IN]            Device handle
 *  @param  strKey                      [IN]            Key value, for example, using "PixelFormat" to get pixel format
 *  @param  pstEnumValue                [IN][OUT]       Structure pointer of camera features
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After the device is connected, call this interface to get specified Enum nodes. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetEnumValue(IN void* handle,IN const char* strKey,OUT MVCC_ENUMVALUE *pstEnumValue);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆Enum鍨嬪睘鎬у€?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strKey                      [IN]            灞炴€ч敭鍊硷紝濡傝幏鍙栧儚绱犳牸寮忎俊鎭垯涓?PixelFormat"
 *  @param  nValue                      [IN]            鎯宠璁剧疆鐨勮澶囩殑灞炴€у€?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 杩炴帴璁惧涔嬪悗璋冪敤璇ユ帴鍙ｅ彲浠ヨ缃瓻num绫诲瀷鐨勬寚瀹氳妭鐐圭殑鍊笺€?
 
 *  @~english
 *  @brief  Set Enum value
 *  @param  handle                      [IN]            Device handle
 *  @param  strKey                      [IN]            Key value, for example, using "PixelFormat" to set pixel format
 *  @param  nValue                      [IN]            Feature value to set
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After the device is connected, call this interface to get specified Enum nodes. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetEnumValue(IN void* handle,IN const char* strKey,IN unsigned int nValue);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆Enum鍨嬪睘鎬у€?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strKey                      [IN]            灞炴€ч敭鍊硷紝濡傝幏鍙栧儚绱犳牸寮忎俊鎭垯涓?PixelFormat"
 *  @param  strValue                    [IN]            鎯宠璁剧疆鐨勮澶囩殑灞炴€у瓧绗︿覆
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 杩炴帴璁惧涔嬪悗璋冪敤璇ユ帴鍙ｅ彲浠ヨ缃瓻num绫诲瀷鐨勬寚瀹氳妭鐐圭殑鍊笺€?
 
 *  @~english
 *  @brief  Set Enum value
 *  @param  handle                      [IN]            Device handle
 *  @param  strKey                      [IN]            Key value, for example, using "PixelFormat" to set pixel format
 *  @param  strValue                    [IN]            Feature String to set
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Call this API after connecting the device. All the values of nodes with IEnumeration type can be set via this API.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetEnumValueByString(IN void* handle,IN const char* strKey,IN const char* strValue);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇Float灞炴€у€?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strKey                      [IN]            灞炴€ч敭鍊?
 *  @param  pstFloatValue               [IN][OUT]       杩斿洖缁欒皟鐢ㄨ€呮湁鍏宠澶囧睘鎬х粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 杩炴帴璁惧涔嬪悗璋冪敤璇ユ帴鍙ｅ彲浠ヨ幏鍙杅loat绫诲瀷鐨勬寚瀹氳妭鐐圭殑鍊笺€?
 
 *  @~english
 *  @brief  Get Float value
 *  @param  handle                      [IN]            Device handle
 *  @param  strKey                      [IN]            Key value
 *  @param  pstFloatValue               [IN][OUT]       Structure pointer of camera features
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After the device is connected, call this interface to get specified float node. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetFloatValue(IN void* handle,IN const char* strKey,OUT MVCC_FLOATVALUE *pstFloatValue);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆float鍨嬪睘鎬у€?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strKey                      [IN]            灞炴€ч敭鍊?
 *  @param  fValue                      [IN]            鎯宠璁剧疆鐨勮澶囩殑灞炴€у€?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 杩炴帴璁惧涔嬪悗璋冪敤璇ユ帴鍙ｅ彲浠ヨ缃甪loat绫诲瀷鐨勬寚瀹氳妭鐐圭殑鍊笺€?
 
 *  @~english
 *  @brief  Set float value
 *  @param  handle                      [IN]            Device handle
 *  @param  strKey                      [IN]            Key value
 *  @param  fValue                      [IN]            Feature value to set
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After the device is connected, call this interface to set specified float node. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetFloatValue(IN void* handle,IN const char* strKey,IN float fValue);
	
/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇Boolean灞炴€у€?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strKey                      [IN]            灞炴€ч敭鍊?
 *  @param  pbValue                     [IN][OUT]       杩斿洖缁欒皟鐢ㄨ€呮湁鍏宠澶囧睘鎬у€?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 杩炴帴璁惧涔嬪悗璋冪敤璇ユ帴鍙ｅ彲浠ヨ幏鍙朾ool绫诲瀷鐨勬寚瀹氳妭鐐圭殑鍊笺€?
 
 *  @~english
 *  @brief  Get Boolean value
 *  @param  handle                      [IN]            Device handle
 *  @param  strKey                      [IN]            Key value
 *  @param  pbValue                     [IN][OUT]       Structure pointer of camera features
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After the device is connected, call this interface to get specified bool nodes. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetBoolValue(IN void* handle,IN const char* strKey,OUT bool *pbValue);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆Boolean鍨嬪睘鎬у€?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strKey                      [IN]            灞炴€ч敭鍊?
 *  @param  bValue                      [IN]            鎯宠璁剧疆鐨勮澶囩殑灞炴€у€?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 杩炴帴璁惧涔嬪悗璋冪敤璇ユ帴鍙ｅ彲浠ヨ缃産ool绫诲瀷鐨勬寚瀹氳妭鐐圭殑鍊笺€?
 
 *  @~english
 *  @brief  Set Boolean value
 *  @param  handle                      [IN]            Device handle
 *  @param  strKey                      [IN]            Key value
 *  @param  bValue                      [IN]            Feature value to set
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After the device is connected, call this interface to set specified bool nodes. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetBoolValue(IN void* handle,IN const char* strKey,IN bool bValue);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇String灞炴€у€?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strKey                      [IN]            灞炴€ч敭鍊?
 *  @param  pstStringValue              [IN][OUT]       杩斿洖缁欒皟鐢ㄨ€呮湁鍏宠澶囧睘鎬х粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 杩炴帴璁惧涔嬪悗璋冪敤璇ユ帴鍙ｅ彲浠ヨ幏鍙杝tring绫诲瀷鐨勬寚瀹氳妭鐐圭殑鍊笺€?
 
 *  @~english
 *  @brief  Get String value
 *  @param  handle                      [IN]            Device handle
 *  @param  strKey                      [IN]            Key value
 *  @param  pstStringValue              [IN][OUT]       Structure pointer of camera features
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After the device is connected, call this interface to get specified string nodes. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetStringValue(IN void* handle,IN const char* strKey,OUT MVCC_STRINGVALUE *pstStringValue);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆String鍨嬪睘鎬у€?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strKey                      [IN]            灞炴€ч敭鍊?
 *  @param  strValue                    [IN]            鎯宠璁剧疆鐨勮澶囩殑灞炴€у€?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 杩炴帴璁惧涔嬪悗璋冪敤璇ユ帴鍙ｅ彲浠ヨ缃畇tring绫诲瀷鐨勬寚瀹氳妭鐐圭殑鍊笺€?
 
 *  @~english
 *  @brief  Set String value
 *  @param  handle                      [IN]            Device handle
 *  @param  strKey                      [IN]            Key value
 *  @param  strValue                    [IN]            Feature value to set
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After the device is connected, call this interface to set specified string nodes. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetStringValue(IN void* handle,IN const char* strKey,IN const char * strValue);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆Command鍨嬪睘鎬у€?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strKey                      [IN]            灞炴€ч敭鍊?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 杩炴帴璁惧涔嬪悗璋冪敤璇ユ帴鍙ｅ彲浠ヨ缃寚瀹氱殑Command绫诲瀷鑺傜偣銆?
 
 *  @~english
 *  @brief  Send Command
 *  @param  handle                      [IN]            Device handle
 *  @param  strKey                      [IN]            Key value
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After the device is connected, call this interface to set specified Command nodes. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetCommandValue(IN void* handle,IN const char* strKey);

/********************************************************************//**
 *  @~chinese
 *  @brief  娓呴櫎GenICam鑺傜偣缂撳瓨
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @~english
 *  @brief  Invalidate GenICam Nodes
 *  @param  handle                      [IN]            Device handle
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_InvalidateNodes(IN void* handle);


/************************************************************************/
/* 璁惧鍗囩骇 鍜?瀵勫瓨鍣ㄨ鍐?鍜屽紓甯搞€佷簨浠跺洖璋?                           	*/
/* Device upgrade, register read and write and exception callback       */
/************************************************************************/
/********************************************************************//**
 *  @~chinese
 *  @brief  璁惧鏈湴鍗囩骇
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strFilePathName             [IN]            鏂囦欢鍚?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 閫氳繃璇ユ帴鍙ｅ彲浠ュ皢鍗囩骇鍥轰欢鏂囦欢鍙戦€佺粰璁惧杩涜鍗囩骇銆傝鎺ュ彛闇€瑕佺瓑寰呭崌绾у浐浠舵枃浠舵垚鍔熶紶缁欒澶囩涔嬪悗鍐嶈繑鍥烇紝鍝嶅簲鏃堕棿鍙兘杈冮暱銆?
 
 *  @~english
 *  @brief  Device Local Upgrade
 *  @param  handle                      [IN]            Device handle
 *  @param  strFilePathName             [IN]            File name
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Call this API to send the upgrade firmware to the device for upgrade. This API will wait for return until the upgrade firmware is sent to the device, this response may take a long time. \n
             For CameraLink device, it keeps sending upgrade firmware continuously. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_LocalUpgrade(IN void* handle, const void *strFilePathName);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇鍗囩骇杩涘害
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pnProcess                   [OUT]           杩涘害鎺ユ敹鍦板潃
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 
 *  @~english
 *  @brief  Get Upgrade Progress
 *  @param  handle                      [IN]            Device handle
 *  @param  pnProcess                   [OUT]           Progress receiving address
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetUpgradeProcess(IN void* handle, unsigned int* pnProcess);

/********************************************************************//**
 *  @~chinese
 *  @brief  璇诲唴瀛?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pBuffer                     [IN][OUT]       浣滀负杩斿洖鍊间娇鐢紝淇濆瓨璇诲埌鐨勫唴瀛樺€硷紙GEV璁惧鍐呭瓨鍊兼槸鎸夌収澶х妯″紡瀛樺偍鐨勶紝鍏跺畠鍗忚璁惧鎸夌収灏忕瀛樺偍锛?
 *  @param  nAddress                    [IN]            寰呰鍙栫殑鍐呭瓨鍦板潃锛岃鍦板潃鍙互浠庤澶囩殑Camera.xml鏂囦欢涓幏鍙栵紝褰㈠xxx_RegAddr鐨剎ml鑺傜偣鍊?
 *  @param  nLength                     [IN]            寰呰鍙栫殑鍐呭瓨闀垮害
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 璁块棶璁惧锛岃鍙栨煇娈靛瘎瀛樺櫒鐨勬暟鎹€?
 
 *  @~english
 *  @brief  Read Memory
 *  @param  handle                      [IN]            Device Handle
 *  @param  pBuffer                     [IN][OUT]       Used as a return value, save the read-in memory value ( Memory value is stored in accordance with the big end model)
 *  @param  nAddress                    [IN]            Memory address to be read, which can be obtained from the Camera.xml file of the device, the form xml node value of xxx_RegAddr
 *  @param  nLength                     [IN]            Length of the memory to be read
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks Access device, read the data from certain register.
*************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_ReadMemory(IN void* handle , void *pBuffer, int64_t nAddress, int64_t nLength);

/********************************************************************//**
 *  @~chinese
 *  @brief  鍐欏唴瀛?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pBuffer                     [IN]            寰呭啓鍏ョ殑鍐呭瓨鍊硷紙娉ㄦ剰GEV璁惧鍐呭瓨鍊艰鎸夌収澶х妯″紡瀛樺偍锛屽叾瀹冨崗璁澶囨寜鐓у皬绔瓨鍌級
 *  @param  nAddress                    [IN]            寰呭啓鍏ョ殑鍐呭瓨鍦板潃锛岃鍦板潃鍙互浠庤澶囩殑Camera.xml鏂囦欢涓幏鍙栵紝褰㈠xxx_RegAddr鐨剎ml鑺傜偣鍊?
 *  @param  nLength                     [IN]            寰呭啓鍏ョ殑鍐呭瓨闀垮害
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 璁块棶璁惧锛屾妸涓€娈垫暟鎹啓鍏ユ煇娈靛瘎瀛樺櫒銆?
 
 *  @~english
 *  @brief  Write Memory
 *  @param  handle                      [IN]            Device Handle
 *  @param  pBuffer                     [IN]            Memory value to be written ( Note the memory value to be stored in accordance with the big end model)
 *  @param  nAddress                    [IN]            Memory address to be written, which can be obtained from the Camera.xml file of the device, the form xml node value of xxx_RegAddr
 *  @param  nLength                     [IN]            Length of the memory to be written
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks Access device, write a piece of data into a certain segment of register.
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_WriteMemory(IN void* handle , const void *pBuffer, int64_t nAddress, int64_t nLength);

/********************************************************************//**
 *  @~chinese
 *  @brief  娉ㄥ唽寮傚父娑堟伅鍥炶皟锛屽湪鎵撳紑璁惧涔嬪悗璋冪敤
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  cbException                 [IN]            寮傚父鍥炶皟鍑芥暟鎸囬拡
 *  @param  pUser                       [IN]            鐢ㄦ埛鑷畾涔夊彉閲?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｉ渶瑕佸湪MV_CC_OpenDevice鎵撳紑璁惧涔嬪悗璋冪敤銆傝澶囧紓甯告柇寮€杩炴帴鍚庡彲浠ュ湪鍥炶皟閲岄潰鑾峰彇鍒板紓甯告秷鎭紝GigE璁惧鎺夌嚎涔嬪悗闇€瑕佸厛璋冪敤MV_CC_CloseDevice鎺ュ彛鍏抽棴璁惧锛屽啀璋冪敤MV_CC_OpenDevice鎺ュ彛閲嶆柊鎵撳紑璁惧銆?
 
 *  @~english
 *  @brief  Register Exception Message CallBack, call after open device
 *  @param  handle                      [IN]            Device handle
 *  @param  cbException                 [IN]            Exception Message CallBack Function Pointer
 *  @param  pUser                       [IN]            User defined variable
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks Call this interface after the device is opened by MV_CC_OpenDevice. When device is exceptionally disconnected, the exception message can be obtained from callback function. For Disconnected GigE device, first call MV_CC_CloseDevice to shut device, and then call MV_CC_OpenDevice to reopen the device. 
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_RegisterExceptionCallBack(IN void* handle, 
                                                             void(__stdcall* cbException)(unsigned int nMsgType, void* pUser), void* pUser);

/********************************************************************//**
 *  @~chinese
 *  @brief  娉ㄥ唽鍏ㄩ儴浜嬩欢鍥炶皟锛屽湪鎵撳紑璁惧涔嬪悗璋冪敤
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  cbEvent                     [IN]            浜嬩欢鍥炶皟鍑芥暟鎸囬拡
 *  @param  pUser                       [IN]            鐢ㄦ埛鑷畾涔夊彉閲?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 閫氳繃璇ユ帴鍙ｈ缃簨浠跺洖璋冿紝鍙互鍦ㄥ洖璋冨嚱鏁伴噷闈㈣幏鍙栭噰闆嗐€佹洕鍏夌瓑浜嬩欢淇℃伅銆?\n
             璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧銆?
 
 *  @~english
 *  @brief  Register event callback, which is called after the device is opened
 *  @param  handle                      [IN]            Device handle
 *  @param  cbEvent                     [IN]            Event CallBack Function Pointer
 *  @param  pUser                       [IN]            User defined variable
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks Call this API to set the event callback function to get the event information, e.g., acquisition, exposure, and so on. \n
             This API is not supported by CameraLink device.
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_RegisterAllEventCallBack(void* handle, void(__stdcall* cbEvent)(MV_EVENT_OUT_INFO * pEventInfo, void* pUser), void* pUser);

/********************************************************************//**
 *  @~chinese
 *  @brief  娉ㄥ唽鍗曚釜浜嬩欢鍥炶皟锛屽湪鎵撳紑璁惧涔嬪悗璋冪敤
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strEventName                [IN]            浜嬩欢鍚嶇О
 *  @param  cbEvent                     [IN]            浜嬩欢鍥炶皟鍑芥暟鎸囬拡
 *  @param  pUser                       [IN]            鐢ㄦ埛鑷畾涔夊彉閲?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 閫氳繃璇ユ帴鍙ｈ缃簨浠跺洖璋冿紝鍙互鍦ㄥ洖璋冨嚱鏁伴噷闈㈣幏鍙栭噰闆嗐€佹洕鍏夌瓑浜嬩欢淇℃伅銆俓n 
             璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧锛屼粎鏀寔"璁惧鎺夌嚎"杩欎竴绉嶄簨浠躲€?
 
 *  @~english
 *  @brief  Register single event callback, which is called after the device is opened
 *  @param  handle                      [IN]            Device handle
 *  @param  strEventName                [IN]            Event name
 *  @param  cbEvent                     [IN]            Event CallBack Function Pointer
 *  @param  pUser                       [IN]            User defined variable
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks Call this API to set the event callback function to get the event information, e.g., acquisition, exposure, and so on. \n
             This API is not supported by CameraLink device .
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_RegisterEventCallBackEx(void* handle, const char* strEventName,
                                                         void(__stdcall* cbEvent)(MV_EVENT_OUT_INFO * pEventInfo, void* pUser), void* pUser);


/************************************************************************/
/* GigEVision 璁惧鐙湁鐨勬帴鍙?                                    		*/
/* GigEVision device specific interface                                 */
/************************************************************************/
/********************************************************************//**
 *  @~chinese
 *  @brief  寮哄埗IP
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nIP                         [IN]            璁剧疆鐨処P
 *  @param  nSubNetMask                 [IN]            瀛愮綉鎺╃爜
 *  @param  nDefaultGateWay             [IN]            榛樿缃戝叧
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 寮哄埗璁剧疆璁惧缃戠粶鍙傛暟锛堝寘鎷琁P銆佸瓙缃戞帺鐮併€侀粯璁ょ綉鍏筹級锛屽己鍒惰缃箣鍚庡皢闇€瑕侀噸鏂板垱寤鸿澶囧彞鏌勶紝浠匞igEVision璁惧鏀寔銆俓n
             濡傛灉璁惧涓篋HCP鐨勭姸鎬侊紝璋冪敤璇ユ帴鍙ｅ己鍒惰缃澶囩綉缁滃弬鏁颁箣鍚庤澶囧皢浼氶噸鍚€?
 
 *  @~english
 *  @brief  Force IP
 *  @param  handle                      [IN]            Device handle
 *  @param  nIP                         [IN]            IP to set
 *  @param  nSubNetMask                 [IN]            Subnet mask
 *  @param  nDefaultGateWay             [IN]            Default gateway
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks Force setting camera network parameter (including IP address, subnet mask, default gateway). After forced setting, device handle should be created again. This function is only supported by GigEVision camera.\n
             If device is in DHCP status, after calling this API to force setting camera network parameter, the device will restart.
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_ForceIpEx(IN void* handle, unsigned int nIP, unsigned int nSubNetMask, unsigned int nDefaultGateWay);

/********************************************************************//**
 *  @~chinese
 *  @brief  閰嶇疆IP鏂瑰紡
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nType                       [IN]            IP绫诲瀷锛岃MV_IP_CFG_x
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 鍙戦€佸懡浠よ缃澶囩殑IP鏂瑰紡锛屽DHCP銆丩LA绛夛紝浠匞igEVision璁惧鏀寔銆?
 
 *  @~english
 *  @brief  IP configuration method
 *  @param  handle                      [IN]            Device handle
 *  @param  nType                       [IN]            IP type, refer to MV_IP_CFG_x
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks Send command to set camera IP mode, such as DHCP and LLA, only supported by GigEVision.
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_SetIpConfig(IN void* handle, unsigned int nType);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆浠呬娇鐢ㄦ煇绉嶆ā寮?type: MV_NET_TRANS_x锛屼笉璁剧疆鏃讹紝榛樿浼樺厛浣跨敤driver
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nType                       [IN]            缃戠粶浼犺緭妯″紡锛岃MV_NET_TRANS_x
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 閫氳繃璇ユ帴鍙ｅ彲浠ヨ缃甋DK鍐呴儴浼樺厛浣跨敤鐨勭綉缁滄ā寮忥紝榛樿浼樺厛浣跨敤椹卞姩妯″紡锛屼粎GigEVision璁惧鏀寔銆?
 
 *  @~english
 *  @brief  Set to use only one mode,type: MV_NET_TRANS_x. When do not set, priority is to use driver by default
 *  @param  handle                      [IN]            Device handle
 *  @param  nType                       [IN]            Net transmission mode, refer to MV_NET_TRANS_x
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarksSet SDK internal priority network mode through this interface, drive mode by default, only supported by GigEVision camera.
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_SetNetTransMode(IN void* handle, unsigned int nType);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇缃戠粶浼犺緭淇℃伅
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstInfo                     [OUT]           淇℃伅缁撴瀯浣?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 閫氳繃璇ユ帴鍙ｅ彲浠ヨ幏鍙栫綉缁滀紶杈撶浉鍏充俊鎭紝鍖呮嫭宸叉帴鏀舵暟鎹ぇ灏忋€佷涪甯ф暟閲忕瓑锛屽湪MV_CC_StartGrabbing寮€鍚噰闆嗕箣鍚庤皟鐢ㄣ€備粎GigEVision璁惧鏀寔銆?
 
 *  @~english
 *  @brief  Get net transmission information
 *  @param  handle                      [IN]            Device handle
 *  @param  pstInfo                     [OUT]           Information Structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Get network transmission information through this API, including received data size, number of lost frames. Call this API after starting image acquiring through MV_CC_StartGrabbing. This API is supported only by GigEVision Camera.
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_GetNetTransInfo(IN void* handle, MV_NETTRANS_INFO* pstInfo);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆鏋氫妇鍛戒护鐨勫洖澶嶅寘绫诲瀷
 *  @param  nMode                       [IN]            鍥炲鍖呯被鍨嬶紙榛樿骞挎挱锛夛紝0-鍗曟挱锛?-骞挎挱
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｅ彧瀵笹igE鐩告満鏈夋晥銆?

 *  @~english
 *  @brief  Setting the ACK mode of devices Discovery.
 *  @param  nMode                       [IN]            ACK mode锛圖efault-Broadcast锛?0-Unicast,1-Broadcast.
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks This interface is ONLY effective on GigE cameras. \n
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_SetDiscoveryMode(unsigned int nMode);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆GVSP鍙栨祦瓒呮椂鏃堕棿
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nMillisec                   [IN]            瓒呮椂鏃堕棿锛岄粯璁?00ms锛岃寖鍥达細>10ms
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 杩炴帴璁惧涔嬪悗锛屽彇娴佸姩浣滃彂鐢熷墠锛岃皟鐢ㄨ鎺ュ彛鍙互璁剧疆GVSP鍙栨祦瓒呮椂鏃堕棿銆侴VSP鍙栨祦瓒呮椂璁剧疆杩囩煭鍙兘閫犳垚鍥惧儚寮傚父锛岃缃繃闀垮彲鑳介€犳垚鍙栨祦鏃堕棿鍙橀暱銆?
 
 *  @~english
 *  @brief  Set GVSP streaming timeout
 *  @param  handle                      [IN]            Device handle
 *  @param  nMillisec                   [IN]            Timeout, default 300ms, range: >10ms
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After the device is connected, and just before start streaming, 
 *           call this interface to set GVSP streaming timeout value.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_SetGvspTimeout(void* handle, unsigned int nMillisec);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇GVSP鍙栨祦瓒呮椂鏃堕棿
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pnMillisec                  [OUT]           瓒呮椂鏃堕棿鎸囬拡锛屼互姣浣嶅崟浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｇ敤浜庤幏鍙栧綋鍓嶇殑GVSP鍙栨祦瓒呮椂鏃堕棿
 
 *  @~english
 *  @brief  Get GVSP streaming timeout
 *  @param  handle                      [IN]            Device handle
 *  @param  pnMillisec                  [OUT]           Timeout, ms as unit
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This interface is used to get the current GVSP streaming timeout.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_GetGvspTimeout(IN void* handle, unsigned int* pnMillisec);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆GVCP鍛戒护瓒呮椂鏃堕棿
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nMillisec                   [IN]            瓒呮椂鏃堕棿锛岄粯璁?00ms锛岃寖鍥达細0-10000ms
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 杩炴帴璁惧涔嬪悗璋冪敤璇ユ帴鍙ｅ彲浠ヨ缃瓽VCP鍛戒护瓒呮椂鏃堕棿銆?
 
 *  @~english
 *  @brief  Set GVCP cammand timeout
 *  @param  handle                      [IN]            Device handle
 *  @param  nMillisec                   [IN]            Timeout, default 500ms, range: 0-10000ms
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After the device is connected, call this interface to set GVCP command timeout time.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_SetGvcpTimeout(void* handle, unsigned int nMillisec);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇GVCP鍛戒护瓒呮椂鏃堕棿
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pnMillisec                  [OUT]           瓒呮椂鏃堕棿鎸囬拡锛屼互姣浣嶅崟浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｇ敤浜庤幏鍙栧綋鍓嶇殑GVCP瓒呮椂鏃堕棿銆?
 
 *  @~english
 *  @brief  Get GVCP cammand timeout
 *  @param  handle                      [IN]            Device handle
 *  @param  pnMillisec                  [OUT]           Timeout, ms as unit
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This interface is used to get the current GVCP timeout.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_GetGvcpTimeout(void* handle, unsigned int* pnMillisec);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆閲嶄紶GVCP鍛戒护娆℃暟
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nRetryGvcpTimes             [IN]            閲嶄紶娆℃暟锛岃寖鍥达細0-100
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｇ敤浜庡湪GVCP鍖呬紶杈撳紓甯告椂锛屽鍔犻噸浼犵殑娆℃暟锛屽湪涓€瀹氱▼搴︿笂鍙互閬垮厤璁惧鎺夌嚎锛岃寖鍥翠负0-100銆?
 
 *  @~english
 *  @brief  Set the number of retry GVCP cammand
 *  @param  handle                      [IN]            Device handle
 *  @param  nRetryGvcpTimes             [IN]            The number of retries锛宺ang锛?-100
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This interface is used to increase The Times of retransmission when GVCP packet transmission is abnormal,and to some extent, it can avoid dropping the camera, with a range of 0-100.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_SetRetryGvcpTimes(void* handle, unsigned int nRetryGvcpTimes);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇閲嶄紶GVCP鍛戒护娆℃暟
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pnRetryGvcpTimes            [OUT]           閲嶄紶娆℃暟鎸囬拡
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｇ敤浜庤幏鍙栧綋鍓嶇殑GVCP閲嶄紶娆℃暟锛岄粯璁?娆°€?
 
 *  @~english
 *  @brief  Get the number of retry GVCP cammand
 *  @param  handle                      [IN]            Device handle
 *  @param  pnRetryGvcpTimes            [OUT]           The number of retries
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This interface is used to get the current number of GVCP retransmissions, which defaults to 3.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_GetRetryGvcpTimes(void* handle, unsigned int* pnRetryGvcpTimes);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇鏈€浣崇殑packet size锛岃鎺ュ彛鐩墠鍙敮鎸丟igE璁惧
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @return 鏈€浣硃acketsize
 *  @remarks 鑾峰彇鏈€浣崇殑packet size锛屽搴擥igEVision璁惧鏄疭CPS锛屽搴擴3V璁惧鏄瘡娆′粠椹卞姩璇诲彇鐨勫寘澶у皬锛岃澶у皬鍗崇綉缁滀笂浼犺緭涓€涓寘鐨勫ぇ灏忋€傝鎺ュ彛闇€瑕佸湪MV_CC_OpenDevice涔嬪悗銆丮V_CC_StartGrabbing涔嬪墠璋冪敤銆?\n
             璇ユ帴鍙ｄ笉鏀寔CameraLink璁惧銆?
 
 *  @~english
 *  @brief  Get the optimal Packet Size, Only support GigE Camera
 *  @param  handle                      [IN]            Device handle
 *  @return Optimal packetsize
 *  @remarks To get optimized packet size, for GigEVision device is SCPS, for USB3Vision device is the size of packet read from drive each time, and it is the size of a packet transported on the network. The interface should be called after MV_CC_OpenDevice and before MV_CC_StartGrabbing. \n
             This API is not supported by CameraLink device. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetOptimalPacketSize(IN void* handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆鏄惁鎵撳紑閲嶅彂鍖呮敮鎸侊紝鍙婇噸鍙戝寘璁剧疆
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  bEnable                     [IN]            鏄惁鏀寔閲嶅彂鍖?
 *  @param  nMaxResendPercent           [IN]            鏈€澶ч噸鍙戞瘮
 *  @param  nResendTimeout              [IN]            閲嶅彂瓒呮椂鏃堕棿
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 杩炴帴璁惧涔嬪悗璋冪敤璇ユ帴鍙ｅ彲浠ヨ缃噸鍙戝寘灞炴€э紝浠匞igEVision璁惧鏀寔銆?
 
 *  @~english
 *  @brief  Set whethe to enable resend, and set resend
 *  @param  handle                      [IN]            Device handle
 *  @param  bEnable                     [IN]            enable resend
 *  @param  nMaxResendPercent           [IN]            Max resend persent
 *  @param  nResendTimeout              [IN]            Resend timeout
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After the device is connected, call this interface to set resend packet properties, only supported by GigEVision camera.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_SetResend(void* handle, unsigned int bEnable, unsigned int nMaxResendPercent, unsigned int nResendTimeout);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆閲嶄紶鍛戒护鏈€澶у皾璇曟鏁?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nRetryTimes                 [IN]            閲嶄紶鍛戒护鏈€澶у皾璇曟鏁帮紝榛樿20
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｅ繀椤诲湪璋冪敤MV_GIGE_SetResend寮€鍚噸浼犲寘鍔熻兘涔嬪悗璋冪敤锛屽惁鍒欏け璐ヤ笖杩斿洖MV_E_CALLORDER
 
 *  @~english
 *  @brief  set the max resend retry times
 *  @param  handle                      [IN]            Device handle
 *  @param  nRetryTimes                 [IN]            The max times to retry resending lost packets锛宒efault 20
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This interface MUST be called after enabling resending lost packets by calling MV_GIGE_SetResend, \n
 *           otherwise would fail and return MV_E_CALLORDER.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall  MV_GIGE_SetResendMaxRetryTimes (void* handle, unsigned int nRetryTimes);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇閲嶄紶鍛戒护鏈€澶у皾璇曟鏁?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pnRetryTimes                [OUT]           閲嶄紶鍛戒护鏈€澶у皾璇曟鏁?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｅ繀椤诲湪璋冪敤MV_GIGE_SetResend寮€鍚噸浼犲寘鍔熻兘涔嬪悗璋冪敤锛屽惁鍒欏け璐ヤ笖杩斿洖MV_E_CALLORDER
 
 *  @~english
 *  @brief  get the max resend retry times
 *  @param  handle                      [IN]            Device handle
 *  @param  pnRetryTimes                [OUT]           The max times to retry resending lost packets
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This interface MUST be called after enabling resending lost packets by calling MV_GIGE_SetResend, \n
 *           otherwise would fail and return MV_E_CALLORDER. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall  MV_GIGE_GetResendMaxRetryTimes (void* handle, unsigned int* pnRetryTimes);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆鍚屼竴閲嶄紶鍖呭娆¤姹備箣闂寸殑鏃堕棿闂撮殧
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nMillisec                   [IN]            鍚屼竴閲嶄紶鍖呭娆¤姹備箣闂寸殑鏃堕棿闂撮殧锛岄粯璁?0ms
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｅ繀椤诲湪璋冪敤MV_GIGE_SetResend寮€鍚噸浼犲寘鍔熻兘涔嬪悗璋冪敤锛屽惁鍒欏け璐ヤ笖杩斿洖MV_E_CALLORDER
 
 *  @~english
 *  @brief  set time interval between same resend requests
 *  @param  handle                      [IN]            Device handle
 *  @param  nMillisec                   [IN]            The time interval between same resend requests,default 10ms
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This interface MUST be called after enabling resending lost packets by calling MV_GIGE_SetResend, \n
 *           otherwise would fail and return MV_E_CALLORDER. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall  MV_GIGE_SetResendTimeInterval (void* handle, unsigned int nMillisec);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇鍚屼竴閲嶄紶鍖呭娆¤姹備箣闂寸殑鏃堕棿闂撮殧
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pnMillisec                  [OUT]           鍚屼竴閲嶄紶鍖呭娆¤姹備箣闂寸殑鏃堕棿闂撮殧
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｅ繀椤诲湪璋冪敤MV_GIGE_SetResend寮€鍚噸浼犲寘鍔熻兘涔嬪悗璋冪敤锛屽惁鍒欏け璐ヤ笖杩斿洖MV_E_CALLORDER
 
 *  @~english
 *  @brief  get time interval between same resend requests
 *  @param  handle                      [IN]            Device handle
 *  @param  pnMillisec                  [OUT]           The time interval between same resend requests
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This interface MUST be called after enabling resending lost packets by calling MV_GIGE_SetResend, \n
 *           otherwise would fail and return MV_E_CALLORDER. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall  MV_GIGE_GetResendTimeInterval (void* handle, unsigned int* pnMillisec);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆浼犺緭妯″紡锛屽彲浠ヤ负鍗曟挱妯″紡銆佺粍鎾ā寮忕瓑
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  stTransmissionType          [IN]            浼犺緭妯″紡缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 閫氳繃璇ユ帴鍙ｅ彲浠ヨ缃紶杈撴ā寮忎负鍗曟挱銆佺粍鎾瓑妯″紡锛屼粎GigEVision璁惧鏀寔銆?

 *  @~english
 *  @brief  Set transmission type,Unicast or Multicast
 *  @param  handle                      [IN]            Device handle
 *  @param  stTransmissionType          [IN]            Struct of transmission type
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Call this API to set the transmission mode as single cast mode and multicast mode. And this API is only valid for GigEVision camera. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_SetTransmissionType(void* handle, MV_TRANSMISSION_TYPE * pstTransmissionType);

/********************************************************************//**
 *  @~chinese
 *  @brief   鍙戝嚭鍔ㄤ綔鍛戒护
 *  @param   pstActionCmdInfo           [IN]            鍔ㄤ綔鍛戒护淇℃伅
 *  @param   pstActionCmdResults        [OUT]           鍔ㄤ綔鍛戒护杩斿洖淇℃伅鍒楄〃
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 浠匞igEVision璁惧鏀寔銆?

 *  @~english
 *  @brief  Issue Action Command
 *  @param   pstActionCmdInfo           [IN]            Action Command
 *  @param   pstActionCmdResults        [OUT]           Action Command Result List
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This API is supported only by GigEVision camera.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_IssueActionCommand(IN MV_ACTION_CMD_INFO* pstActionCmdInfo, OUT MV_ACTION_CMD_RESULT_LIST* pstActionCmdResults);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇缁勬挱鐘舵€?
 *  @param  pstDevInfo                  [IN]            璁惧淇℃伅缁撴瀯浣?
 *  @param  pbStatus                    [OUT]           缁勬挱鐘舵€?true:缁勬挱鐘舵€侊紝false:闈炵粍鎾?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｇ敤浜庡垽鏂澶囧綋鍓嶆槸鍚﹀浜庣粍鎾姸鎬侊紝瑙ｅ喅瀹㈡埛绔灇涓炬椂闇€瑕佹墦寮€璁惧鍒ゆ柇缁勬挱鐨勯棶棰樸€?

 *  @~english
 *  @brief  Get Multicast Status
 *  @param  pstDevInfo                  [IN]            Device Information Structure
 *  @param  pbStatus                    [OUT]           Status of Multicast
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This interface is used to determine whether the camera is currently in multicast state, and to solve the problem that the client needs to turn on the camera to determine multicast when enumerating.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_GetMulticastStatus(IN MV_CC_DEVICE_INFO* pstDevInfo, OUT bool* pbStatus);


/************************************************************************/
/* CameraLink 璁惧鐙湁鐨勬帴鍙?                                           */
/************************************************************************/
/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆璁惧娉㈢壒鐜?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nBaudrate                   [IN]            璁剧疆鐨勬尝鐗圭巼鍊硷紝鏁板€煎弬鑰僀ameraParams.h涓畯瀹氫箟锛屽#define MV_CAML_BAUDRATE_9600  0x00000001
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｆ敮鎸佸湪璁惧鏈繛鎺ユ椂璋冪敤銆?
 
 *  @~english
 *  @brief  Set device baudrate using one of the CL_BAUDRATE_XXXX value
 *  @param  handle                      [IN]            Device handle
 *  @param  nBaudrate                   [IN]            baud rate to set. Refer to the 'CameraParams.h' for parameter definitions, for example, #define MV_CAML_BAUDRATE_9600  0x00000001
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks This API is supported only by CameraLink device.\n
             This API support calls when devices are not connected.
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CAML_SetDeviceBaudrate(IN void* handle, unsigned int nBaudrate);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇璁惧娉㈢壒鐜?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pnCurrentBaudrate           [OUT]           娉㈢壒鐜囦俊鎭寚閽堬紝鏁板€煎弬鑰僀ameraParams.h涓畯瀹氫箟锛屽#define MV_CAML_BAUDRATE_9600  0x00000001
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｆ敮鎸佸湪璁惧鏈繛鎺ユ椂璋冪敤銆?
 
 *  @~english
 *  @brief  Returns the current device baudrate, using one of the CL_BAUDRATE_XXXX value
 *  @param  handle                      [IN]            Device handle
 *  @param  pnCurrentBaudrate           [OUT]           Return pointer of baud rate to user. Refer to the 'CameraParams.h' for parameter definitions, for example, #define MV_CAML_BAUDRATE_9600  0x00000001
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks This API is supported only by CameraLink device.\n
             This API support calls when devices are not connected.
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CAML_GetDeviceBaudrate(IN void* handle,unsigned int* pnCurrentBaudrate);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇璁惧涓庝富鏈洪棿杩炴帴鏀寔鐨勬尝鐗圭巼
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pnBaudrateAblity            [OUT]           鏀寔鐨勬尝鐗圭巼淇℃伅鐨勬寚閽堛€傛墍鏀寔娉㈢壒鐜囩殑鎴栬繍绠楃粨鏋滐紝鍗曚釜鏁板€煎弬鑰僀ameraParams.h涓畯瀹氫箟锛屽MV_CAML_BAUDRATE_9600  0x00000001
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｆ敮鎸佸湪璁惧鏈繛鎺ユ椂璋冪敤銆?
 
 *  @~english
 *  @brief  Returns supported baudrates of the combined device and host interface
 *  @param  handle                      [IN]            Device handle
 *  @param  pnBaudrateAblity            [OUT]           Return pointer of the supported baudrates to user. 'OR' operation results of the supported baudrates. Refer to the 'CameraParams.h' for single value definitions, for example, MV_CAML_BAUDRATE_9600  0x00000001
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks This API is supported only by CameraLink device.\n
             This API support calls when devices are not connected.
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CAML_GetSupportBaudrates(IN void* handle,unsigned int* pnBaudrateAblity);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆涓插彛鎿嶄綔绛夊緟鏃堕暱
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nMillisec                   [IN]            涓插彛鎿嶄綔鐨勭瓑寰呮椂闀? 鍗曚綅涓簃s
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?

 *  @~english
 *  @brief  Sets the timeout for operations on the serial port
 *  @param  handle                      [IN]            Device handle
 *  @param  nMillisec                   [IN]            Timeout in [ms] for operations on the serial port.
 *  @return Success, return MV_OK. Failure, return error code 
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CAML_SetGenCPTimeOut(IN void* handle, unsigned int nMillisec);


/************************************************************************/
/* U3V 璁惧鐙湁鐨勬帴鍙?                                           */
/************************************************************************/
/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆U3V鐨勪紶杈撳寘澶у皬
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nTransferSize               [IN]            浼犺緭鐨勫寘澶у皬, Byte锛岄粯璁や负1M锛宺ang锛?=0x10000
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 澧炲姞浼犺緭鍖呭ぇ灏忓彲浠ラ€傚綋闄嶄綆鍙栨祦鏃剁殑CPU鍗犵敤鐜囥€備絾涓嶅悓鐨凱C鍜屼笉鍚孶SB鎵╁睍鍗″瓨鍦ㄤ笉鍚岀殑鍏煎鎬э紝濡傛灉璇ュ弬鏁拌缃繃澶у彲鑳戒細鍑虹幇鍙栦笉鍒板浘鍍忕殑椋庨櫓銆?
 
 *  @~english
 *  @brief  Set transfer size of U3V device
 *  @param  handle                      [IN]            Device handle
 *  @param  nTransferSize               [IN]            Transfer size锛孊yte锛宒efault锛?M锛宺ang锛?=0x10000
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks Increasing the transmission packet size can reduce the CPU utilization at the time of fetching. However, different PCS and different USB extension CARDS have different compatibility, and if this parameter is set too large, there may be the risk of not getting the image.
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_USB_SetTransferSize(IN void* handle, unsigned int nTransferSize);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇U3V鐨勪紶杈撳寘澶у皬
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pnTransferSize              [OUT]           浼犺緭鐨勫寘澶у皬鎸囬拡, Byte
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｇ敤浜庤幏鍙栧綋鍓嶇殑U3V浼犺緭鍖呭ぇ灏忥紝榛樿1M銆?
 
 *  @~english
 *  @brief  Get transfer size of U3V device
 *  @param  handle                      [IN]            Device handle
 *  @param  pnTransferSize              [OUT]           Transfer size锛孊yte
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks This interface is used to get the current U3V transfer packet size, default 1M.
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_USB_GetTransferSize(IN void* handle, unsigned int* pnTransferSize);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆U3V鐨勪紶杈撻€氶亾涓暟
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nTransferWays               [IN]            浼犺緭閫氶亾涓暟锛岃寖鍥达細1-10
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 鐢ㄦ埛鍙互鏍规嵁PC鐨勬€ц兘銆佽澶囧嚭鍥惧抚鐜囥€佸浘鍍忓ぇ灏忓拰鍐呭瓨浣跨敤鐜囩瓑鍥犵礌瀵硅鍙傛暟杩涜璋冭妭銆備絾涓嶅悓鐨凱C鍜屼笉鍚岀殑USB鎵╁睍鍗″瓨鍦ㄤ笉鍚岀殑鍏煎鎬с€?
 
 *  @~english
 *  @brief  Set transfer ways of U3V device
 *  @param  handle                      [IN]            Device handle
 *  @param  nTransferWays               [IN]            Transfer ways锛宺ang锛?-10
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks Users can adjust this parameter according to PC performance, camera image frame rate, image size, memory utilization and other factors. But different PCS and different USB expansion CARDS have different compatibility.
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_USB_SetTransferWays(IN void* handle, unsigned int nTransferWays);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇U3V鐨勪紶杈撻€氶亾涓暟
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pnTransferWays              [OUT]           浼犺緭閫氶亾涓暟鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｇ敤浜庤幏鍙栧綋鍓嶇殑U3V寮傛鍙栨祦鑺傜偣涓暟锛?000W璁惧鐨凪ONO8榛樿涓?涓紝YUV涓洪粯璁?涓紝RGB涓洪粯璁?涓紝鍏跺畠鎯呭喌榛樿8涓妭鐐广€?
 
 *  @~english
 *  @brief  Get transfer ways of U3V device
 *  @param  handle                      [IN]            Device handle
 *  @param  pnTransferWays              [OUT]           Transfer ways
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks This interface is used to get the current number of U3V asynchronous feed nodes. For 2000W camera, MONO8 defaults to 3, YUV defaults to 2, RGB defaults to 1, and other cases default to 8 nodes.
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_USB_GetTransferWays(IN void* handle, unsigned int* pnTransferWays);

/************************************************************************/
/* GenTL鐩稿叧鎺ュ彛锛屽叾瀹冩帴鍙ｅ彲浠ュ鐢紙閮ㄥ垎鎺ュ彛涓嶆敮鎸侊級                    */
/************************************************************************/
/********************************************************************//**
 *  @~chinese
 *  @brief  閫氳繃GenTL鏋氫妇Interfaces
 *  @param  pstIFList                   [OUT]           Interfaces鍒楄〃
 *  @param  strGenTLPath                [IN]            GenTL鐨刢ti鏂囦欢璺緞
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks Interfaces鍒楄〃鐨勫唴瀛樻槸鍦⊿DK鍐呴儴鍒嗛厤鐨勶紝澶氱嚎绋嬭皟鐢ㄨ鎺ュ彛鏃朵細杩涜璁惧鍒楄〃鍐呭瓨鐨勯噴鏀惧拰鐢宠锛孿n
             寤鸿灏介噺閬垮厤澶氱嚎绋嬫灇涓炬搷浣溿€?
 
 *  @~english
 *  @brief  Enumerate Interfaces with GenTL
 *  @param  pstIFList                   [OUT]           Interfaces List
 *  @param  strGenTLPath                [IN]            GenTL cti file path
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks The memory of the Interfaces list is allocated within the SDK. When the interface is invoked by multiple threads, the memory of the device list will be released and applied.\n
             It is recommended to avoid multithreaded enumeration operations as much as possible.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_EnumInterfacesByGenTL(IN OUT MV_GENTL_IF_INFO_LIST* pstIFList, IN const char * strGenTLPath);

/********************************************************************//**
 *  @~chinese
 *  @brief  閫氳繃GenTL Interface鏋氫妇璁惧
 *  @param  pstIFInfo                   [IN]            Interface淇℃伅
 *  @param  pstDevList                  [OUT]           璁惧鍒楄〃
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璁惧鍒楄〃鐨勫唴瀛樻槸鍦⊿DK鍐呴儴鍒嗛厤鐨勶紝澶氱嚎绋嬭皟鐢ㄨ鎺ュ彛鏃朵細杩涜璁惧鍒楄〃鍐呭瓨鐨勯噴鏀惧拰鐢宠锛孿n
             寤鸿灏介噺閬垮厤澶氱嚎绋嬫灇涓炬搷浣溿€?
 
 *  @~english
 *  @brief  Enumerate Devices with GenTL interface
 *  @param  pstIFInfo                   [IN]            Interface information
 *  @param  pstDevList                  [OUT]           Device List
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks The memory of the list is allocated within the SDK. When the interface is invoked by multiple threads, the memory of the device list will be released and applied.\n
             It is recommended to avoid multithreaded enumeration operations as much as possible.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_EnumDevicesByGenTL(IN MV_GENTL_IF_INFO* pstIFInfo, IN OUT MV_GENTL_DEV_INFO_LIST* pstDevList);

/********************************************************************//**
 *  @~chinese
 *  @brief  閫氳繃GenTL璁惧淇℃伅鍒涘缓璁惧鍙ユ焺
 *  @param  handle                      [OUT]           璁惧鍙ユ焺
 *  @param  pstDevInfo                  [IN]            璁惧淇℃伅缁撴瀯浣撴寚閽?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 鏍规嵁杈撳叆鐨勮澶囦俊鎭紝鍒涘缓搴撳唴閮ㄥ繀椤荤殑璧勬簮鍜屽垵濮嬪寲鍐呴儴妯″潡銆?
 
 *  @~english
 *  @brief  Create Device Handle with GenTL Device Info
 *  @param  handle                      [OUT]           Device handle
 *  @param  pstDevInfo                  [IN]            Device Information
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Create required resources within library and initialize internal module according to input device information.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_CreateHandleByGenTL(OUT void ** handle, IN const MV_GENTL_DEV_INFO* pstDevInfo);


/************************************************************************/
/* XML瑙ｆ瀽鏍戠殑鐢熸垚                                                      */
/* XML parse tree generation                                            */
/************************************************************************/
/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇璁惧灞炴€ф爲XML
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pData                       [OUT]           XML鏁版嵁鎺ユ敹缂撳瓨
 *  @param  nDataSize                   [IN]            鎺ユ敹缂撳瓨澶у皬
 *  @param  pnDataLen                   [OUT]           瀹為檯鏁版嵁澶у皬
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 褰損Data涓篘ULL鎴杗DataSize姣斿疄闄呯殑xml鏂囦欢灏忔椂锛屼笉鎷疯礉鏁版嵁锛岀敱pnDataLen杩斿洖xml鏂囦欢澶у皬锛沑n
             褰損Data涓烘湁鏁堢紦瀛樺湴鍧€锛屼笖缂撳瓨瓒冲澶ф椂锛屾嫹璐濆畬鏁存暟鎹繚瀛樺湪璇ョ紦瀛橀噷闈紝骞剁敱pnDataLen杩斿洖xml鏂囦欢瀹為檯澶у皬銆?
 
 *  @~english
 *  @brief  Get camera feature tree XML
 *  @param  handle                      [IN]            Device handle
 *  @param  pData                       [OUT]           XML data receiving buffer
 *  @param  nDataSize                   [IN]            Buffer size
 *  @param  pnDataLen                   [OUT]           Actual data length
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks When pData is NULL or nDataSize than the actual XML file hours, do not copy the data, returned by pnDataLen XML file size.\n
             When pData is a valid cache address and the cache is large enough, copy the full data into the cache, and pnDataLen returns the actual size of the XML file.
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_XML_GetGenICamXML(IN void* handle, IN OUT unsigned char* pData, IN unsigned int nDataSize, OUT unsigned int* pnDataLen);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰緱褰撳墠鑺傜偣鐨勮闂ā寮?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strName                     [IN]            鑺傜偣鍚嶇О
 *  @param  penAccessMode               [OUT]           鑺傜偣鐨勮闂ā寮?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?

 *  @~english
 *  @brief  Get Access mode of cur node
 *  @param  handle                      [IN]            Device handle
 *  @param  strName                     [IN]            Name of node
 *  @param  penAccessMode               [OUT]           Access mode of the node
 *  @return Success, return MV_OK. Failure, return error code
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_XML_GetNodeAccessMode(IN void* handle, IN const char * pstrName, OUT MV_XML_AccessMode *penAccessMode);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰緱褰撳墠鑺傜偣鐨勭被鍨?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strName                     [IN]            鑺傜偣鍚嶇О
 *  @param  penInterfaceType            [OUT]           鑺傜偣鐨勭被鍨?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｅ彲浠ュ湪璋冪敤涓囪兘鎺ュ彛涔嬪墠锛屾彁鍓嶇煡閬撹妭鐐圭被鍨嬶紝鏂逛究鐢ㄦ埛閫夋嫨鍚堥€傜殑涓囪兘鎺ュ彛杩涜鑺傜偣鍊肩殑璁剧疆鍜岃幏鍙栥€?

 *  @~english
 *  @brief  Get Interface Type of cur node
 *  @param  handle                      [IN]            Device handle
 *  @param  strName                     [IN]            Name of node
 *  @param  penInterfaceType            [OUT]           Interface Type of the node
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks The interface can know the node type in advance before calling the universal interface, so as to facilitate users to select the appropriate universal interface for setting and obtaining the node value.
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_XML_GetNodeInterfaceType(IN void* handle, IN const char * pstrName, OUT MV_XML_InterfaceType *penInterfaceType);

/************************************************************************/
/* 闄勫姞鎺ュ彛                                   							*/
/* Additional interface                                                 */
/************************************************************************/
/********************************************************************//**
 *  @~chinese
 *  @brief  淇濆瓨鍥剧墖锛屾敮鎸丅mp鍜孞peg.
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstSaveParam                [IN][OUT]       淇濆瓨鍥剧墖鍙傛暟缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 閫氳繃灏嗘帴鍙ｅ彲浠ュ皢浠庤澶囬噰闆嗗埌鐨勫師濮嬪浘鍍忔暟鎹浆鎹㈡垚JPEG鎴栬€匓MP绛夋牸寮忓苟瀛樻斁鍦ㄦ寚瀹氬唴瀛樹腑锛岀劧鍚庣敤鎴峰彲浠ュ皢杞崲涔嬪悗鐨勬暟鎹洿鎺ヤ繚瀛樻垚鍥剧墖鏂囦欢銆傝鎺ュ彛璋冪敤鏃犳帴鍙ｉ『搴忚姹傦紝鏈夊浘鍍忔簮鏁版嵁灏卞彲浠ヨ繘琛岃浆鎹紝鍙互鍏堣皟鐢∕V_CC_GetOneFrameTimeout鎴栬€匨V_CC_RegisterImageCallBackEx璁剧疆鍥炶皟鍑芥暟锛岃幏鍙栦竴甯у浘鍍忔暟鎹紝鐒跺悗鍐嶉€氳繃璇ユ帴鍙ｈ浆鎹㈡牸寮忋€?\n
             MV_CC_SaveImageEx2姣擬V_CC_SaveImageEx澧炲姞鍙傛暟handle锛屼负浜嗕繚璇佷笌鍏朵粬鎺ュ彛鐨勭粺涓€銆?
 
 *  @~english
 *  @brief  Save image, support Bmp and Jpeg.
 *  @param  handle                      [IN]            Device handle
 *  @param  pstSaveParam                [IN][OUT]       Save image parameters structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Once there is image data, you can call this API to convert the data. \n
             You can also call MV_CC_GetOneFrameTimeout or MV_CC_RegisterImageCallBackEx or MV_CC_GetImageBuffer to get one image frame and set the callback function, and then call this API to convert the format. \n
             Comparing with the API MV_CC_SaveImageEx, this API added the parameter handle to ensure the unity with other API. 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SaveImageEx2(IN void* handle, MV_SAVE_IMAGE_PARAM_EX* pstSaveParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  淇濆瓨鍥惧儚鍒版枃浠?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstSaveFileParam            [IN][OUT]       淇濆瓨鍥剧墖鏂囦欢鍙傛暟缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｆ敮鎸丅MP/JPEG/PNG/TIFF銆?
 
 *  @~english
 *  @brief  Save the image file.
 *  @param  handle                      [IN]            Device handle
 *  @param  pstSaveFileParam            [IN][OUT]       Save the image file parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This API support BMP/JPEG/PNG/TIFF.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SaveImageToFile(IN void* handle, MV_SAVE_IMG_TO_FILE_PARAM* pstSaveFileParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  淇濆瓨3D鐐逛簯鏁版嵁锛屾敮鎸丳LY銆丆SV鍜孫BJ涓夌鏍煎紡
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstPointDataParam           [IN][OUT]       淇濆瓨鐐逛簯鏁版嵁鍙傛暟缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 3D鏁版嵁鏍煎紡淇濆瓨鎴?D鏂囦欢鏍煎紡锛屾敮鎸丳LY/CSV/OBJ锛?n
             鐩墠鏀寔PixelType_Gvsp_Coord3D_ABC32銆丳ixelType_Gvsp_Coord3D_ABC32f銆丳ixelType_Gvsp_Coord3D_AB32銆丳ixelType_Gvsp_Coord3D_AB32f銆丳ixelType_Gvsp_Coord3D_AC32銆丳ixelType_Gvsp_Coord3D_AC32f,鏆備笉鏀寔鍏朵粬3D鏍煎紡銆?
 
 *  @~english
 *  @brief  Save 3D point data, support PLY銆丆SV and OBJ
 *  @param  handle                      [IN]            Device handle
 *  @param  pstPointDataParam           [IN][OUT]       Save 3D point data parameters structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Save the 3D data format to 3D file format锛宻upport PLY銆丆SV and OBJ锛?n
             only support PixelType_Gvsp_Coord3D_ABC32銆丳ixelType_Gvsp_Coord3D_ABC32f銆丳ixelType_Gvsp_Coord3D_AB32銆丳ixelType_Gvsp_Coord3D_AB32f銆丳ixelType_Gvsp_Coord3D_AC32銆丳ixelType_Gvsp_Coord3D_AC32f,Other 3D format is not supported now.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SavePointCloudData(IN void* handle, MV_SAVE_POINT_CLOUD_PARAM* pstPointDataParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  鍥惧儚鏃嬭浆
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstRotateParam              [IN][OUT]       鍥惧儚鏃嬭浆鍙傛暟缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｅ彧鏀寔MONO8/RGB24/BGR24鏍煎紡鏁版嵁鐨?0/180/270搴︽棆杞€?
 
 *  @~english
 *  @brief  Rotate Image
 *  @param  handle                      [IN]            Device handle
 *  @param  pstRotateParam              [IN][OUT]       Rotate image parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This API only support 90/180/270 rotation of data in the MONO8/RGB24/BGR24 format.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_RotateImage(IN void* handle, IN OUT MV_CC_ROTATE_IMAGE_PARAM* pstRotateParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  鍥惧儚缈昏浆
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstFlipParam                [IN][OUT]       鍥惧儚缈昏浆鍙傛暟缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｅ彧鏀寔MONO8/RGB24/BGR24鏍煎紡鏁版嵁鐨勫瀭鐩村拰姘村钩缈昏浆銆?
 
 *  @~english
 *  @brief  Flip Image
 *  @param  handle                      [IN]            Device handle
 *  @param  pstFlipParam                [IN][OUT]       Flip image parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This API only support vertical and horizontal reverse of data in the MONO8/RGB24/BGR24 format.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_FlipImage(IN void* handle, IN OUT MV_CC_FLIP_IMAGE_PARAM* pstFlipParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  鍍忕礌鏍煎紡杞崲
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstCvtParam                 [IN][OUT]       鍍忕礌鏍煎紡杞崲鍙傛暟缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 閫氳繃灏嗘帴鍙ｅ彲浠ュ皢浠庤澶囬噰闆嗗埌鐨勫師濮嬪浘鍍忔暟鎹浆鎹㈡垚鐢ㄦ埛鎵€闇€鐨勫儚绱犳牸寮忓苟瀛樻斁鍦ㄦ寚瀹氬唴瀛樹腑銆傝鎺ュ彛璋冪敤鏃犳帴鍙ｉ『搴忚姹傦紝鏈夊浘鍍忔簮鏁版嵁灏卞彲浠ヨ繘琛岃浆鎹紝鍙互鍏堣皟鐢∕V_CC_GetOneFrameTimeout鎴栬€匨V_CC_RegisterImageCallBackEx璁剧疆鍥炶皟鍑芥暟锛岃幏鍙栦竴甯у浘鍍忔暟鎹紝鐒跺悗鍐嶉€氳繃璇ユ帴鍙ｈ浆鎹㈡牸寮忋€傚鏋滆澶囧綋鍓嶉噰闆嗗浘鍍忔槸JPEG鍘嬬缉鐨勬牸寮忥紝鍒欎笉鏀寔璋冪敤璇ユ帴鍙ｈ繘琛岃浆鎹€?\n
 
 *  @~english
 *  @brief  Pixel format conversion
 *  @param  handle                      [IN]            Device handle
 *  @param  pstCvtParam                 [IN][OUT]       Convert Pixel Type parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This API is used to transform the collected original data to pixel format and save to specified memory. There is no order requirement to call this API, the transformation will execute when there is image data. First call MV_CC_GetOneFrameTimeout or MV_CC_RegisterImageCallBackEx to set callback function, and get a frame of image data, then call this API to transform the format. \n
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_ConvertPixelType(IN void* handle, IN OUT MV_CC_PIXEL_CONVERT_PARAM* pstCvtParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆鎻掑€肩畻娉曠被鍨?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nBayerCvtQuality            [IN]            Bayer鐨勬彃鍊兼柟娉? 0-蹇€?1-鍧囪　 2-鏈€浼橈紙榛樿涓烘渶浼橈級
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璁剧疆鍐呴儴鍥惧儚杞崲鎺ュ彛鐨凚ayer鎻掑€肩畻娉曠被鍨嬪弬鏁帮紝MV_CC_ConvertPixelType銆丮V_CC_GetImageForRGB/BGR鎺ュ彛鍐呴儴浣跨敤鐨勬彃鍊肩畻娉曟槸璇ユ帴鍙ｆ墍璁惧畾鐨勩€?
 
 *  @~english
 *  @brief  Interpolation algorithm type setting
 *  @param  handle                      [IN]            Device handle
 *  @param  nBayerCvtQuality            [IN]            Bayer interpolation method  0-Fast 1-Equilibrium 2-Optimal
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Set the bell interpolation quality parameters of the internal image conversion interface, and the interpolation algorithm used in the MV_CC_ConvertPixelType and MV_CC_GetImageForRGB/BGR interfaces is set by this interface.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetBayerCvtQuality(IN void* handle, IN unsigned int nBayerCvtQuality);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆Bayer鏍煎紡鐨凣amma鍊?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  fBayerGammaValue            [IN]            Gamma鍊糩0.1,4.0]
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璁剧疆璇ュ€煎悗锛屽湪璋冪敤MV_CC_ConvertPixelType銆丮V_CC_SaveImageEx2鎺ュ彛灏咮ayer8/10/12/16鏍煎紡杞垚RGB24/48锛?RGBA32/64锛孊GR24/48锛孊GRA32/64鏃惰捣鏁堛€?
 
 *  @~english
 *  @brief  Set Gamma value
 *  @param  handle                      [IN]            Device handle
 *  @param  fBayerGammaValue            [IN]            Gamma value[0.1,4.0]
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After setting the value, it work in the calling MV_CC_ConvertPixelType\MV_CC_SaveImageEx2 API convert Bayer8/10/12/16 to RGB24/48锛?RGBA32/64锛孊GR24/48锛孊GRA32/64.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetBayerGammaValue(IN void* handle, IN float fBayerGammaValue);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆Bayer鏍煎紡鐨凣amma淇℃伅
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstGammaParam               [IN]            Gamma淇℃伅   
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璁剧疆璇ヤ俊鎭悗锛屽湪璋冪敤MV_CC_ConvertPixelType銆丮V_CC_SaveImageEx2鎺ュ彛灏咮ayer8/10/12/16鏍煎紡杞垚RGB24/48锛?RGBA32/64锛孊GR24/48锛孊GRA32/64鏃惰捣鏁堛€?

 *  @~english
 *  @brief  Set Gamma param
 *  @param  handle                      [IN]            Device handle
 *  @param  pstGammaParam               [IN]            Gamma param
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After setting the param, it work in the calling MV_CC_ConvertPixelType\MV_CC_SaveImageEx2 API convert Bayer8/10/12/16 to RGB24/48锛?RGBA32/64锛孊GR24/48锛孊GRA32/64.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetBayerGammaParam(IN void* handle, IN MV_CC_GAMMA_PARAM* pstGammaParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆Bayer鏍煎紡鐨凜CM浣胯兘鍜岀煩闃碉紝閲忓寲绯绘暟榛樿1024
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstCCMParam                 [IN]            CCM鍙傛暟
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 寮€鍚疌CM骞惰缃瓹CM鐭╅樀鍚庯紝鍦ㄨ皟鐢∕V_CC_ConvertPixelType銆丮V_CC_SaveImageEx2鎺ュ彛灏咮ayer8/10/12/16鏍煎紡杞垚RGB24/48锛?RGBA32/64锛孊GR24/48锛孊GRA32/64鏃惰捣鏁堛€?
 
 *  @~english
 *  @brief  Set CCM param,Scale default 1024
 *  @param  handle                      [IN]            Device handle
 *  @param  pstCCMParam                 [IN]            CCM parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After enable the color correction and set the color correction matrix, It work in the calling MV_CC_ConvertPixelType\MV_CC_SaveImageEx2 API convert Bayer8/10/12/16 to RGB24/48锛?RGBA32/64锛孊GR24/48锛孊GRA32/64.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetBayerCCMParam(IN void* handle, IN MV_CC_CCM_PARAM* pstCCMParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆Bayer鏍煎紡鐨凜CM浣胯兘鍜岀煩闃?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstCCMParam                 [IN]            CCM鍙傛暟
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 寮€鍚疌CM骞惰缃瓹CM鐭╅樀鍚庯紝鍦ㄨ皟鐢∕V_CC_ConvertPixelType銆丮V_CC_SaveImageEx2鎺ュ彛灏咮ayer8/10/12/16鏍煎紡杞垚RGB24/48锛?RGBA32/64锛孊GR24/48锛孊GRA32/64鏃惰捣鏁堛€?

 *  @~english
 *  @brief  Set CCM param
 *  @param  handle                      [IN]            Device handle
 *  @param  pstCCMParam                 [IN]            CCM parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After enable the color correction and set the color correction matrix, It work in the calling MV_CC_ConvertPixelType\MV_CC_SaveImageEx2 API convert Bayer8/10/12/16 to RGB24/48锛?RGBA32/64锛孊GR24/48锛孊GRA32/64.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetBayerCCMParamEx(IN void* handle, IN MV_CC_CCM_PARAM_EX* pstCCMParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆Bayer鏍煎紡鐨凜LUT浣胯兘鍜屼俊鎭?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstCLUTParam                [IN]            CLUT鍙傛暟
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 寮€鍚疌LUT骞惰缃瓹LUT淇℃伅鍚庯紝鍦ㄨ皟鐢∕V_CC_ConvertPixelType銆丮V_CC_SaveImageEx2鎺ュ彛灏咮ayer8/10/12/16鏍煎紡杞垚RGB24/48锛?RGBA32/64锛孊GR24/48锛孊GRA32/64鏃惰捣鏁堛€?

 *  @~english
 *  @brief  Set CLUT param
 *  @param  handle                      [IN]            Device handle
 *  @param  pstCLUTParam                [IN]            CLUT parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks After enable the CLUT and set CLUT, It work in the calling MV_CC_ConvertPixelType\MV_CC_SaveImageEx2 API convert Bayer8/10/12/16 to RGB24/48锛?RGBA32/64锛孊GR24/48锛孊GRA32/64.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetBayerCLUTParam(IN void* handle, IN MV_CC_CLUT_PARAM* pstCLUTParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  鍥惧儚瀵规瘮搴﹁皟鑺?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstContrastParam            [IN]            瀵规瘮搴﹁皟鑺傚弬鏁?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?

 *  @~english
 *  @brief  Adjust image contrast
 *  @param  handle                      [IN]            Device handle
 *  @param  pstContrastParam            [IN]            Contrast parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_ImageContrast(IN void* handle, IN OUT MV_CC_CONTRAST_PARAM* pstContrastParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  鍥惧儚閿愬寲
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstSharpenParam             [IN]            閿愬寲鍙傛暟
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?

 *  @~english
 *  @brief  Image sharpen
 *  @param  handle                      [IN]            Device handle
 *  @param  pstSharpenParam             [IN]            Sharpen parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_ImageSharpen(IN void* handle, IN OUT MV_CC_SHARPEN_PARAM* pstSharpenParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑹插僵鏍℃锛堝寘鎷珻CM鍜孋LUT锛?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstColorCorrectParam        [IN]            鑹插僵鏍℃鍙傛暟
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｆ敮鎸佸崟鐙珻CM鎴栬€匔LUT锛屼篃鏀寔鍚屾椂杩涜CCM鍜孋LUT锛岀敤鎴峰彲浠ラ€氳繃CCM鍜孋LUT淇℃伅涓殑浣胯兘寮€鍏宠繘琛岄€夋嫨銆?

 *  @~english
 *  @brief  Color Correct(include CCM and CLUT)
 *  @param  handle                      [IN]            Device handle
 *  @param  pstColorCorrectParam        [IN]            Color Correct parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This API supports CCM or CLUT alone, as well as CCM and CLUT at the same time. The user can select by means of the enable switch in CCM and CLUT information.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_ColorCorrect(IN void* handle, IN OUT MV_CC_COLOR_CORRECT_PARAM* pstColorCorrectParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  鍣０浼拌
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstNoiseEstimateParam       [IN]            鍣０浼拌鍙傛暟
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 濡傛灉鐢ㄦ埛閫夋嫨鍏ㄥ浘鍋氬櫔澹颁及璁★紝nROINum鍙緭鍏?锛宲stROIRect鍙疆绌恒€?

 *  @~english
 *  @brief  Noise Estimate
 *  @param  handle                      [IN]            Device handle
 *  @param  pstNoiseEstimateParam       [IN]            Noise Estimate parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks If the user selects the full image, nROINum can be typed with 0 and pstROIRect empty.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_NoiseEstimate(IN void* handle, IN OUT MV_CC_NOISE_ESTIMATE_PARAM* pstNoiseEstimateParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  绌哄煙闄嶅櫔
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstSpatialDenoiseParam      [IN]            绌哄煙闄嶅櫔鍙傛暟
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?

 *  @~english
 *  @brief  Spatial Denoise
 *  @param  handle                      [IN]            Device handle
 *  @param  pstSpatialDenoiseParam      [IN]            Spatial Denoise parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SpatialDenoise(IN void* handle, IN OUT MV_CC_SPATIAL_DENOISE_PARAM* pstSpatialDenoiseParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  LSC鏍囧畾
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstLSCCalibParam            [IN]            鏍囧畾鍙傛暟
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?

 *  @~english
 *  @brief  LSC Calib
 *  @param  handle                      [IN]            Device handle
 *  @param  pstLSCCalibParam            [IN]            LSC Calib parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_LSCCalib(IN void* handle, IN OUT MV_CC_LSC_CALIB_PARAM* pstLSCCalibParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  LSC鏍℃
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstLSCCorrectParam          [IN]            鏍℃鍙傛暟
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?

 *  @~english
 *  @brief  LSC Correct
 *  @param  handle                      [IN]            Device handle
 *  @param  pstLSCCorrectParam          [IN]            LSC Correct parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_LSCCorrect(IN void* handle, IN OUT MV_CC_LSC_CORRECT_PARAM* pstLSCCorrectParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  鏃犳崯瑙ｇ爜
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstDecodeParam              [IN][OUT]       鏃犳崯瑙ｇ爜鍙傛暟缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 灏嗕粠鐩告満涓彇鍒扮殑鏃犳崯鍘嬬缉鐮佹祦瑙ｇ爜鎴愯８鏁版嵁锛屽悓鏃舵敮鎸佽В鏋愬綋鍓嶇浉鏈哄疄鏃跺浘鍍忕殑姘村嵃淇℃伅锛堝鏋滆緭鍏ョ殑鏃犳崯鐮佹祦涓嶆槸褰撳墠鐩告満鎴栬€呬笉鏄疄鏃跺彇娴佺殑锛屽垯姘村嵃瑙ｆ瀽鍙兘寮傚父锛?
 
 *  @~english
 *  @brief  High Bandwidth Decode
 *  @param  handle                      [IN]            Device handle
 *  @param  pstDecodeParam              [IN][OUT]       High Bandwidth Decode parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks Decode the lossless compressed data from the camera into raw data 
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_HB_Decode(IN void* handle, IN OUT MV_CC_HB_DECODE_PARAM* pstDecodeParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  淇濆瓨璁惧灞炴€?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strFileName                 [IN]            灞炴€ф枃浠跺悕
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @~english
 *  @brief  Save camera feature
 *  @param  handle                      [IN]            Device handle
 *  @param  strFileName                 [IN]            File name
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_FeatureSave(IN void* handle, IN const char* strFileName);

/********************************************************************//**
 *  @~chinese
 *  @brief  瀵煎叆璁惧灞炴€?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  strFileName                 [IN]            灞炴€ф枃浠跺悕
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @~english
 *  @brief  Load camera feature
 *  @param  handle                      [IN]            Device handle
 *  @param  strFileName                 [IN]            File name
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_FeatureLoad(IN void* handle, IN const char* strFileName);

/********************************************************************//**
 *  @~chinese
 *  @brief  浠庤澶囪鍙栨枃浠?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstFileAccess               [IN]            鏂囦欢瀛樺彇缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @~english
 *  @brief  Read the file from the camera
 *  @param  handle                      [IN]            Device handle
 *  @param  pstFileAccess               [IN]            File access structure
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_FileAccessRead(IN void* handle, IN MV_CC_FILE_ACCESS * pstFileAccess);

/********************************************************************//**
 *  @~chinese
 *  @brief  灏嗘枃浠跺啓鍏ヨ澶?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstFileAccess               [IN]            鏂囦欢瀛樺彇缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @~english
 *  @brief  Write the file to camera
 *  @param  handle                      [IN]            Device handle
 *  @param  pstFileAccess               [IN]            File access structure
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_FileAccessWrite(IN void* handle, IN MV_CC_FILE_ACCESS * pstFileAccess);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇鏂囦欢瀛樺彇鐨勮繘搴?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstFileAccessProgress       [IN]            杩涘害鍐呭
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?锛堝綋鍓嶆枃浠跺瓨鍙栫殑鐘舵€侊級
 
 *  @~english
 *  @brief  Get File Access Progress 
 *  @param  handle                      [IN]            Device handle
 *  @param  pstFileAccessProgress       [IN]            File access Progress
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetFileAccessProgress(IN void* handle, OUT MV_CC_FILE_ACCESS_PROGRESS * pstFileAccessProgress);

/********************************************************************//**
 *  @~chinese
 *  @brief  寮€濮嬪綍鍍?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstRecordParam              [IN]            褰曞儚鍙傛暟缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @~english
 *  @brief  Start Record
 *  @param  handle                      [IN]            Device handle
 *  @param  pstRecordParam              [IN]            Record param structure
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_StartRecord(IN void* handle, IN MV_CC_RECORD_PARAM* pstRecordParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  杈撳叆褰曞儚鏁版嵁
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstInputFrameInfo           [IN]            褰曞儚鏁版嵁缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @~english
 *  @brief  Input RAW data to Record
 *  @param  handle                      [IN]            Device handle
 *  @param  pstInputFrameInfo           [IN]            Record data structure
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_InputOneFrame(IN void* handle, IN MV_CC_INPUT_FRAME_INFO * pstInputFrameInfo);

/********************************************************************//**
 *  @~chinese
 *  @brief  鍋滄褰曞儚
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @~english
 *  @brief  Stop Record
 *  @param  handle                      [IN]            Device handle
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_StopRecord(IN void* handle);


/************************************************************************/
/* 涓嶅缓璁娇鐢ㄧ殑鎺ュ彛                     	                    		*/
/* Interfaces not recommended                                           */
/************************************************************************/
/************************************************************************
 *  @fn     MV_CC_GetImageInfo
 *  @brief  鑾峰彇鍥惧儚鍩烘湰淇℃伅
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstInfo                     [IN][OUT]       杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈哄浘鍍忓熀鏈俊鎭粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MV_CC_GetImageInfo
 *  @brief  Get basic information of image
 *  @param  handle                      [IN]            Device handle
 *  @param  pstInfo                     [IN][OUT]       Structure pointer of image basic information
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetImageInfo(IN void* handle, IN OUT MV_IMAGE_BASIC_INFO* pstInfo);

/************************************************************************
 *  @fn     MV_CC_SetSDKLogPath()
 *  @brief  璁剧疆SDK鏃ュ織璺緞锛堝鏋滄棩蹇楁湇鍔vLogServer宸插惎鐢紝鍒欒鎺ュ彛鏃犳晥锛岄粯璁ゆ棩蹇楁湇鍔′负寮€鍚姸鎬侊級
 *  @param  pSDKLogPath             [IN]           SDK鏃ュ織璺緞
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @fn     MV_CC_SetSDKLogPath()
 *  @brief  Set SDK log path
 *  @param  pSDKLogPath             [IN]           SDK log path
 *  @return Access, return true. Not access, return false
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetSDKLogPath(IN const char * pSDKLogPath);

/************************************************************************
 *  @fn     MV_CC_GetTlProxy
 *  @brief  鑾峰彇GenICam浠ｇ悊
 *  @param  handle                 [IN]           鍙ユ焺鍦板潃
 *  @return GenICam浠ｇ悊绫绘寚閽?锛屾甯歌繑鍥炲€奸潪NULL锛涘紓甯歌繑鍥濶ULL
 
 *  @fn     MV_CC_GetTlProxy
 *  @brief  Get GenICam proxy
 *  @param  handle                 [IN]           Handle address
 *  @return GenICam proxy pointer, normal, return non-NULL; exception, return NULL
 ************************************************************************/
MVSDKEXPORT_API void* __stdcall MV_CC_GetTlProxy(IN void* handle);

/***********************************************************************
 *  @fn         MV_XML_GetRootNode
 *  @brief      鑾峰彇鏍硅妭鐐?
 *  @param       handle                 [IN]          鍙ユ焺
 *  @param       pstNode                [OUT]         鏍硅妭鐐逛俊鎭粨鏋勪綋
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @fn         MV_XML_GetRootNode
 *  @brief      Get root node
 *  @param       handle                 [IN]          Handle
 *  @param       pstNode                [OUT]         Root node information structure
 *  @return Success, return MV_OK. Failure, return error code
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_XML_GetRootNode(IN void* handle, IN OUT MV_XML_NODE_FEATURE* pstNode);

/***********************************************************************
 *  @fn         MV_XML_GetChildren
 *  @brief      浠巟ml涓幏鍙栨寚瀹氳妭鐐圭殑鎵€鏈夊瓙鑺傜偣锛屾牴鑺傜偣涓篟oot
 *  @param       handle                 [IN]          鍙ユ焺
 *  @param       pstNode                [IN]          鏍硅妭鐐逛俊鎭粨鏋勪綋
 *  @param       pstNodesList           [OUT]         鑺傜偣鍒楄〃缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @fn         MV_XML_GetChildren
 *  @brief      Get all children node of specific node from xml, root node is Root
 *  @param       handle                 [IN]          Handle
 *  @param       pstNode                [IN]          Root node information structure
 *  @param       pstNodesList           [OUT]         Node information structure
 *  @return Success, return MV_OK. Failure, return error code
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_XML_GetChildren(IN void* handle, IN MV_XML_NODE_FEATURE* pstNode, IN OUT MV_XML_NODES_LIST* pstNodesList);

/***********************************************************************
 *  @fn         MV_XML_GetNodeFeature
 *  @brief      鑾峰緱褰撳墠鑺傜偣鐨勫睘鎬?
 *  @param       handle                 [IN]          鍙ユ焺
 *  @param       pstNode                [IN]          鏍硅妭鐐逛俊鎭粨鏋勪綋
 *  @param       pstFeature             [OUT]         褰撳墠鑺傜偣灞炴€х粨鏋勪綋锛?
                           pstFeature 鍏蜂綋缁撴瀯浣撳唴瀹瑰弬鑰?MV_XML_FEATURE_x
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @fn         MV_XML_GetNodeFeature
 *  @brief      Get current node feature
 *  @param       handle                 [IN]          Handle
 *  @param       pstNode                [IN]          Root node information structure
 *  @param       pstFeature             [OUT]         Current node feature structure
                           Details of pstFeature refer to MV_XML_FEATURE_x
 *  @return Success, return MV_OK. Failure, return error code
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_XML_GetNodeFeature(IN void* handle, IN MV_XML_NODE_FEATURE* pstNode, IN OUT void* pstFeature);

/***********************************************************************
 *  @fn         MV_XML_UpdateNodeFeature
 *  @brief      鏇存柊鑺傜偣
 *  @param       handle                 [IN]          鍙ユ焺
 *  @param       enType                 [IN]          鑺傜偣绫诲瀷
 *  @param       pstFeature             [OUT]         褰撳墠鑺傜偣灞炴€х粨鏋勪綋
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @fn         MV_XML_UpdateNodeFeature
 *  @brief      Update node
 *  @param       handle                 [IN]          Handle
 *  @param       enType                 [IN]          Node type
 *  @param       pstFeature             [OUT]         Current node feature structure
 *  @return Success, return MV_OK. Failure, return error code
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_XML_UpdateNodeFeature(IN void* handle, IN enum MV_XML_InterfaceType enType, IN void* pstFeature);

// 鏈夎妭鐐归渶瑕佹洿鏂版椂鐨勫洖璋冨嚱鏁?
// 褰撹皟鐢∕V_XML_UpdateNodeFeature鎺ュ彛鏇存柊鑺傜偣灞炴€ф椂锛屾敞鍐岀殑鍥炶皟鍑芥暟cbUpdate浼氬湪pstNodesList涓繑鍥炰笌涔嬬浉鍏宠仈鐨勮妭鐐?
/***********************************************************************
 *  @fn         MV_XML_RegisterUpdateCallBack
 *  @brief      娉ㄥ唽鏇存柊鍥炶皟
 *  @param       handle                 [IN]          鍙ユ焺
 *  @param       cbUpdate               [IN]          鍥炶皟鍑芥暟鎸囬拡
 *  @param       pUser                  [IN]          鐢ㄦ埛鑷畾涔夊彉閲?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @fn         MV_XML_RegisterUpdateCallBack
 *  @brief      Register update callback
 *  @param       handle                 [IN]          Handle
 *  @param       cbUpdate               [IN]          Callback function pointer
 *  @param       pUser                  [IN]          User defined variable
 *  @return Success, return MV_OK. Failure, return error code
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_XML_RegisterUpdateCallBack(IN void* handle, 
                                                           IN void(__stdcall* cbUpdate)(enum MV_XML_InterfaceType enType, void* pstFeature, MV_XML_NODES_LIST* pstNodesList, void* pUser),
                                                           IN void* pUser);

/************************************************************************/
/* 寮冪敤鐨勬帴鍙ｏ紙瀛樺湪鏇翠紭鍖栫殑鎺ュ彛鍙浛鎹級                     			*/
/* Abandoned interface                                                  */
/************************************************************************/
/***********************************************************************
 *  @fn         MV_CC_GetOneFrame
 *  @brief      鑾峰彇涓€甯у浘鍍忥紝姝ゅ嚱鏁颁负鏌ヨ寮忚幏鍙栵紝姣忔璋冪敤鏌ヨ鍐呴儴缂撳瓨鏈?
                鏃犳暟鎹紝鏈夋暟鎹垯鑼冨洿鏁版嵁锛屾棤鏁版嵁杩斿洖閿欒鐮?
                锛堣鎺ュ彛宸插純鐢紝寤鸿鏀圭敤 MV_CC_GetOneFrameTimeOut鎺ュ彛锛?
 *  @param       handle                 [IN]          鍙ユ焺
 *  @param       pData                  [OUT]         鍥惧儚鏁版嵁鎺ユ敹鎸囬拡
 *  @param       nDataSize              [IN]          鎺ユ敹缂撳瓨澶у皬
 *  @param       pFrameInfo             [OUT]         鍥惧儚淇℃伅缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @fn         MV_CC_GetOneFrame
 *  @brief      Get one frame data, this function is using query to get data, 
                query whether the internal cache has data, return data if there has, return error code if no data
                (This interface is abandoned, it is recommended to use the MV_CC_GetOneFrameTimeOut)
 *  @param       handle                 [IN]          Handle
 *  @param       pData                  [OUT]         Recevied image data pointer
 *  @param       nDataSize              [IN]          Recevied buffer size
 *  @param       pFrameInfo             [OUT]         Image information structure
 *  @return Success, return MV_OK. Failure, return error code
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetOneFrame(IN void* handle, IN OUT unsigned char * pData , IN unsigned int nDataSize, IN OUT MV_FRAME_OUT_INFO* pFrameInfo);

/***********************************************************************
 *  @fn         MV_CC_GetOneFrameEx
 *  @brief      鑾峰彇涓€甯runck鏁版嵁锛屾鍑芥暟涓烘煡璇㈠紡鑾峰彇锛屾瘡娆¤皟鐢ㄦ煡璇㈠唴閮?
                缂撳瓨鏈夋棤鏁版嵁锛屾湁鏁版嵁鍒欒寖鍥存暟鎹紝鏃犳暟鎹繑鍥為敊璇爜
                锛堣鎺ュ彛宸插純鐢紝寤鸿鏀圭敤 MV_CC_GetOneFrameTimeOut鎺ュ彛锛?
 *  @param       handle                 [IN]          鍙ユ焺
 *  @param       pData                  [OUT]         鍥惧儚鏁版嵁鎺ユ敹鎸囬拡
 *  @param       nDataSize              [IN]          鎺ユ敹缂撳瓨澶у皬
 *  @param       pFrameInfo             [OUT]         鍥惧儚淇℃伅缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @fn         MV_CC_GetOneFrameEx
 *  @brief      Get one frame of trunck data, this function is using query to get data, 
                query whether the internal cache has data, return data if there has, return error code if no data
                (This interface is abandoned, it is recommended to use the MV_CC_GetOneFrameTimeOut)
 *  @param       handle                 [IN]          Handle
 *  @param       pData                  [OUT]         Recevied image data pointer
 *  @param       nDataSize              [IN]          Recevied buffer size
 *  @param       pFrameInfo             [OUT]         Image information structure
 *  @return Success, return MV_OK. Failure, return error code
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetOneFrameEx(IN void* handle, IN OUT unsigned char * pData , IN unsigned int nDataSize, IN OUT MV_FRAME_OUT_INFO_EX* pFrameInfo);

/***********************************************************************
 *  @fn         MV_CC_RegisterImageCallBack
 *  @brief      娉ㄥ唽鍥惧儚鏁版嵁鍥炶皟锛堣鎺ュ彛宸插純鐢紝寤鸿鏀圭敤 MV_CC_RegisterImageCallBackEx鎺ュ彛锛?
 *  @param       handle                 [IN]          鍙ユ焺
 *  @param       cbOutput               [IN]          鍥炶皟鍑芥暟鎸囬拡
 *  @param       pUser                  [IN]          鐢ㄦ埛鑷畾涔夊彉閲?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @fn         MV_CC_RegisterImageCallBack
 *  @brief      Register image data callback (This interface is abandoned, it is recommended to use the MV_CC_RegisterImageCallBackEx)
 *  @param       handle                 [IN]          Handle
 *  @param       cbOutput               [IN]          Callback function pointer
 *  @param       pUser                  [IN]          User defined variable
 *  @return Success, return MV_OK. Failure, return error code
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_RegisterImageCallBack(void* handle, 
                                                         void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO* pFrameInfo, void* pUser),
                                                         void* pUser);

/************************************************************************
 *  @fn     MV_CC_SaveImage
 *  @brief  淇濆瓨鍥剧墖锛堣鎺ュ彛宸插純鐢紝寤鸿鏀圭敤 MV_CC_SaveImageEx2鎺ュ彛锛?
 *  @param  pSaveParam             [IN][OUT]          淇濆瓨鍥剧墖鍙傛暟缁撴瀯浣?
                       pData;              // [IN]     杈撳叆鏁版嵁缂撳瓨
                       nDataLen;           // [IN]     杈撳叆鏁版嵁澶у皬
                       enPixelType;        // [IN]     杈撳叆鏁版嵁鐨勫儚绱犳牸寮?
                       nWidth;             // [IN]     鍥惧儚瀹?
                       nHeight;            // [IN]     鍥惧儚楂?
                       pImageBuffer;       // [OUT]    杈撳嚭鍥剧墖缂撳瓨
                       nImageLen;          // [OUT]    杈撳嚭鍥剧墖澶у皬
                       nBufferSize;        // [IN]     鎻愪緵鐨勮緭鍑虹紦鍐插尯澶у皬
                       enImageType;        // [IN]     杈撳嚭鍥剧墖鏍煎紡
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @fn     MV_CC_SaveImage
 *  @brief  Save image (This interface is abandoned, it is recommended to use the MV_CC_SaveImageEx)
 *  @param  pSaveParam             [IN][OUT]          Save image parameters structure
                       pData;              // [IN]     Input data buffer
                       nDataLen;           // [IN]     Input data size
                       enPixelType;        // [IN]     Input data pixel format
                       nWidth;             // [IN]     Width
                       nHeight;            // [IN]     Height
                       pImageBuffer;       // [OUT]    Output image buffer
                       nImageLen;          // [OUT]    Output image size
                       nBufferSize;        // [IN]     Provided output buffer size
                       enImageType;        // [IN]     Output image type
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SaveImage(IN OUT MV_SAVE_IMAGE_PARAM* pSaveParam);

/************************************************************************
 *  @fn     MV_CC_SaveImageEx
 *  @brief  淇濆瓨鍥剧墖锛屾敮鎸丅mp鍜孞peg.缂栫爜璐ㄩ噺鍦?0-99涔嬪墠 锛堣鎺ュ彛宸插純鐢紝寤鸿鏀圭敤 MV_CC_SaveImageEx2鎺ュ彛锛?
 *  @param  pSaveParam             [IN][OUT]          淇濆瓨鍥剧墖鍙傛暟缁撴瀯浣?
                       pData;              // [IN]     杈撳叆鏁版嵁缂撳瓨
                       nDataLen;           // [IN]     杈撳叆鏁版嵁澶у皬
                       enPixelType;        // [IN]     杈撳叆鏁版嵁鐨勫儚绱犳牸寮?
                       nWidth;             // [IN]     鍥惧儚瀹?
                       nHeight;            // [IN]     鍥惧儚楂?
                       pImageBuffer;       // [OUT]    杈撳嚭鍥剧墖缂撳瓨
                       nImageLen;          // [OUT]    杈撳嚭鍥剧墖澶у皬
                       nBufferSize;        // [IN]     鎻愪緵鐨勮緭鍑虹紦鍐插尯澶у皬
                       enImageType;        // [IN]     杈撳嚭鍥剧墖鏍煎紡
                       nJpgQuality;        // [IN]     缂栫爜璐ㄩ噺, (50-99]
                       nReserved[4];
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @fn     MV_CC_SaveImageEx
 *  @brief  Save image, support Bmp and Jpeg. Encoding quality, (50-99]
 *  @param  pSaveParam             [IN][OUT]           Save image parameters structure
                       pData;              // [IN]     Input data buffer
                       nDataLen;           // [IN]     Input data size
                       enPixelType;        // [IN]     Pixel format of input data
                       nWidth;             // [IN]     Image width
                       nHeight;            // [IN]     Image height
                       pImageBuffer;       // [OUT]    Output image buffer
                       nImageLen;          // [OUT]    Output image size
                       nBufferSize;        // [IN]     Output buffer size provided
                       enImageType;        // [IN]     Output image format
                       nJpgQuality;        // [IN]     Encoding quality, (50-99]
                       nReserved[4];
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SaveImageEx(IN OUT MV_SAVE_IMAGE_PARAM_EX* pSaveParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  Bayer鍣０浼拌锛堣鎺ュ彛宸插純鐢紝寤鸿鏀圭敤 MV_CC_NoiseEstimate鎺ュ彛锛?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstNoiseEstimateParam       [IN][OUT]       Bayer鍣０浼拌鍙傛暟缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｅ彧鏀寔Bayer8/Bayer10/Bayer12鏍煎紡,鍏跺畠Bayer鏍煎紡闇€鍏堣浆鎴怋ayer8/Bayer10/Bayer12鏍煎紡銆俓n
             璇ユ帴鍙ｅ彧鏈夊湪鎵撳紑鎴戝徃鐗瑰畾褰╄壊鐩告満鍚庢墠鍙互姝ｅ父浣跨敤锛屽綋鐩告満琚柇寮€鎴栬€呮帀绾垮悗锛岀户缁娇鐢ㄨ鎺ュ彛浼氭姤閿欍€?
 
 *  @~english
 *  @brief  Noise estimate of Bayer format
 *  @param  handle                      [IN]            Device handle
 *  @param  pstNoiseEstimateParam       [IN][OUT]       Noise estimate parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This API only support Bayer8/Bayer10/Bayer12 format, other Bayer format must Convert to Bayer8/Bayer10/Bayer12 format.\n
             This API is only available when the camera is turned on, and when the camera is disconnected or disconnected, continuing to use This API will return an error.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_BayerNoiseEstimate(IN void* handle, IN OUT MV_CC_BAYER_NOISE_ESTIMATE_PARAM* pstNoiseEstimateParam);

/********************************************************************//**
 *  @~chinese
 *  @brief  Bayer绌哄煙闄嶅櫔锛堣鎺ュ彛宸插純鐢紝寤鸿鏀圭敤 MV_CC_SpatialDenoise鎺ュ彛锛?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pstSpatialDenoiseParam      [IN][OUT]       Bayer绌哄煙闄嶅櫔鍙傛暟缁撴瀯浣?
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 *  @remarks 璇ユ帴鍙ｅ彧鏀寔Bayer8/Bayer10/Bayer12鏍煎紡,鍏跺畠Bayer鏍煎紡闇€鍏堣浆鎴怋ayer8/Bayer10/Bayer12鏍煎紡銆俓n
             璇ユ帴鍙ｅ彧鏈夊湪鎵撳紑鎴戝徃鐗瑰畾褰╄壊鐩告満鍚庢墠鍙互姝ｅ父浣跨敤锛屽綋鐩告満琚柇寮€鎴栬€呮帀绾垮悗锛岀户缁娇鐢ㄨ鎺ュ彛浼氭姤閿欍€?
 
 *  @~english
 *  @brief  Spatial Denoise of Bayer format
 *  @param  handle                      [IN]            Device handle
 *  @param  pstSpatialDenoiseParam      [IN][OUT]       Spatial Denoise parameter structure
 *  @return Success, return MV_OK. Failure, return error code
 *  @remarks This API only support Bayer8/Bayer10/Bayer12 format, other Bayer format must Convert to Bayer8/Bayer10/Bayer12 format.\n
             This API is only available when the camera is turned on, and when the camera is disconnected or disconnected, continuing to use This API will return an error.
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_BayerSpatialDenoise(IN void* handle, IN OUT MV_CC_BAYER_SPATIAL_DENOISE_PARAM* pstSpatialDenoiseParam);

/************************************************************************
 *  @fn     MV_GIGE_ForceIp
 *  @brief  寮哄埗IP锛堣鎺ュ彛宸插純鐢紝寤鸿鏀圭敤 MV_GIGE_ForceIpEx鎺ュ彛锛?
 *  @param  handle锛氳澶囧彞鏌?
 *  @param  nIP               [IN]      璁剧疆鐨処P
 *  @return 瑙佽繑鍥為敊璇爜
 
 *  @fn     MV_GIGE_ForceIp
 *  @brief  Force IP (This interface is abandoned, it is recommended to use the MV_GIGE_ForceIpEx)
 *  @param  handle Handle
 *  @param  nIP               [IN]      IP to set
 *  @return Refer to error code
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_ForceIp(IN void* handle, unsigned int nIP);

/************************************************************************
 *  @fn     MV_CC_RegisterEventCallBack
 *  @brief  娉ㄥ唽浜嬩欢鍥炶皟锛堣鎺ュ彛宸插純鐢紝寤鸿鏀圭敤 MV_CC_RegisterEventCallBackEx鎺ュ彛锛?
 *  @param  handle锛氳澶囧彞鏌?
 *  @param  cbEvent           [IN]      浜嬩欢鍥炶皟鍑芥暟鎸囬拡
 *  @param  pUser             [IN]      鐢ㄦ埛鑷畾涔夊彉閲?
 *  @return 瑙佽繑鍥為敊璇爜
 
 *  @fn     MV_CC_RegisterEventCallBack
 *  @brief  Register event callback (this interface has been deprecated and is recommended to be converted to the MV_CC_RegisterEventCallBackEx interface)
 *  @param  handle锛氳澶囧彞鏌?
 *  @param  cbEvent           [IN]      event callback pointer
 *  @param  pUser             [IN]      User defined value
 *  @return 瑙佽繑鍥為敊璇爜
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_RegisterEventCallBack(void* handle, void(__stdcall* cbEvent)(unsigned int nExternalEventId, void* pUser), void* pUser);

/***********************************************************************
 *  @fn         MV_CC_Display
 *  @brief      鏄剧ず鍥惧儚锛屾敞鍐屾樉绀虹獥鍙ｏ紝鍐呴儴鑷姩鏄剧ず锛堜笌MV_CC_GetImageBuffer涓嶈兘鍚屾椂浣跨敤锛屽缓璁敼鐢∕V_CC_DisplayOneFrame鎺ュ彛锛?
 *  @param       handle                 [IN]          鍙ユ焺
 *  @param       hWnd                   [IN]          鏄剧ず绐楀彛鍙ユ焺
 *  @return 鎴愬姛锛岃繑鍥濵V_OK锛涢敊璇紝杩斿洖閿欒鐮?
 
 *  @fn         MV_CC_Display
 *  @brief      Display one frame image, register display window, automatic display internally
 *  @param      handle                 [IN]          Handle
 *  @param      hWnd                   [IN]          Display Window Handle
 *  @return     Success, return MV_OK. Failure, return error code
 ***********************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_Display(IN void* handle, void* hWnd);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetIntValue(IN void* handle,
                                                           IN const char* strKey,
                                                           OUT MVCC_INTVALUE *pIntValue);
 *  @brief  鑾峰彇Integer灞炴€у€硷紙寤鸿鏀圭敤MV_CC_GetIntValueEx鎺ュ彛锛?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  char* strKey                [IN]        灞炴€ч敭鍊硷紝濡傝幏鍙栧搴︿俊鎭垯涓?Width"
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈哄睘鎬х粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetIntValue(IN void* handle,
                                                           IN const char* strKey,
                                                           OUT MVCC_INTVALUE *pIntValue);
 *  @brief  Get Integer value
 *  @param  void* handle                [IN]        Handle
 *  @param  char* strKey                [IN]        Key value, for example, using "Width" to get width
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Structure pointer of camera features
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetIntValue(IN void* handle,IN const char* strKey,OUT MVCC_INTVALUE *pIntValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetIntValue(IN void* handle,
                                                           IN const char* strKey,
                                                           IN unsigned int nValue);
 *  @brief  璁剧疆Integer鍨嬪睘鎬у€硷紙寤鸿鏀圭敤MV_CC_SetIntValueEx鎺ュ彛锛?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  char* strKey                [IN]        灞炴€ч敭鍊硷紝濡傝幏鍙栧搴︿俊鎭垯涓?Width"
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勭浉鏈虹殑灞炴€у€?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetIntValue(IN void* handle,
                                                           IN const char* strKey,
                                                           IN unsigned int nValue);
 *  @brief  Set Integer value
 *  @param  void* handle                [IN]        Handle
 *  @param  char* strKey                [IN]        Key value, for example, using "Width" to set width
 *          const unsigned int nValue   [IN]        Feature value to set
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetIntValue(IN void* handle,IN const char* strKey,IN unsigned int nValue);


/************************************************************************/
/* 鐩告満鍙傛暟鑾峰彇鍜岃缃紝姝ゆā鍧楃殑鎵€鏈夋帴鍙ｅ凡搴熷純锛屽缓璁娇鐢ㄤ竾鑳芥帴鍙ｄ唬鏇?  */
/* Get and set camara parameters, all interfaces of this module will be replaced by general interface*/
/************************************************************************/
/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetWidth(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇鍥惧儚瀹藉害
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈哄搴︾殑淇℃伅缁撴瀯浣撴寚閽?
 *          杩斿洖鐨刾stValue缁撴瀯浣撶殑鎰忎箟
 *                  unsigned int    nCurValue;      // 浠ｈ〃鐩告満褰撳墠鐨勫搴﹀€?
 *                  unsigned int    nMax;           // 琛ㄧず鐩告満鍏佽鐨勬渶澶у彲璁剧疆鐨勫搴﹀€?
 *                  unsigned int    nMin;           // 琛ㄧず鐩告満鍏佽鐨勬渶灏忓彲璁剧疆鐨勫搴﹀€?
 *                  unsigned int    nInc;           // 琛ㄧず鐩告満璁剧疆鐨勫搴﹀閲忓繀椤绘槸nInc鐨勫€嶆暟锛屽惁鍒欐棤鏁?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞惰幏寰楃浉搴斿弬鏁颁俊鎭殑缁撴瀯浣? 澶辫触, 杩斿洖閿欒鐮?
 *  
 *          鍏朵粬鏁村瀷缁撴瀯浣撳弬鏁扮殑鎺ュ彛鍙弬鐓ф鎺ュ彛
 
 * @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetWidth(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get image width
 *  @param  void* handle                [IN]        Camera Handle
 *          MVCC_INTVALUE* pstValue     [IN][OUT]   Returns the information structure pointer about the camera's width for the caller
 *          The meaning of returns pstValue structure
 *                  unsigned int    nCurValue;      // Represents the current width value of the camera
 *                  unsigned int    nMax;           // Indicates the maximum settable width value allowed by the camera
 *                  unsigned int    nMin;           // Indicates the minimum settable width value allowed by the camera
 *                  unsigned int    nInc;           // Indicates that the width increment set by the camera must be a multiple of nInc, otherwise it is invalid
 *  @return Success, return MV_OK, and get the structure of the corresponding parameters. Failure, return error code
 *  
 *          Other Integer structure parameters interface can refer to this interface
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetWidth(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
*  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetWidth(IN void* handle, IN const unsigned int nValue);
*  @brief  璁剧疆鍥惧儚瀹藉害
*  @param  void* handle                [IN]        鐩告満鍙ユ焺
*          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勭浉鏈哄搴︾殑鍊?娉ㄦ剰姝ゅ搴﹀€煎繀椤绘槸MV_CC_GetWidth鎺ュ彛杩斿洖鐨刾stValue涓殑nInc鐨勫€嶆暟鎵嶈兘璁剧疆鎴愬姛
*  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満瀹藉害灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?

* @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetWidth(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set image width
 *  @param  void* handle                [IN]        Camera Handle
 *          const unsigned int nValue   [IN]        To set the value of the camera width, note that the width value must be a multiple of nInc in the pstValue returned by the MV_CC_GetWidth interface
 *  @return Success, return MV_OK, and the camera width will change to the corresponding value. Failure, return error code
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetWidth(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetHeight(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇鍥惧儚楂樺害
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈洪珮搴︾殑淇℃伅缁撴瀯浣撴寚閽?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞跺皢楂樺害淇℃伅杩斿洖鍒扮粨鏋勪綋涓紝澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetHeight(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get image height
 *  @param  void* handle                [IN]        Camera handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Return pointer of information structure related to camera height to user
 *  @return Success, return MV_OK, and return height information to the structure. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetHeight(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetHeight(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆鍥惧儚楂樺害
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勭浉鏈哄搴︾殑鍊?娉ㄦ剰姝ゅ搴﹀€煎繀椤绘槸MV_CC_GetWidth鎺ュ彛杩斿洖鐨刾stValue涓殑nInc鐨勫€嶆暟鎵嶈兘璁剧疆鎴愬姛
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満楂樺害灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetHeight(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set image height
 *  @param  void* handle                [IN]        Camera Handle
 *          const unsigned int nValue   [IN]        Camera height value to set, note that this value must be times of nInc of pstValue returned by MV_CC_GetWidth
 *  @return Success, return MV_OK, and the camera height will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetHeight(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetAOIoffsetX(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇鍥惧儚X鍋忕Щ
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈篨鍋忕Щ鐨勪俊鎭粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetAOIoffsetX(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get image X offset
 *  @param  void* handle                [IN]        Camera Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Return pointer of information structure related to camera X offset to user
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetAOIoffsetX(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetAOIoffsetX(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆鍥惧儚AOI鍋忕Щ
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勭浉鏈篈OI鐨勫€?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満AOI鍋忕Щ灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetAOIoffsetX(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set image X offset
 *  @param  void* handle                [IN]        Camera Handle
 *          const unsigned int nValue   [IN]        Camera X offset value to set
 *  @return Success, return MV_OK, and the camera X offset will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetAOIoffsetX(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetAOIoffsetY(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇鍥惧儚Y鍋忕Щ
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈篩鍋忕Щ鐨勪俊鎭粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetAOIoffsetY(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get image Y offset
 *  @param  void* handle                [IN]        Camera Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Return pointer of information structure related to camera Y offset to user
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetAOIoffsetY(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetAOIoffsetX(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆鍥惧儚AOI鍋忕Щ
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勭浉鏈篈OI鐨勫€?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満AOI鍋忕Щ灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetAOIoffsetY(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set image Y offset
 *  @param  void* handle                [IN]        Camera Handle
 *          const unsigned int nValue   [IN]        Camera Y offset value to set
 *  @return Success, return MV_OK, and the camera Y offset will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetAOIoffsetY(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetAutoExposureTimeLower(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇鏇濆厜涓嬮檺
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈烘洕鍏夊€间笅闄愮粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetAutoExposureTimeLower(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get exposure lower limit
 *  @param  void* handle                [IN]        Camera Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Return pointer of information structure related to camera exposure lower to user
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetAutoExposureTimeLower(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetAutoExposureTimeLower(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆鏇濆厜鍊间笅闄?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勬洕鍏夊€间笅闄?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満鏇濆厜涓嬮檺灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetAutoExposureTimeLower(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set exposure lower limit
 *  @param  void* handle                [IN]        Camera Handle
 *          const unsigned int nValue   [IN]        Exposure lower to set
 *  @return Success, return MV_OK, and the camera exposure time lower limit value will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetAutoExposureTimeLower(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetAutoExposureTimeUpper(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇鏇濆厜涓婇檺
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈烘洕鍏夊€间笂闄愮粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetAutoExposureTimeUpper(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get exposure upper limit
 *  @param  void* handle                [IN]        Camera Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Return pointer of information structure related to camera exposure upper to user
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetAutoExposureTimeUpper(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetAutoExposureTimeUpper(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆鏇濆厜鍊间笂闄?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勬洕鍏夊€间笂闄?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満鏇濆厜涓婇檺灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetAutoExposureTimeUpper(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set exposure upper limit
 *  @param  void* handle                [IN]        Camera Handle
 *          const unsigned int nValue   [IN]        Exposure upper to set
 *  @return Success, return MV_OK, and the camera exposure time upper limit value will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetAutoExposureTimeUpper(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetBrightness(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇浜害鍊?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈轰寒搴︾粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetBrightness(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get brightness
 *  @param  void* handle                [IN]        Camera Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Return pointer of information structure related to camera brightness to user
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetBrightness(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetBrightness(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆浜害鍊?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勪寒搴﹀€?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満浜害灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetBrightness(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set brightness
 *  @param  void* handle                [IN]        Camera Handle
 *          const unsigned int nValue   [IN]        Brightness upper to set
 *  @return Success, return MV_OK, and the camera brightness value will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetBrightness(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetFrameRate(IN void* handle, IN OUT MVCC_FLOATVALUE* pstValue);
 *  @brief  鑾峰彇甯х巼
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          MVCC_FLOATVALUE* pstValue   [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈哄抚鐜囩殑淇℃伅缁撴瀯浣撴寚閽?
 *          杩斿洖鐨刾stValue缁撴瀯浣撶殑鎰忎箟
 *                                      float           fCurValue;      // 琛ㄧず鐩告満褰撳墠鐨勫抚鐜?
 *                                      float           fMax;           // 琛ㄧず鐩告満鍏佽璁剧疆鐨勬渶澶у抚鐜?
 *                                      float           fMin;           // 琛ㄧず鐩告満鍏佽璁剧疆鐨勬渶灏忓抚鐜?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞惰幏寰楃浉搴斿弬鏁颁俊鎭殑缁撴瀯浣? 澶辫触, 杩斿洖閿欒鐮?
 *  
 *          鍏朵粬娴偣鍨嬬粨鏋勪綋鍙傛暟鐨勬帴鍙ｅ彲鍙傜収姝ゆ帴鍙?
 
 * @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetFrameRate(IN void* handle, IN OUT MVCC_FLOATVALUE* pstValue);
 *  @brief  Get Frame Rate
 *  @param  void* handle                [IN]        Camera Handle
 *          MVCC_FLOATVALUE* pstValue   [IN][OUT]   Return pointer of information structure related to camera frame rate to user
 *          The meaning of returns pstValue structure
 *                                      float           fCurValue;      // Indicates the current frame rate of the camera
 *                                      float           fMax;           // Indicates the maximum frame rate allowed by the camera
 *                                      float           fMin;           // Indicates the minimum frame rate allowed by the camera
 *  @return Success, return MV_OK, and get the structure of the corresponding parameters. Failure, return error code
 *  
 *          Other interface of Float structure parameters can refer to this interface
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetFrameRate(IN void* handle, IN OUT MVCC_FLOATVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetFrameRate(IN void* handle, IN const float fValue);
 *  @brief  璁剧疆甯х巼
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const float fValue          [IN]        鎯宠璁剧疆鐨勭浉鏈哄抚鐜?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満甯х巼灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 * @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetFrameRate(IN void* handle, IN const float fValue);
 *  @brief  Set frame rate
 *  @param  void* handle                [IN]        Camera Handle
 *          const float fValue          [IN]        Camera frame rate to set 
 *  @return Success, return MV_OK, and camera frame rate will be changed to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetFrameRate(IN void* handle, IN const float fValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetGain(IN void* handle, IN OUT MVCC_FLOATVALUE* pstValue);
 *  @brief  鑾峰彇澧炵泭
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          MVCC_FLOATVALUE* pstValue   [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈哄鐩婄殑淇℃伅缁撴瀯浣撴寚閽?
 *          杩斿洖鐨刾stValue缁撴瀯浣撶殑鎰忎箟
 *                                      float           fCurValue;      // 琛ㄧず鐩告満褰撳墠鐨勫抚鐜?
 *                                      float           fMax;           // 琛ㄧず鐩告満鍏佽璁剧疆鐨勬渶澶у抚鐜?
 *                                      float           fMin;           // 琛ㄧず鐩告満鍏佽璁剧疆鐨勬渶灏忓抚鐜?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞惰幏寰楃浉搴斿弬鏁颁俊鎭殑缁撴瀯浣? 澶辫触, 杩斿洖閿欒鐮?
 *  
 *          鍏朵粬娴偣鍨嬬粨鏋勪綋鍙傛暟鐨勬帴鍙ｅ彲鍙傜収姝ゆ帴鍙?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetGain(IN void* handle, IN OUT MVCC_FLOATVALUE* pstValue);
 *  @brief  Get Gain
 *  @param  void* handle                [IN]        Camera Handle
 *          MVCC_FLOATVALUE* pstValue   [IN][OUT]   Return pointer of information structure related to gain to user
 *  @return Success, return MV_OK, and get the structure of the corresponding parameters. Failure, return error code
 *                                      float           fCurValue;      // Camera current gain
 *                                      float           fMax;           // The maximum gain camera allowed
 *                                      float           fMin;           // The minimum gain camera allowed
 *  @return Success, return MV_OK, and get the structure of the corresponding parameters. Failure, return error code
 *  
 *          Other interface of Float structure parameters can refer to this interface
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetGain(IN void* handle, IN OUT MVCC_FLOATVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetGain(IN void* handle, IN const float fValue);
 *  @brief  璁剧疆甯х巼
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const float fValue          [IN]        鎯宠璁剧疆鐨勭浉鏈哄抚鐜?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満甯х巼灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetGain(IN void* handle, IN const float fValue);
 *  @brief  Set Gain
 *  @param  void* handle                [IN]        Camera Handle
 *          const float fValue          [IN]        Gain value to set
 *  @return Success, return MV_OK, and the camera gain value will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetGain(IN void* handle, IN const float fValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetExposureTime(IN void* handle, IN OUT MVCC_FLOATVALUE* pstValue);
 *  @brief  鑾峰彇鏇濆厜鏃堕棿
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          MVCC_FLOATVALUE* pstValue   [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈烘洕鍏夋椂闂寸殑淇℃伅缁撴瀯浣撴寚閽?
 *          杩斿洖鐨刾stValue缁撴瀯浣撶殑鎰忎箟
 *                                      float           fCurValue;      // 琛ㄧず鐩告満褰撳墠鐨勫抚鐜?
 *                                      float           fMax;           // 琛ㄧず鐩告満鍏佽璁剧疆鐨勬渶澶у抚鐜?
 *                                      float           fMin;           // 琛ㄧず鐩告満鍏佽璁剧疆鐨勬渶灏忓抚鐜?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞惰幏寰楃浉搴斿弬鏁颁俊鎭殑缁撴瀯浣? 澶辫触, 杩斿洖閿欒鐮?
 *  
 *          鍏朵粬娴偣鍨嬬粨鏋勪綋鍙傛暟鐨勬帴鍙ｅ彲鍙傜収姝ゆ帴鍙?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetExposureTime(IN void* handle, IN OUT MVCC_FLOATVALUE* pstValue);
 *  @brief  Get exposure time
 *  @param  void* handle                [IN]        Camera Handle
 *          MVCC_FLOATVALUE* pstValue   [IN][OUT]   Return pointer of information structure related to exposure time to user
 *  @return Success, return MV_OK, and get the structure of the corresponding parameters. Failure, return error code
 *                                      float           fCurValue;      // Camera current exposure time
 *                                      float           fMax;           // The maximum exposure time camera allowed
 *                                      float           fMin;           // The minimum exposure time camera allowed
 *  @return Success, return MV_OK, and get the structure of the corresponding parameters. Failure, return error code
 *  
 *          Other interface of Float structure parameters can refer to this interface
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetExposureTime(IN void* handle, IN OUT MVCC_FLOATVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetExposureTime(IN void* handle, IN const float fValue);
 *  @brief  璁剧疆鏇濆厜鏃堕棿
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const float fValue          [IN]        鎯宠璁剧疆鐨勭浉鏈哄抚鐜?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満甯х巼灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetExposureTime(IN void* handle, IN const float fValue);
 *  @brief  Set exposure time
 *  @param  void* handle                [IN]        Camera Handle
 *          const float fValue          [IN]        Exposure time to set
 *  @return Success, return MV_OK, and the camera exposure time value will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetExposureTime(IN void* handle, IN const float fValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetPixelFormat(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);
 *  @brief  鑾峰彇鍍忕礌鏍煎紡
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          MVCC_ENUMVALUE* pstValue    [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呯殑鏈夊叧鍍忕礌鏍煎紡鐨勪俊鎭粨鏋勪綋鎸囬拡
 *          杩斿洖鐨刾stValue缁撴瀯浣撶殑鎰忎箟
 *          unsigned int    nCurValue;                              //  鐩告満褰撳墠鐨勫儚绱犳牸寮忥紝鏄灇涓剧被鍨?姣斿璇碢ixelType_Gvsp_Mono8, 杩欓噷鑾峰緱鐨勬槸鍏舵暣鍨嬪€?鍏蜂綋鏁板€煎弬鐓ixelType.h鐨凪vGvspPixelType鏋氫妇绫诲瀷
 *          unsigned int    nSupportedNum;                          //  鐩告満鏀寔鐨勫儚绱犳牸寮忕殑涓暟
 *          unsigned int    nSupportValue[MV_MAX_XML_SYMBOLIC_NUM]; //  鐩告満鎵€鏈夋敮鎸佺殑鍍忕礌鏍煎紡瀵瑰簲鐨勬暣鍨嬪€煎垪琛紝鍚庨潰瑕佽缃儚绱犳牸寮忔椂锛屽弬鏁板繀椤绘槸杩欎釜鏁扮粍涓殑涓€绉嶏紝鍚﹀垯鏃犳晥
 *  @return 鎴愬姛,杩斿洖MV_OK,骞惰幏寰楃浉搴斿弬鏁颁俊鎭殑缁撴瀯浣? 澶辫触, 杩斿洖閿欒鐮?
 *  
            鍏朵粬鏋氫妇绫诲瀷鍙傛暟鎺ュ彛鍙弬鐓ф鎺ュ彛锛屾湁鍏崇浉搴斿弬鏁扮殑鏋氫妇绫诲瀷瀵瑰簲鐨勬暣鍨嬪€艰鏌ユ壘PixelType.h 鍜?CameraParams.h涓浉搴旂殑瀹氫箟
 
 * @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetPixelFormat(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);
 *  @brief  Get Pixel Format
 *  @param  void* handle                [IN]        Camera Handle
 *          MVCC_ENUMVALUE* pstValue    [IN][OUT]   Returns the information structure pointer about pixel format for the caller
 *          The meaning of returns pstValue structure
 *          unsigned int    nCurValue;                              //  The current pixel format of the camera, is the enumeration type, such as PixelType_Gvsp_Mono8, here is the integer value, the specific value please refer to MvGvspPixelType enumeration type in PixelType.h
 *          unsigned int    nSupportedNum;                          //  Number of pixel formats supported by the camera
 *          unsigned int    nSupportValue[MV_MAX_XML_SYMBOLIC_NUM]; //  The integer values list correspond to all supported pixel formats of the camera, followed by when set the pixel format, the parameter must be one of this list, otherwise invalid
 *  @return Success, return MV_OK, and get the structure of the corresponding parameters. Failure, return error code
 *  
            Other interface of Enumeration structure parameters can refer to this interface, look for the corresponding definition in PixelType.h and CameraParams.h for the integer values of the enum type parameter
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetPixelFormat(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetPixelFormat(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆鍍忕礌鏍煎紡
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        瑕佽缃殑鍍忕礌鏍煎紡瀵瑰簲鐨勬暣鍨嬪€硷紝璋冪敤姝ゆ帴鍙ｆ椂鍙互鐩存帴濉啓鏋氫妇鍊硷紝濡侻V_CC_SetPixelFormat(m_handle, PixelType_Gvsp_RGB8_Packed);
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満鍍忕礌鏍煎紡灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 *  
 *          瑕佽缃殑鏋氫妇绫诲瀷蹇呴』鏄疓et鎺ュ彛杩斿洖鐨刵SupportValue[MV_MAX_XML_SYMBOLIC_NUM]涓殑涓€绉嶏紝鍚﹀垯浼氬け璐?
 
 * @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetPixelFormat(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set Pixel Format
 *  @param  void* handle                [IN]        Camera Handle
 *          const unsigned int nValue   [IN]        The corresponding integer value for pixel format to be set, when calling this interface can be directly filled in enumeration values, such as MV_CC_SetPixelFormat(m_handle, PixelType_Gvsp_RGB8_Packed);
 *  @return Success, return MV_OK, and the camera pixel format will change to the corresponding value. Failure, return error code
 *  
 *          Other interface of Enumeration structure parameters can refer to this interface, the enumeration type to be set must be one of the nSupportValue [MV_MAX_XML_SYMBOLIC_NUM] returned by the Get interface, otherwise it will fail
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetPixelFormat(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetAcquisitionMode(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);
 *  @brief  鑾峰彇閲囬泦妯″紡
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          MVCC_ENUMVALUE* pstValue    [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呯殑鏈夊叧閲囬泦妯″紡鐨勪俊鎭粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,骞惰幏寰楃浉搴斿弬鏁颁俊鎭殑缁撴瀯浣? 澶辫触, 杩斿洖閿欒鐮?
 *  
            鍙弬鐓ф帴鍙V_CC_GetPixelFormat锛屽弬鑰?CameraParam.h 涓殑 MV_CAM_ACQUISITION_MODE 瀹氫箟
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetAcquisitionMode(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);
 *  @brief  Get acquisition mode
 *  @param  void* handle                [IN]        Handle
 *          MVCC_ENUMVALUE* pstValue    [IN][OUT]   Structure pointer of acquisition mode
 *  @return Success, return MV_OK, and get the structure of the corresponding parameters. Failure, return error code
 *  
            Refer to MV_CC_GetPixelFormat and definition of MV_CAM_ACQUISITION_MODE in CameraParam.h
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetAcquisitionMode(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetAcquisitionMode(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆鍍忕礌鏍煎紡
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        瑕佽缃殑閲囬泦妯″紡瀵瑰簲鐨勬暣鍨嬪€?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満閲囬泦妯″紡灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetAcquisitionMode(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set acquisition mode
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        Integer value to set corresponding to acquisition mode
 *  @return Success, return MV_OK, and the camera acquisition mode will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetAcquisitionMode(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetGainMode(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);
 *  @brief  鑾峰彇澧炵泭妯″紡
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          MVCC_ENUMVALUE* pstValue    [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呯殑鏈夊叧澧炵泭妯″紡鐨勪俊鎭粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,骞惰幏寰楃浉搴斿弬鏁颁俊鎭殑缁撴瀯浣? 澶辫触, 杩斿洖閿欒鐮?
 *  
            鍙弬鐓ф帴鍙V_CC_GetPixelFormat锛屽弬鑰?CameraParam.h 涓殑 MV_CAM_GAIN_MODE 瀹氫箟
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetGainMode(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);
 *  @brief  Get gain mode
 *  @param  void* handle                [IN]        Handle
 *          MVCC_ENUMVALUE* pstValue    [IN][OUT]    Structure pointer of gain mode
 *  @return Success, return MV_OK, and get the structure of the corresponding parameters. Failure, return error code
 *  
            Refer to MV_CC_GetPixelFormat and definition of MV_CAM_GAIN_MODE in CameraParam.h
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetGainMode(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetGainMode(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆澧炵泭妯″紡
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        瑕佽缃殑澧炵泭妯″紡瀵瑰簲鐨勬暣鍨嬪€?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満澧炵泭妯″紡灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetGainMode(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set gain mode
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        Integer value to set corresponding to gain mode
 *  @return Success, return MV_OK, and the camera gain mode will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetGainMode(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetExposureAutoMode(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);
 *  @brief  鑾峰彇鑷姩鏇濆厜妯″紡
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          MVCC_ENUMVALUE* pstValue    [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呯殑鏈夊叧鑷姩鏇濆厜妯″紡鐨勪俊鎭粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,骞惰幏寰楃浉搴斿弬鏁颁俊鎭殑缁撴瀯浣? 澶辫触, 杩斿洖閿欒鐮?
 *  
            鍙弬鐓ф帴鍙V_CC_GetPixelFormat锛屽弬鑰?CameraParam.h 涓殑 MV_CAM_EXPOSURE_AUTO_MODE 瀹氫箟
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetExposureAutoMode(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);
 *  @brief  Get auto exposure mode
 *  @param  void* handle                [IN]        Handle
 *          MVCC_ENUMVALUE* pstValue    [IN][OUT]   Structure pointer of auto exposure mode
 *  @return Success, return MV_OK, and get the structure of the corresponding parameters. Failure, return error code
 *  
            Refer to MV_CC_GetPixelFormat and definition of MV_CAM_EXPOSURE_AUTO_MODE in CameraParam.h
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetExposureAutoMode(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetExposureAutoMode(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆鑷姩鏇濆厜妯″紡
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        瑕佽缃殑鑷姩鏇濆厜妯″紡瀵瑰簲鐨勬暣鍨嬪€?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満鑷姩鏇濆厜妯″紡灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetExposureAutoMode(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set auto exposure mode
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        Integer value to set corresponding to auto exposure mode
 *  @return Success, return MV_OK, and the camera auto exposure mode will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetExposureAutoMode(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetTriggerMode(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);
 *  @brief  鑾峰彇瑙﹀彂妯″紡
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          MVCC_ENUMVALUE* pstValue    [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呯殑鏈夊叧瑙﹀彂妯″紡鐨勪俊鎭粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,骞惰幏寰楃浉搴斿弬鏁颁俊鎭殑缁撴瀯浣? 澶辫触, 杩斿洖閿欒鐮?
 *  
            鍙弬鐓ф帴鍙V_CC_GetPixelFormat锛屽弬鑰?CameraParam.h 涓殑 MV_CAM_TRIGGER_MODE 瀹氫箟
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetTriggerMode(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);
 *  @brief  Get trigger mode
 *  @param  void* handle                [IN]        Handle
 *          MVCC_ENUMVALUE* pstValue    [IN][OUT]   Structure pointer of trigger mode
 *  @return Success, return MV_OK, and get the structure of the corresponding parameters. Failure, return error code
 *  
            Refer to MV_CC_GetPixelFormat and definition of MV_CAM_TRIGGER_MODE in CameraParam.h
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetTriggerMode(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetTriggerMode(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆瑙﹀彂妯″紡
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        瑕佽缃殑瑙﹀彂妯″紡瀵瑰簲鐨勬暣鍨嬪€?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満瑙﹀彂妯″紡灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetTriggerMode(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set trigger mode
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        Integer value to set corresponding to trigger mode
 *  @return Success, return MV_OK, and the camera trigger mode will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetTriggerMode(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetTriggerDelay(IN void* handle, IN OUT MVCC_FLOATVALUE* pstValue);
 *  @brief  鑾峰彇瑙﹀彂寤舵椂
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          MVCC_FLOATVALUE* pstValue   [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈鸿Е鍙戝欢鏃剁殑淇℃伅缁撴瀯浣撴寚閽?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞惰幏寰楃浉搴斿弬鏁颁俊鎭殑缁撴瀯浣? 澶辫触, 杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetFrameRate
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetTriggerDelay(IN void* handle, IN OUT MVCC_FLOATVALUE* pstValue);
 *  @brief  Get tigger delay
 *  @param  void* handle                [IN]        Handle
 *          MVCC_FLOATVALUE* pstValue   [IN][OUT]   Structure pointer of trigger delay
 *  @return Success, return MV_OK, and get the structure of the corresponding parameters. Failure, return error code
 *  
 *          Refer to MV_CC_GetFrameRate
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetTriggerDelay(IN void* handle, IN OUT MVCC_FLOATVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetTriggerDelay(IN void* handle, IN const float fValue);
 *  @brief  璁剧疆瑙﹀彂寤舵椂
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const float fValue          [IN]        鎯宠璁剧疆鐨勭浉鏈鸿Е鍙戝欢鏃?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満瑙﹀彂寤舵椂灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetTriggerDelay(IN void* handle, IN const float fValue);
 *  @brief  Set tigger delay
 *  @param  void* handle                [IN]        Handle
 *          const float fValue          [IN]        Trigger delay to set
 *  @return Success, return MV_OK, and the camera trigger delay will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetTriggerDelay(IN void* handle, IN const float fValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetTriggerSource(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);
 *  @brief  鑾峰彇瑙﹀彂婧?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          MVCC_ENUMVALUE* pstValue    [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呯殑鏈夊叧瑙﹀彂婧愮殑淇℃伅缁撴瀯浣撴寚閽?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞惰幏寰楃浉搴斿弬鏁颁俊鎭殑缁撴瀯浣? 澶辫触, 杩斿洖閿欒鐮?
 *  
            鍙弬鐓ф帴鍙V_CC_GetPixelFormat锛屽弬鑰?CameraParam.h 涓殑 MV_CAM_TRIGGER_SOURCE 瀹氫箟
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetTriggerSource(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);
 *  @brief  Get trigger source
 *  @param  void* handle                [IN]        Handle
 *          MVCC_ENUMVALUE* pstValue    [IN][OUT]   Structure pointer of trigger source
 *  @return Success, return MV_OK, and get the structure of the corresponding parameters. Failure, return error code
 *  
            Refer to MV_CC_GetPixelFormat and definition of MV_CAM_TRIGGER_SOURCE in CameraParam.h
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetTriggerSource(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetTriggerSource(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆瑙﹀彂婧?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        瑕佽缃殑瑙﹀彂婧愬搴旂殑鏁村瀷鍊?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満瑙﹀彂婧愬皢浼氭洿鏀逛负鐩稿簲鍊硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetTriggerSource(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set trigger source
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        Integer value to set corresponding to trigger source
 *  @return Success, return MV_OK, and the camera trigger source will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetTriggerSource(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_TriggerSoftwareExecute(IN void* handle);
 *  @brief  杞Е鍙戜竴娆★紙鎺ュ彛浠呭湪宸查€夋嫨鐨勮Е鍙戞簮涓鸿蒋浠惰Е鍙戞椂鏈夋晥锛?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @return 鎴愬姛,杩斿洖MV_OK, 澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_TriggerSoftwareExecute(IN void* handle);
 *  @brief  Execute software trigger once (this interface only valid when the trigger source is set to software)
 *  @param  void* handle                [IN]        Handle
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_TriggerSoftwareExecute(IN void* handle);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetGammaSelector(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);
 *  @brief  鑾峰彇Gamma绫诲瀷
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          MVCC_ENUMVALUE* pstValue    [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呯殑鏈夊叧Gamma绫诲瀷鐨勪俊鎭粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,骞惰幏寰楃浉搴斿弬鏁颁俊鎭殑缁撴瀯浣? 澶辫触, 杩斿洖閿欒鐮?
 *  
            鍙弬鐓ф帴鍙V_CC_GetPixelFormat锛屽弬鑰?CameraParam.h 涓殑 MV_CAM_GAMMA_SELECTOR 瀹氫箟
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetGammaSelector(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);
 *  @brief  Get Gamma mode
 *  @param  void* handle                [IN]        Handle
 *          MVCC_ENUMVALUE* pstValue    [IN][OUT]   Structure pointer of gamma mode
 *  @return Success, return MV_OK, and get the structure of the corresponding parameters. Failure, return error code
 *  
            Refer to MV_CC_GetPixelFormat and definition of MV_CAM_GAMMA_SELECTOR in CameraParam.h
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetGammaSelector(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetGammaSelector(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆Gamma绫诲瀷
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        瑕佽缃殑Gamma绫诲瀷瀵瑰簲鐨勬暣鍨嬪€?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満Gamma绫诲瀷灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetGammaSelector(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set Gamma mode
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        Integer value to set corresponding to gamma mode
 *  @return Success, return MV_OK, and the camera gamma mode will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetGammaSelector(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetGamma(IN void* handle, IN OUT MVCC_FLOATVALUE* pstValue);
 *  @brief  鑾峰彇Gamma鍊?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          MVCC_FLOATVALUE* pstValue   [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈篏amma鍊肩殑淇℃伅缁撴瀯浣撴寚閽?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞惰幏寰楃浉搴斿弬鏁颁俊鎭殑缁撴瀯浣? 澶辫触, 杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetFrameRate
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetGamma(IN void* handle, IN OUT MVCC_FLOATVALUE* pstValue);
 *  @brief  Get Gamma value
 *  @param  void* handle                [IN]        Handle
 *          MVCC_FLOATVALUE* pstValue   [IN][OUT]   Structure pointer of gamma value
 *  @return Success, return MV_OK, and get the structure of the corresponding parameters. Failure, return error code
 *  
 *          Refer to MV_CC_GetFrameRate
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetGamma(IN void* handle, IN OUT MVCC_FLOATVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetGamma(IN void* handle, IN const float fValue);
 *  @brief  璁剧疆Gamma鍊?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const float fValue          [IN]        鎯宠璁剧疆鐨勭浉鏈篏amma鍊?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満Gamma鍊煎皢浼氭洿鏀逛负鐩稿簲鍊硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetGamma(IN void* handle, IN const float fValue);
 *  @brief  Set Gamma value
 *  @param  void* handle                [IN]        Handle
 *          const float fValue          [IN]        Gamma value to set
 *  @return Success, return MV_OK, and the camera gamma value will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetGamma(IN void* handle, IN const float fValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetSharpness(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇閿愬害
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈洪攼搴︾粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetSharpness(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get sharpness
 *  @param  void* handle                [IN]        Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Structure pointer of sharpness
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetSharpness(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetSharpness(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆閿愬害
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勯攼搴?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満閿愬害灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetSharpness(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set sharpness
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        Sharpness to set
 *  @return Success, return MV_OK, and the camera sharpness will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetSharpness(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetHue(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇鐏板害
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈虹伆搴︾粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetHue(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get Hue
 *  @param  void* handle                [IN]        Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Structure pointer of Hue
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetHue(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetHue(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆鐏板害
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勭伆搴?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満鐏板害灏嗕細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetHue(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set Hue
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        Hue to set
 *  @return Success, return MV_OK, and the camera Hue will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetHue(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetSaturation(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇楗卞拰搴?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈洪ケ鍜屽害缁撴瀯浣撴寚閽?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
  *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetSaturation(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get Saturation
 *  @param  void* handle                [IN]        Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Structure pointer of Saturation
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetSaturation(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetSaturation(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆楗卞拰搴?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勯ケ鍜屽害
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満楗卞拰搴﹀皢浼氭洿鏀逛负鐩稿簲鍊硷紝澶辫触,杩斿洖閿欒鐮?
 
*  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetSaturation(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set Saturation
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        Saturation to set
 *  @return Success, return MV_OK, and the camera Saturation will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetSaturation(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetBalanceWhiteAuto(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);
 *  @brief  鑾峰彇鑷姩鐧藉钩琛?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          MVCC_ENUMVALUE* pstValue    [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呯殑鏈夊叧鑷姩鐧藉钩琛＄殑淇℃伅缁撴瀯浣撴寚閽?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞惰幏寰楃浉搴斿弬鏁颁俊鎭殑缁撴瀯浣? 澶辫触, 杩斿洖閿欒鐮?
 *  
            鍙弬鐓ф帴鍙V_CC_GetPixelFormat锛屽弬鑰?CameraParam.h 涓殑 MV_CAM_BALANCEWHITE_AUTO 瀹氫箟
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetBalanceWhiteAuto(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);
 *  @brief  Get Auto white balance
 *  @param  void* handle                [IN]        Handle
 *          MVCC_ENUMVALUE* pstValue    [IN][OUT]   Structure pointer of auto white balance
 *  @return Success, return MV_OK, and get the structure of the corresponding parameters. Failure, return error code
 *  
            Refer to MV_CC_GetPixelFormat and definition of MV_CAM_BALANCEWHITE_AUTO in CameraParam.h
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetBalanceWhiteAuto(IN void* handle, IN OUT MVCC_ENUMVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetBalanceWhiteAuto(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆鑷姩鐧藉钩琛?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        瑕佽缃殑鑷姩鐧藉钩琛″搴旂殑鏁村瀷鍊?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満鑷姩鐧藉钩琛″皢浼氭洿鏀逛负鐩稿簲鍊硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetBalanceWhiteAuto(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set Auto white balance
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        Integer value to set corresponding to auto white balance
 *  @return Success, return MV_OK, and the camera auto white balance will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetBalanceWhiteAuto(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetBalanceRatioRed(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇鐧藉钩琛?绾?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈虹櫧骞宠　 绾㈢粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetBalanceRatioRed(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get white balance red
 *  @param  void* handle                [IN]        Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Structure pointer of white balance red
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetBalanceRatioRed(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetBalanceRatioRed(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆鐧藉钩琛?绾?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勭櫧骞宠　 绾?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満鐧藉钩琛?绾㈠皢浼氭洿鏀逛负鐩稿簲鍊硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetBalanceRatioRed(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set white balance red
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        White balance red to set
 *  @return Success, return MV_OK, and the camera white balance red will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetBalanceRatioRed(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetBalanceRatioGreen(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇鐧藉钩琛?缁?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈虹櫧骞宠　 缁跨粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetBalanceRatioGreen(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get white balance green
 *  @param  void* handle                [IN]        Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Structure pointer of white balance green
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetBalanceRatioGreen(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetBalanceRatioGreen(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆鐧藉钩琛?缁?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勭櫧骞宠　 缁?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満鐧藉钩琛?缁垮皢浼氭洿鏀逛负鐩稿簲鍊硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetBalanceRatioGreen(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set white balance green
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        White balance green to set
 *  @return Success, return MV_OK, and the camera white balance green will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetBalanceRatioGreen(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetBalanceRatioBlue(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇鐧藉钩琛?钃?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈虹櫧骞宠　 钃濈粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetBalanceRatioBlue(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get white balance blue
 *  @param  void* handle                [IN]        Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Structure pointer of white balance blue
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetBalanceRatioBlue(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetBalanceRatioBlue(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆鐧藉钩琛?钃?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勭櫧骞宠　 钃?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満鐧藉钩琛?钃濆皢浼氭洿鏀逛负鐩稿簲鍊硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetBalanceRatioBlue(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set white balance blue
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        White balance blue to set
 *  @return Success, return MV_OK, and the camera white balance blue will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetBalanceRatioBlue(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetFrameSpecInfoAbility(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇姘村嵃淇℃伅鍐呭寘鍚殑淇℃伅绫诲瀷
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈烘按鍗颁俊鎭唴鍖呭惈鐨勪俊鎭被鍨嬬粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetFrameSpecInfoAbility(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get information type included by frame stamp
 *  @param  void* handle                [IN]        Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Structure pointer of information type included by frame stamp
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetFrameSpecInfoAbility(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetFrameSpecInfoAbility(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆姘村嵃淇℃伅鍐呭寘鍚殑淇℃伅绫诲瀷
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勬按鍗颁俊鎭唴鍖呭惈鐨勪俊鎭被鍨?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満姘村嵃淇℃伅鍐呭寘鍚殑淇℃伅绫诲瀷浼氭洿鏀逛负鐩稿簲鍊硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetFrameSpecInfoAbility(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set information type included by frame stamp
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        Information type included by frame stamp to set
 *  @return Success, return MV_OK, and the camera information type included by frame stamp will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetFrameSpecInfoAbility(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetDeviceUserID(IN void* handle, IN OUT MVCC_STRINGVALUE* pstValue);
 *  @brief  鑾峰彇璁惧鑷畾涔夊悕瀛?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          MVCC_STRINGVALUE* pstValue  [IN OUT]    杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈哄悕瀛楃粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鑾峰彇鍒扮浉鏈虹殑鑷畾涔夊悕瀛楋紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetDeviceUserID(IN void* handle, IN OUT MVCC_STRINGVALUE* pstValue);
 *  @brief  Get device user defined name
 *  @param  void* handle                [IN]        Handle
 *          MVCC_STRINGVALUE* pstValue  [IN OUT]    Structure pointer of device name
 *  @return Success, return MV_OK, and get the camera user defined name. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetDeviceUserID(IN void* handle, IN OUT MVCC_STRINGVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetDeviceUserID(IN void* handle, IN const char* chValue);
 *  @brief  璁剧疆璁惧鑷畾涔夊悕瀛?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          IN const char* chValue      [IN]        璁惧鍚嶅瓧
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖璁剧疆璁惧鑷畾涔夊悕瀛楋紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetDeviceUserID(IN void* handle, IN const char* chValue);
 *  @brief  Set device user defined name
 *  @param  void* handle                [IN]        Handle
 *          IN const char* chValue      [IN]        Device name
 *  @return Success, return MV_OK, and set the camera user defined name. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetDeviceUserID(IN void* handle, IN const char* chValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetBurstFrameCount(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇涓€娆¤Е鍙戠殑甯ф暟
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈轰竴娆¤Е鍙戠殑甯ф暟缁撴瀯浣撴寚閽?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetBurstFrameCount(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get frame number trigger by once
 *  @param  void* handle                [IN]        Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Structure pointer of frame number trigger by once
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetBurstFrameCount(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetBurstFrameCount(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆涓€娆¤Е鍙戠殑甯ф暟
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勪竴娆¤Е鍙戠殑甯ф暟
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満涓€娆¤Е鍙戠殑甯ф暟浼氭洿鏀逛负鐩稿簲鍊硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetBurstFrameCount(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set frame number trigger by once
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        Frame number trigger by once to set
 *  @return Success, return MV_OK, and the camera frame number trigger by once will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetBurstFrameCount(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetAcquisitionLineRate(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇琛岄
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈鸿棰戠粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetAcquisitionLineRate(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get line rate
 *  @param  void* handle                [IN]        Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Structure pointer of line rate
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetAcquisitionLineRate(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetAcquisitionLineRate(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆琛岄
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勮棰?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満琛岄浼氭洿鏀逛负鐩稿簲鍊硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetAcquisitionLineRate(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set line rate
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        Line rate to set
 *  @return Success, return MV_OK, and the camera line rate will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetAcquisitionLineRate(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetHeartBeatTimeout(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇蹇冭烦淇℃伅
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈哄績璺充俊鎭粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_GetHeartBeatTimeout(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get heartbeat information
 *  @param  void* handle                [IN]        Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Structure pointer of heartbeat information
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_GetHeartBeatTimeout(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetHeartBeatTimeout(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆蹇冭烦淇℃伅
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勫績璺充俊鎭?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満蹇冭烦淇℃伅浼氭洿鏀逛负鐩稿簲鍊硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_CC_SetHeartBeatTimeout(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set heartbeat information
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        Heartbeat information to set
 *  @return Success, return MV_OK, and the camera heartbeat information will change to the corresponding value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CC_SetHeartBeatTimeout(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_GIGE_GetGevSCPSPacketSize(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇缃戠粶鍖呭ぇ灏?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈虹綉缁滃寘澶у皬缁撴瀯浣撴寚閽?
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_GIGE_GetGevSCPSPacketSize(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get network packet size
 *  @param  void* handle                [IN]        Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Structure pointer of network packet size
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_GetGevSCPSPacketSize(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_GIGE_SetGevSCPSPacketSize(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆缃戠粶鍖呭ぇ灏?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勭綉缁滃寘澶у皬
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満缃戠粶鍖呭ぇ灏忎細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_GIGE_SetGevSCPSPacketSize(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set network packet size
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        Packet size to set
 *  @return Success, return MV_OK, and change packet size to setting value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_SetGevSCPSPacketSize(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_GIGE_GetGevSCPD(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  鑾峰彇缃戠粶鍖呭彂閫侀棿闅?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮湁鍏崇浉鏈虹綉缁滃寘鍙戦€侀棿闅旂粨鏋勪綋鎸囬拡
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  
 *          鍙弬鐓ф帴鍙V_CC_GetWidth
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_GIGE_GetGevSCPD(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);
 *  @brief  Get network packet sending delay
 *  @param  void* handle                [IN]        Handle
 *  @param  MVCC_INTVALUE* pstValue     [IN][OUT]   Structure pointer of network packet sending delay
 *  @return Success, return MV_OK. Failure, return error code
 *  
 *          Refer to MV_CC_GetWidth
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_GetGevSCPD(IN void* handle, IN OUT MVCC_INTVALUE* pstValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_GIGE_SetGevSCPD(IN void* handle, IN const unsigned int nValue);
 *  @brief  璁剧疆缃戠粶鍖呭彂閫侀棿闅?
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          const unsigned int nValue   [IN]        鎯宠璁剧疆鐨勭綉缁滃寘鍙戦€侀棿闅?
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満缃戠粶鍖呭彂閫侀棿闅斾細鏇存敼涓虹浉搴斿€硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_GIGE_SetGevSCPD(IN void* handle, IN const unsigned int nValue);
 *  @brief  Set network packet sending delay
 *  @param  void* handle                [IN]        Handle
 *          const unsigned int nValue   [IN]        Packet delay to set
 *  @return Success, return MV_OK, and change packet delay to setting value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_SetGevSCPD(IN void* handle, IN const unsigned int nValue);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_GIGE_GetGevSCDA(IN void* handle, unsigned int* pnIP);
 *  @brief  鑾峰彇鎺ユ敹绔疘P鍦板潃锛?xa9fe0102 琛ㄧず 169.254.1.2
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  unsigned int* pnIP          [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呮帴鏀剁IP鍦板潃
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_GIGE_GetGevSCDA(IN void* handle, unsigned int* pnIP);
 *  @brief  Get receiver IP address, 0xa9fe0102 indicates 169.254.1.2
 *  @param  void* handle                [IN]        Handle
 *  @param  unsigned int* pnIP          [IN][OUT]   Receiver IP address
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_GetGevSCDA(IN void* handle, unsigned int* pnIP);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_GIGE_SetGevSCDA(IN void* handle, unsigned int nIP);
 *  @brief  璁剧疆鎺ユ敹绔疘P鍦板潃
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          unsigned int nIP            [IN]        鎯宠璁剧疆鐨勬帴鏀剁IP鍦板潃
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満鎺ユ敹绔疘P鍦板潃浼氭洿鏀逛负鐩稿簲鍊硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_GIGE_SetGevSCDA(IN void* handle, unsigned int nIP);
 *  @brief  Set receiver IP address
 *  @param  void* handle                [IN]        Handel
 *          unsigned int nIP            [IN]        Receiver IP address to set
 *  @return Success, return MV_OK, and change receiver IP address to setting value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_SetGevSCDA(IN void* handle, unsigned int nIP);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_GIGE_GetGevSCSP(IN void* handle, unsigned int* pnPort);
 *  @brief  鑾峰彇鍙戦€佺鐨勭鍙ｅ彿
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *  @param  unsigned int* pnPort        [IN][OUT]   杩斿洖缁欒皟鐢ㄨ€呭彂閫佺鐨勭鍙ｅ彿
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_GIGE_GetGevSCSP(IN void* handle, unsigned int* pnPort);
 *  @brief  Get transmitter port number
 *  @param  void* handle                [IN]        Handle
 *  @param  unsigned int* pnPort        [IN][OUT]   Transmitter port number
 *  @return Success, return MV_OK. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_GetGevSCSP(IN void* handle, unsigned int* pnPort);

/************************************************************************
 *  @fn     MVSDKEXPORT_API int __stdcall MV_GIGE_SetGevSCSP(IN void* handle, unsigned int nPort);
 *  @brief  璁剧疆鍙戦€佺鐨勭鍙ｅ彿
 *  @param  void* handle                [IN]        鐩告満鍙ユ焺
 *          unsigned int nPort          [IN]        鎯宠璁剧疆鐨勫彂閫佺鐨勭鍙ｅ彿
 *  @return 鎴愬姛,杩斿洖MV_OK,骞朵笖鐩告満鍙戦€佺鐨勭鍙ｅ彿浼氭洿鏀逛负鐩稿簲鍊硷紝澶辫触,杩斿洖閿欒鐮?
 
 *  @fn     MVSDKEXPORT_API int __stdcall MV_GIGE_SetGevSCSP(IN void* handle, unsigned int nPort);
 *  @brief  Set transmitter port number
 *  @param  void* handle                [IN]        Handle
 *          unsigned int nPort          [IN]        Transmitter port number to set
 *  @return Success, return MV_OK, and change transmitter port number to setting value. Failure, return error code
 ************************************************************************/
MVSDKEXPORT_API int __stdcall MV_GIGE_SetGevSCSP(IN void* handle, unsigned int nPort);

/********************************************************************//**
 *  @~chinese
 *  @brief  璁剧疆璁惧娉㈢壒鐜?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  nBaudrate                   [IN]            璁剧疆鐨勬尝鐗圭巼鍊硷紝鏁板€煎弬鑰僀ameraParams.h涓畯瀹氫箟锛屽#define MV_CAML_BAUDRATE_9600  0x00000001
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 锛堣鎺ュ彛宸插純鐢紝寤鸿鏀圭敤 MV_CAML_SetDeviceBaudrate鎺ュ彛锛?
 
 *  @~english
 *  @brief  Set device baudrate using one of the CL_BAUDRATE_XXXX value
 *  @param  handle                      [IN]            Device handle
 *  @param  nBaudrate                   [IN]            baud rate to set. Refer to the 'CameraParams.h' for parameter definitions, for example, #define MV_CAML_BAUDRATE_9600  0x00000001
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks (This interface is abandoned, it is recommended to use the MV_CAML_SetDeviceBaudrate)
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CAML_SetDeviceBauderate(IN void* handle, unsigned int nBaudrate);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇璁惧娉㈢壒鐜?
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pnCurrentBaudrate           [OUT]           娉㈢壒鐜囦俊鎭寚閽堬紝鏁板€煎弬鑰僀ameraParams.h涓畯瀹氫箟锛屽#define MV_CAML_BAUDRATE_9600  0x00000001
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 锛堣鎺ュ彛宸插純鐢紝寤鸿鏀圭敤 MV_CAML_GetDeviceBaudrate鎺ュ彛锛?
 
 *  @~english
 *  @brief  Returns the current device baudrate, using one of the CL_BAUDRATE_XXXX value
 *  @param  handle                      [IN]            Device handle
 *  @param  pnCurrentBaudrate           [OUT]           Return pointer of baud rate to user. Refer to the 'CameraParams.h' for parameter definitions, for example, #define MV_CAML_BAUDRATE_9600  0x00000001
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks (This interface is abandoned, it is recommended to use the MV_CAML_GetDeviceBaudrate)
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CAML_GetDeviceBauderate(IN void* handle,unsigned int* pnCurrentBaudrate);

/********************************************************************//**
 *  @~chinese
 *  @brief  鑾峰彇璁惧涓庝富鏈洪棿杩炴帴鏀寔鐨勬尝鐗圭巼
 *  @param  handle                      [IN]            璁惧鍙ユ焺
 *  @param  pnBaudrateAblity            [OUT]           鏀寔鐨勬尝鐗圭巼淇℃伅鐨勬寚閽堛€傛墍鏀寔娉㈢壒鐜囩殑鎴栬繍绠楃粨鏋滐紝鍗曚釜鏁板€煎弬鑰僀ameraParams.h涓畯瀹氫箟锛屽MV_CAML_BAUDRATE_9600  0x00000001
 *  @return 鎴愬姛,杩斿洖MV_OK,澶辫触,杩斿洖閿欒鐮?
 *  @remarks 锛堣鎺ュ彛宸插純鐢紝寤鸿鏀圭敤 MV_CAML_GetSupportBaudrates鎺ュ彛锛?
 
 *  @~english
 *  @brief  Returns supported baudrates of the combined device and host interface
 *  @param  handle                      [IN]            Device handle
 *  @param  pnBaudrateAblity            [OUT]           Return pointer of the supported baudrates to user. 'OR' operation results of the supported baudrates. Refer to the 'CameraParams.h' for single value definitions, for example, MV_CAML_BAUDRATE_9600  0x00000001
 *  @return Success, return MV_OK. Failure, return error code 
 *  @remarks (This interface is abandoned, it is recommended to use the MV_CAML_GetSupportBaudrates)
************************************************************************/
MVSDKEXPORT_API int __stdcall MV_CAML_GetSupportBauderates(IN void* handle,unsigned int* pnBaudrateAblity);

#ifdef __cplusplus
}
#endif 

#endif //_MV_SDK_EXPORT_H__
