#ifndef WRAPPER_SRC_PUBLIC_FINCLIP_API_H_
#define WRAPPER_SRC_PUBLIC_FINCLIP_API_H_

#include "finclip_api_const.h"

#ifdef __cplusplus
#include <cstddef>
#else
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct IKnown IKnown;
typedef struct IResultSet IResultSet;
typedef struct IEvent IEvent;
typedef struct IFinPacker IFinPacker;
typedef struct IFinConfig IFinConfig;
typedef struct IFinConfigPacker IFinConfigPacker;
typedef struct IPackerFactory IPackerFactory;
typedef struct Callback Callback;
typedef void (*FinClipSDKCallback)(IEvent*);

/**
 * @brief
 */
DLL_EXPORT int FINSTDMETHODCALLTYPE
finclip_initialize(IFinConfigPacker* configpacker);
/**
 * @brief
 */
DLL_EXPORT IPackerFactory* FINSTDMETHODCALLTYPE finclip_get_packer_factory();

/**
 * @brief
 */
DLL_EXPORT int FINSTDMETHODCALLTYPE finclip_start_applet(int appstore,
                                                         const char* appid);
/**
 * @brief
 */
DLL_EXPORT int FINSTDMETHODCALLTYPE finclip_close_all_applet();

/**
 * @brief 根据appid关闭小程序
 */
DLL_EXPORT int FINSTDMETHODCALLTYPE finclip_close_applet(const char* appid);

/**
 * @brief 设置小程序主程序位置
 * 需要设置
 *
 */
DLL_EXPORT void FINSTDMETHODCALLTYPE finclip_set_position(const char* appid,
                                                          int left, int top,
                                                          int width,
                                                          int height);

/**
 * @brief
 */
DLL_EXPORT IFinConfigPacker* FINSTDMETHODCALLTYPE
finclip_packer_factory_get_config_packer(IPackerFactory* factory);

/**
 * @brief
 */
DLL_EXPORT IFinConfig* FINSTDMETHODCALLTYPE
finclip_config_packer_new_config(IFinConfigPacker* packer);

/**
 * @brief
 */
DLL_EXPORT IFinConfig* FINSTDMETHODCALLTYPE
finclip_config_packer_get_config(IFinConfigPacker* packer, int appstore);

/**
 * @brief
 */
DLL_EXPORT int FINSTDMETHODCALLTYPE
finclip_config_packer_add_config(IFinConfigPacker* packer, IFinConfig* config);

/**
 * @brief
 */
DLL_EXPORT void FINSTDMETHODCALLTYPE
finclip_register_callback(IFinConfigPacker* packer, FinClipApiType type,
                          const char* apis, FinclipApiHandle handle);

/**
 * @brief
 */
DLL_EXPORT void FINSTDMETHODCALLTYPE
finclip_config_set_app_store(IFinConfig* config, int app_store);

/**
 * @brief
 */
DLL_EXPORT void FINSTDMETHODCALLTYPE
finclip_config_set_app_key(IFinConfig* config, const char* app_key);

/**
 * @brief
 */
DLL_EXPORT void FINSTDMETHODCALLTYPE
finclip_config_set_secret(IFinConfig* config, const char* secret);

/**
 * @brief
 */
DLL_EXPORT void FINSTDMETHODCALLTYPE
finclip_config_set_domain(IFinConfig* config, const char* domain);

/**
 * @brief
 */
DLL_EXPORT void FINSTDMETHODCALLTYPE
finclip_config_set_app_window_style(IFinConfig* config, int type);

/**
 * @brief 设置启动模式, 参见: StartFlags
 * 全同步模式: kBaseLibrarySync | kAppletSync
 */
DLL_EXPORT void FINSTDMETHODCALLTYPE
finclip_config_set_start_flag(IFinConfig* config, int flag);

/**
 * @brief 是否展示loading动画
 */
DLL_EXPORT void FINSTDMETHODCALLTYPE
finclip_config_set_show_loading(IFinConfig* config, bool show_loading);

/**
 * @brief 设置小程序主程序执行文件的位置, 当宿主程序和exe不在同一目录时,
 * 需要设置
 *
 */
DLL_EXPORT void FINSTDMETHODCALLTYPE
finclip_config_set_exe_path(IFinConfig* config, const char* exe_path);

#ifdef _WIN32
/**
 * @brief
 */
DLL_EXPORT int FINSTDMETHODCALLTYPE
finclip_start_applet_embed(int appstore, const char* appid, HWND container);
#endif

#ifdef __cplusplus

/**
 * @brief
 */
DLL_EXPORT void FINSTDMETHODCALLTYPE
finclip_register_callback_cpp(IFinConfigPacker* packer, FinClipApiType type,
                              const char* apis, Callback* callback);

/**
 * @brief
 */
DLL_EXPORT int FINSTDMETHODCALLTYPE finclip_invoke_api_cpp(FinClipApiType type,
                                                           const char* app_id,
                                                           const char* api_name,
                                                           const char* params,
                                                           Callback* callback);
#endif

#ifdef __cplusplus
}
#endif

#endif /* WRAPPER_SRC_PUBLIC_FINCLIP_API_H_ */