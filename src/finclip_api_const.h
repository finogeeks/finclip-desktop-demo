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

#define FINCLIP_CONFIG_APPSTORE "appstore"
// appkey, secret, domain 从管理后台获取, 必填
#define FINCLIP_CONFIG_APPKEY "appkey"
#define FINCLIP_CONFIG_SECRET "secret"
#define FINCLIP_CONFIG_DOMAIN "domain"
//
#define FINCLIP_CONFIG_WINDOW_STYLE "window_style"
// 是否展示loading动画, 1: 展示, 0: 不展示
#define FINCLIP_CONFIG_SHOW_LOADING "show_loading"
// windows: finclip.exe位置, 默认为 ./finclip, 即当前目录下的finclip文件夹
// macos: finclip.app的位置, 默认为
#define FINCLIP_CONFIG_EXE_PATH "exe_path"
// finclip.exe位置, 默认到当前目录的finclip文件夹下找
#define FINCLIP_CONFIG_PAGE_PATH "page_path"
// 启动参数, 值为序列化后的json
#define FINCLIP_CONFIG_START_PARAMS "start_params"
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
 * @brief 调用h5方法后的返回值回调
 *
 */
typedef void (*FinclipApiCallback)(const char* res, void* input);

/**
 * @brief 自定义api
 *
 */
typedef void (*FinclipApiHandle)(const char* event, const char* param,
                                 void* input, void* res);
#ifdef __cplusplus
}
#endif
#endif /* WRAPPER_SRC_PUBLIC_FINCLIP_API_CONST_H_ */
