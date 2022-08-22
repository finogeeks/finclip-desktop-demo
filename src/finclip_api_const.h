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

// Config: 小程序框架的配置信息
#define FINCLIP_CONFIG_APPSTORE "appstore"
// appkey, secret, domain 从管理后台获取, 必填
#define FINCLIP_CONFIG_APPKEY "appkey"
#define FINCLIP_CONFIG_SECRET "secret"
#define FINCLIP_CONFIG_DOMAIN "domain"
// windows: finclip.exe位置, 默认为 ./finclip, 即当前目录下的finclip文件夹
// macos: finclip.app的位置, 默认为
#define FINCLIP_CONFIG_EXE_PATH "exe_path"

// Params: 每个小程序独立设置, 0: 独立模式, 1: 嵌入模式
// 注意, 嵌入模式目前仅支持windows, 且必须调用finclip_embed_applet, 完成嵌入.
#define FINCLIP_UI_CONFIG_WINDOW_TYPE "window_type"
// 是否展示loading动画, 1: 展示, 0: 不展示
#define FINCLIP_UI_CONFIG_SHOW_LOADING "show_loading"
// 标题栏更多菜单设置, hidden: 隐藏, normal: 显示
#define FINCLIP_UI_CONFIG_TITLE_BAR_MORE_BUTTON "title_bar_more_button"
// 初始窗口高度
#define FINCLIP_UI_CONFIG_WINDOW_HEIGHT "window_height"
// 初始窗口宽度
#define FINCLIP_UI_CONFIG_WINDOW_WIDTH "window_width"

// 小程序首屏路径
#define FINCLIP_PARAMS_PAGE_PATH "page_path"
// 传给小程序的启动参数
#define FINCLIP_PARAMS_START_PARAMS "start_params"
// 启动设置, 可以设置是否同步更新小程序, 基础库等, 默认异步,
// 参见下面的StartFlags
#define FINCLIP_PARAMS_START_FLAG "start_flag"

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
                                 void* input, int callbackid);

#ifdef __cplusplus
}
#endif
#endif /* WRAPPER_SRC_PUBLIC_FINCLIP_API_CONST_H_ */
