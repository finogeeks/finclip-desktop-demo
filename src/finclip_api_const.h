#ifndef SRC_FINCLIP_API_CONST_H_
#define SRC_FINCLIP_API_CONST_H_
#ifndef WRAPPER_SRC_PUBLIC_FINCLIP_API_CONST_H_
#define WRAPPER_SRC_PUBLIC_FINCLIP_API_CONST_H_

#define FIN_SID const char*
#define FIN_OK 0
#define FIN_FAIL 1

#ifdef _WIN32
#include <windows.h>
#define FINCLIP_WINDOW_HANDLE HWND
#elif defined __linux__
#include <gtk/gtk.h>
#define FINCLIP_WINDOW_HANDLE GtkWindow*
#elif defined __APPLE__
typedef struct objc_object* FINCLIP_WINDOW_HANDLE;
#endif

#ifdef _WIN32
#ifndef FINSTDMETHODCALLTYPE
#define FINSTDMETHODCALLTYPE __stdcall
#define DLL_EXPORT _declspec(dllexport)
#endif
#else
#define FINSTDMETHODCALLTYPE
#define DLL_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 启动机制设置
 * kAsync: 异步启动
 * kBaseLibrarySync: 基础库同步加载
 * kAppletSync: 小程序同步加载
 */
enum StartFlags {
  kAsync = 0,
  kBaseLibrarySync = 1 << 0,
  kAppletSync = 1 << 1,
};

/**
 * @brief 自定义API类型，分别用于小程序和jssdk
 *
 */
typedef enum { kApplet, kWebView } FinClipApiType;

/**
 * @brief 自定义api返回值回调
 *
 */
typedef void (*FinclipApiCallback)(const char* res);

/**
 * @brief 自定义api
 *
 */
typedef void (*FinclipApiHandle)(const char* event, const char* param,
                                 FinclipApiCallback callback);
#ifdef __cplusplus
}
#endif
#endif /* WRAPPER_SRC_PUBLIC_FINCLIP_API_CONST_H_ */

#endif /* SRC_FINCLIP_API_CONST_H_ */
