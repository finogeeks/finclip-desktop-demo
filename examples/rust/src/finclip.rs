use std::{
    collections::HashMap,
    ffi::{CString, NulError},
    fmt::{self, Display},
    os::raw::{c_char, c_int, c_void},
};

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
}

fn str_to_cstr(k: &str) -> Result<CString, NulError> {
    let v = CString::new(k.as_bytes())?;
    Ok(v)
}

pub enum FinClipApiType {
    Applet = 0,
    WebView = 1,
}

#[derive(Debug)]
pub struct AppParams {
    pub appstore: String,
    pub appkey: String,
    pub secret: String,
    pub domain: String,
    pub appid: String,
    pub exe_path: String,
    pub show_loading: String,
    pub params: HashMap<String, String>,
}

impl Display for AppParams {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "({}, {}, {}, {}, {}, {}, {})",
            self.appstore,
            self.appid,
            self.appkey,
            self.secret,
            self.domain,
            self.show_loading,
            self.exe_path
        )
    }
}

pub mod wrapper {
    use super::*;

    #[allow(dead_code)]
    pub fn get_packer_factory() -> *mut IPackerFactory {
        unsafe { finclip_get_packer_factory() }
    }

    #[allow(dead_code)]
    pub fn packer_factory_get_config_packer(factory: *mut IPackerFactory) -> *mut IFinConfigPacker {
        unsafe { finclip_packer_factory_get_config_packer(factory) }
    }

    #[allow(dead_code)]
    pub fn initialize(configpacker: *mut IFinConfigPacker) -> c_int {
        unsafe { finclip_initialize(configpacker) }
    }

    #[allow(dead_code)]
    pub fn create_params() -> *mut FinclipParams {
        unsafe { finclip_create_params() }
    }

    #[allow(dead_code)]
    pub fn destory_params(params: *mut FinclipParams) {
        unsafe { finclip_destory_params(params) }
    }

    #[allow(dead_code)]
    pub fn params_set(params: *mut FinclipParams, key: &str, value: &str) -> Result<(), NulError> {
        unsafe {
            finclip_params_set(
                params,
                str_to_cstr(key)?.as_ptr(),
                str_to_cstr(value)?.as_ptr(),
            );
            Ok(())
        }
    }

    #[allow(dead_code)]
    pub fn params_del(params: *mut FinclipParams, key: &str) -> Result<(), NulError> {
        unsafe {
            finclip_params_del(params, str_to_cstr(key)?.as_ptr());
            Ok(())
        }
    }

    #[allow(dead_code)]
    pub fn config_packer_add_config(
        packer: *mut IFinConfigPacker,
        config: *mut FinclipParams,
    ) -> c_int {
        unsafe { finclip_config_packer_add_config(packer, config) }
    }

    #[allow(dead_code)]
    pub fn config_packer_new_config(packer: *mut IFinConfigPacker) -> *mut FinclipParams {
        unsafe { finclip_config_packer_new_config(packer) }
    }

    #[allow(dead_code)]
    pub fn config_packer_get_config(
        packer: *mut IFinConfigPacker,
        appstore: &str,
    ) -> Result<*mut FinclipParams, NulError> {
        unsafe {
            let ret = finclip_config_packer_get_config(packer, str_to_cstr(appstore)?.as_ptr());
            Ok(ret)
        }
    }

    #[allow(dead_code)]
    pub fn set_position(
        appid: &str,
        left: c_int,
        top: c_int,
        width: c_int,
        height: c_int,
    ) -> Result<(), NulError> {
        unsafe {
            finclip_set_position(str_to_cstr(appid)?.as_ptr(), left, top, width, height);
            Ok(())
        }
    }

    #[allow(dead_code)]
    pub fn start_applet(appstore: &str, appid: &str) -> Result<c_int, NulError> {
        unsafe {
            let ret = finclip_start_applet(
                str_to_cstr(appstore)?.as_ptr(),
                str_to_cstr(appid)?.as_ptr(),
            );
            Ok(ret)
        }
    }

    #[allow(dead_code)]
    pub fn register_api(
        packer: *mut IFinConfigPacker,
        typ: FinClipApiType,
        apis: &str,
        handle: ApiHandler,
        input: *mut c_void,
    ) -> Result<(), NulError> {
        unsafe {
            finclip_register_api(
                packer,
                typ as c_int,
                str_to_cstr(apis)?.as_ptr(),
                handle,
                input,
            );
            Ok(())
        }
    }

    #[allow(dead_code)]
    pub fn invoke_api(
        typ: FinClipApiType,
        app_id: &str,
        api_name: &str,
        params: &str,
        callback: ApiCallback,
        input: *mut c_void,
    ) -> Result<c_int, NulError> {
        unsafe {
            let ret = finclip_invoke_api(
                typ as c_int,
                str_to_cstr(app_id)?.as_ptr(),
                str_to_cstr(api_name)?.as_ptr(),
                str_to_cstr(params)?.as_ptr(),
                callback,
                input,
            );
            Ok(ret)
        }
    }

    #[allow(dead_code)]
    pub fn close_applet(appid: &str) -> Result<c_int, NulError> {
        unsafe {
            let ret = finclip_close_applet(str_to_cstr(appid)?.as_ptr());
            Ok(ret)
        }
    }

    #[allow(dead_code)]
    pub fn close_all_applet() -> c_int {
        unsafe { finclip_close_all_applet() }
    }

    #[allow(dead_code)]
    pub fn applet_set_params(params: *mut FinclipParams, from: &AppParams) -> Result<(), NulError> {
        for k in &from.params {
            params_set(params, k.0.as_str(), k.1.as_str())?;
        }
        params_set(params, "appstore", from.appstore.as_str())?;
        params_set(params, "appkey", from.appkey.as_str())?;
        params_set(params, "secret", from.secret.as_str())?;
        params_set(params, "domain", from.domain.as_str())?;
        params_set(params, "app_id", from.appid.as_str())?;
        if !from.exe_path.is_empty() {
            params_set(params, "exe_path", from.exe_path.as_str())?;
        }
        params_set(params, "show_loading", from.show_loading.as_str())?;
        Ok(())
    }

    #[allow(dead_code)]
    pub fn batch_app_info(
        appid: &str,
        req_list: &str,
        callback: ApiCallback,
        input: *mut c_void,
    ) -> Result<c_int, NulError> {
        unsafe {
            let ret = finclip_batch_app_info(
                str_to_cstr(appid)?.as_ptr(),
                str_to_cstr(req_list)?.as_ptr(),
                callback,
                input,
            );
            Ok(ret)
        }
    }

    #[allow(dead_code)]
    pub fn search_app(
        appid: &str,
        search_text: &str,
        callback: ApiCallback,
        input: *mut c_void,
    ) -> Result<c_int, NulError> {
        unsafe {
            let ret = finclip_search_app(
                str_to_cstr(appid)?.as_ptr(),
                str_to_cstr(search_text)?.as_ptr(),
                callback,
                input,
            );
            Ok(ret)
        }
    }

    #[allow(dead_code)]
    pub fn callback_res(
        appid: &str,
        callback_id: c_int,
        result: *mut FinclipParams,
    ) -> Result<c_int, NulError> {
        unsafe {
            let ret = finclip_callback_res(
                str_to_cstr(appid)?.as_ptr(),
                callback_id,
                result as *mut c_void,
            );
            Ok(ret)
        }
    }
}
