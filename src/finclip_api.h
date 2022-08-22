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
typedef struct FinclipParams FinclipParams;
typedef struct IFinConfigPacker IFinConfigPacker;
typedef struct IPackerFactory IPackerFactory;
typedef struct FinclipCallback FinclipCallback;
typedef void (*FinClipSDKCallback)(IEvent*);

/**
 * @brief SDK初始化
 */
DLL_EXPORT int FINSTDMETHODCALLTYPE
finclip_initialize(IFinConfigPacker* configpacker);

/**
 * @brief 获取配置工厂
 */
DLL_EXPORT IPackerFactory* FINSTDMETHODCALLTYPE finclip_get_packer_factory();

/**
 * @brief 启动
 */
DLL_EXPORT int FINSTDMETHODCALLTYPE finclip_start_applet(const char* appstore,
                                                         const char* appid);
DLL_EXPORT int FINSTDMETHODCALLTYPE finclip_start_applet_params(
    const char* appstore, const char* appid, void* params);
/**
 * @brief 关闭所有小程序
 */
DLL_EXPORT int FINSTDMETHODCALLTYPE finclip_close_all_applet();

/**
 * @brief 根据appid关闭小程序
 */
DLL_EXPORT int FINSTDMETHODCALLTYPE finclip_close_applet(const char* appid);

/**
 * @brief 设置小程序窗口位置
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
 * @brief 新建配置
 */
DLL_EXPORT FinclipParams* FINSTDMETHODCALLTYPE
finclip_config_packer_new_config(IFinConfigPacker* packer);

/**
 * @brief 获取配置, 如果获取失败, 则返回NULL
 */
DLL_EXPORT FinclipParams* FINSTDMETHODCALLTYPE
finclip_config_packer_get_config(IFinConfigPacker* packer, char* appstore);

/**
 * @brief 增加配置
 */
DLL_EXPORT int FINSTDMETHODCALLTYPE finclip_config_packer_add_config(
    IFinConfigPacker* packer, FinclipParams* config);

/**
 * @brief 注册自定义api, 供小程序或h5调用
 * @param packer
 * @param type 类型, 区分h5与小程序
 * @param apis api名称
 * @param handle 处理函数
 * @param input 自定义参数, 透传给handle
 */
DLL_EXPORT void FINSTDMETHODCALLTYPE
finclip_register_api(IFinConfigPacker* packer, FinClipApiType type,
                     const char* apis, FinclipApiHandle handle, void* input);

/**
 * @brief 创建启动参数
 */
DLL_EXPORT FinclipParams* FINSTDMETHODCALLTYPE finclip_create_params();

/**
 * @brief 销毁启动参数
 */
DLL_EXPORT void FINSTDMETHODCALLTYPE
finclip_destory_params(FinclipParams* params);

/**
 * @brief 设置启动参数
 */
DLL_EXPORT void FINSTDMETHODCALLTYPE finclip_params_set(FinclipParams* params,
                                                        const char* key,
                                                        const char* value);
/**
 * @brief 根据key删除启动启动参数
 */
DLL_EXPORT void FINSTDMETHODCALLTYPE finclip_params_del(FinclipParams* params,
                                                        const char* key);

#ifdef _WIN32
DLL_EXPORT int FINSTDMETHODCALLTYPE finclip_embed_applet(const char* appstore,
                                                         const char* appid,
                                                         HWND container);
DLL_EXPORT int FINSTDMETHODCALLTYPE finclip_start_applet_embed(
    const char* appstore, const char* appid, void* params, HWND container);
#endif

/**
 * @brief 调用h5中的自定义api, 小程序不支持自定义api
 */
DLL_EXPORT int FINSTDMETHODCALLTYPE finclip_invoke_api(
    FinClipApiType type, const char* app_id, const char* api_name,
    const char* params, FinclipApiCallback callback, void* input);

DLL_EXPORT int FINSTDMETHODCALLTYPE finclip_batch_app_info(
    const char* app_id, const char* req_list, FinclipApiCallback callback);

DLL_EXPORT int FINSTDMETHODCALLTYPE finclip_search_app(
    const char* app_id, const char* search_text, FinclipApiCallback callback);

/**
 * @brief 回调函数设置返回值
 */
DLL_EXPORT int FINSTDMETHODCALLTYPE finclip_callback_res(const char* app_id,
                                                         int callback_id,
                                                         void* result);
#ifdef __cplusplus
}
#endif

#endif /* WRAPPER_SRC_PUBLIC_FINCLIP_API_H_ */
