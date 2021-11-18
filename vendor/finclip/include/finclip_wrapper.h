#ifndef __H_FINCLIP_WRAPPER_H__
#define __H_FINCLIP_WRAPPER_H__
#include "finclip_api.h"
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief 初始化SDI信息
 * @detail 只需要初始化一次
 *
 * @param hinstance 应用实例句柄
 * @param configpacker 配置打包器
 * @return 0成功，1失败
 */
DLL_EXPORT int FINSTDMETHODCALLTYPE Initialize(HINSTANCE hinstance, IFinConfigPacker* configpacker);

/**
 * @brief 函数简介
 * @detail 详细说明
 *
 * @param 形参 参数说明
 * @param 形参 参数说明
 * @return 返回说明
 *   @retval 返回值说明
 * @note 注解
 * @attention 注意
 * @warning 警告
 * @exception 异常
 */
DLL_EXPORT IPackerFactory* FINSTDMETHODCALLTYPE GetPackerFactory();

/**
 * @brief 函数简介
 * @detail 详细说明
 *
 * @param 形参 参数说明
 * @param 形参 参数说明
 * @return 返回说明
 *   @retval 返回值说明
 * @note 注解
 * @attention 注意
 * @warning 警告
 * @exception 异常
 */
DLL_EXPORT HRESULT FINSTDMETHODCALLTYPE CloseAllApplet();

/**
 * @brief 函数简介
 * @detail 详细说明
 *
 * @param 形参 参数说明
 * @param 形参 参数说明
 * @return 返回说明
 *   @retval 返回值说明
 * @note 注解
 * @attention 注意
 * @warning 警告
 * @exception 异常
 */
DLL_EXPORT HRESULT FINSTDMETHODCALLTYPE CleanCache();

///小程序Api
/**打开小程序
 *
 * @param appstore 应用市场类型，需要与配置信息里面设置的应用市场一致
 * @param appId 小程序appId
 * @param param 打开参数
 * @param callback 打开小程序回调
 * @return 0表示成功，1表示失败
 */
DLL_EXPORT HRESULT FINSTDMETHODCALLTYPE StartApplet(HWND hWnd, int apptype, const char* appid, const char* page_path,
                                                    IFinPacker* params, FinClipSDKCallback callback);

DLL_EXPORT HWND FINSTDMETHODCALLTYPE    SetAppletPos(const char* appid, int appstore, int left, int top, int width,
                                                     int height);
DLL_EXPORT HRESULT FINSTDMETHODCALLTYPE FinClipShutdown();

#ifdef __cplusplus
}
#endif
#endif
