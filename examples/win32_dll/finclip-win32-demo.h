#pragma once

#include "./finclip_api.h"
#include "resource.h"

typedef IPackerFactory *(FINSTDMETHODCALLTYPE *findll_get_packer_factory)();
typedef IFinConfigPacker *(
    FINSTDMETHODCALLTYPE *findll_packer_factory_get_config_packer)(
    IPackerFactory *factory);
typedef int(FINSTDMETHODCALLTYPE *findll_config_packer_add_config)(
    IFinConfigPacker *packer, FinclipParams *config);

typedef void(FINSTDMETHODCALLTYPE *findll_register_api)(
    IFinConfigPacker *packer, FinClipApiType type, const char *apis,
    FinclipApiHandle handle, void *input);
typedef int(FINSTDMETHODCALLTYPE *findll_initialize)(
    IFinConfigPacker *configpacker);
typedef int(FINSTDMETHODCALLTYPE *findll_start_applet)(const char *appstore,
                                                       const char *appid);
typedef int(FINSTDMETHODCALLTYPE *findll_set_position)(const char *appid,
                                                       int left, int top,
                                                       int width, int height);
typedef int(FINSTDMETHODCALLTYPE *findll_invoke_api)(
    FinClipApiType type, const char *app_id, const char *api_name,
    const char *params, FinclipApiCallback callback, void *input);

typedef int(FINSTDMETHODCALLTYPE *findll_params_set)(FinclipParams *params,
                                                     const char *key,
                                                     const char *value);
typedef int(FINSTDMETHODCALLTYPE *findll_params_del)(FinclipParams *params,
                                                     const char *key);
typedef FinclipParams *(FINSTDMETHODCALLTYPE *findll_create_params)();
typedef void(FINSTDMETHODCALLTYPE *findll_destory_params)(
    FinclipParams *params);
typedef int(FINSTDMETHODCALLTYPE *findll_close_all_applet)();