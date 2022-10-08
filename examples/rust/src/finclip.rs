use std::os::raw::{c_char, c_int, c_void};

#[repr(C)]
pub struct IPackerFactory;

#[repr(C)]
pub struct FinclipParams;

#[repr(C)]
pub struct IFinConfigPacker;

pub type ApiHandler = extern "C" fn(*const c_char, *const c_char, *mut c_void, *mut c_void);
pub type ApiCallback = extern "C" fn(*const c_char, *mut c_void);

#[allow(improper_ctypes)]
#[link(name = "FinClipSDKWrapper")]
extern "C" {
    #[allow(dead_code)]
    pub fn finclip_get_packer_factory() -> *mut IPackerFactory;

    #[allow(dead_code)]
    pub fn finclip_packer_factory_get_config_packer(
        factory: *mut IPackerFactory,
    ) -> *mut IFinConfigPacker;

    #[allow(dead_code)]
    pub fn finclip_initialize(configpacker: *mut IFinConfigPacker) -> c_int;

    #[allow(dead_code)]
    pub fn finclip_create_params() -> *mut FinclipParams;

    #[allow(dead_code)]
    pub fn finclip_destory_params(params: *mut FinclipParams);

    #[allow(dead_code)]
    pub fn finclip_params_set(params: *mut FinclipParams, key: *const c_char, value: *const c_char);

    #[allow(dead_code)]
    pub fn finclip_params_del(params: *mut FinclipParams, key: *const c_char);

    #[allow(dead_code)]
    pub fn finclip_config_packer_add_config(
        packer: *mut IFinConfigPacker,
        config: *mut FinclipParams,
    ) -> c_int;

    #[allow(dead_code)]
    pub fn finclip_config_packer_new_config(packer: *mut IFinConfigPacker) -> *mut FinclipParams;

    #[allow(dead_code)]
    pub fn finclip_config_packer_get_config(
        packer: *mut IFinConfigPacker,
        appstore: *const c_char,
    ) -> *mut FinclipParams;

    #[allow(dead_code)]
    pub fn finclip_set_position(
        appid: *const c_char,
        left: c_int,
        top: c_int,
        width: c_int,
        height: c_int,
    );

    #[allow(dead_code)]
    pub fn finclip_start_applet(appstore: *const c_char, appid: *const c_char) -> c_int;

    #[allow(dead_code)]
    pub fn finclip_register_api(
        packer: *mut IFinConfigPacker,
        typ: c_int,
        apis: *const c_char,
        handle: ApiHandler,
        input: *mut c_void,
    );

    #[allow(dead_code)]
    pub fn finclip_invoke_api(
        typ: c_int,
        app_id: *const c_char,
        api_name: *const c_char,
        params: *const c_char,
        callback: ApiCallback,
        input: *mut c_void,
    ) -> c_int;

    #[allow(dead_code)]
    pub fn finclip_close_applet(appid: *const c_char) -> c_int;

    #[allow(dead_code)]
    pub fn finclip_close_all_applet() -> c_int;

    #[allow(dead_code)]
    pub fn finclip_batch_app_info(
        appid: *const c_char,
        req_list: *const c_char,
        callback: ApiCallback,
        input: *mut c_void,
    ) -> c_int;

    #[allow(dead_code)]
    pub fn finclip_search_app(
        appid: *const c_char,
        search_text: *const c_char,
        callback: ApiCallback,
        input: *mut c_void,
    ) -> c_int;

    #[allow(dead_code)]
    pub fn finclip_callback_res(
        appid: *const c_char,
        callback_id: c_int,
        result: *mut c_void,
    ) -> c_int;

    #[allow(dead_code)]
    pub fn finclip_is_applet_open(appid: *const c_char) -> bool;
}

#[allow(dead_code)]
pub enum FinClipApiType {
    Applet = 0,
    WebView = 1,
}
