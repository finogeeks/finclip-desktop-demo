extern crate anyhow;
extern crate serde;
extern crate serde_json;

mod finclip;

use anyhow::Result;
use finclip::*;
use std::collections::HashMap;
use std::ffi::{CStr, CString, NulError};
use std::fmt::{Display, self};
use std::os::raw::{c_char, c_void};
use std::thread;
use std::time::Duration;

pub fn str_to_cstr(k: &str) -> Result<CString, NulError> {
    let v = CString::new(k.as_bytes())?;
    Ok(v)
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
            "AppParams({}, {}, {}, {}, {}, {}, {})",
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

extern "C" fn web_api_handler(
    res: *const c_char,
    _res: *const c_char,
    input: *mut c_void,
    _input: *mut c_void,
) {
    unsafe {
        let appid = {
            let data = &*(input as *mut AppParams);
            data.appid.clone()
        };
        println!(
            ">> web_api_callback {} {}",
            CStr::from_ptr(res).to_str().unwrap(),
            appid,
        );
        //finclip_callback_res(appid, callback_id, result);
    }
}

fn start_normal(cfg: &AppParams) -> Result<()> {
    unsafe {
        let factory = finclip_get_packer_factory();
        let packer = finclip_packer_factory_get_config_packer(factory);
        println!("get config packer.");
        let cfg_ptr = &cfg as *const _ as *mut c_void;
        finclip_register_api(
            packer,
            FinClipApiType::Applet as i32,
            str_to_cstr("openapi_search")?.as_ptr(),
            web_api_handler,
            cfg_ptr,
        );
        let res = finclip_initialize(packer);
        println!("initialize packer. result: {}", res);
        let config = finclip_create_params();
        println!("create params.");
        for k in &cfg.params {
            finclip_params_set(
                config,
                str_to_cstr(k.0.as_str())?.as_ptr(),
                str_to_cstr(k.1.as_str())?.as_ptr(),
            );
        }
        finclip_params_set(
            config,
            str_to_cstr("appstore")?.as_ptr(),
            str_to_cstr(cfg.appstore.as_str())?.as_ptr(),
        );
        finclip_params_set(
            config,
            str_to_cstr("appkey")?.as_ptr(),
            str_to_cstr(cfg.appkey.as_str())?.as_ptr(),
        );
        finclip_params_set(
            config,
            str_to_cstr("secret")?.as_ptr(),
            str_to_cstr(cfg.secret.as_str())?.as_ptr(),
        );
        finclip_params_set(
            config,
            str_to_cstr("domain")?.as_ptr(),
            str_to_cstr(cfg.domain.as_str())?.as_ptr(),
        );
        finclip_params_set(
            config,
            str_to_cstr("app_id")?.as_ptr(),
            str_to_cstr(cfg.appid.as_str())?.as_ptr(),
        );
        if !cfg.exe_path.is_empty() {
            finclip_params_set(
                config,
                str_to_cstr("exe_path")?.as_ptr(),
                str_to_cstr(cfg.exe_path.as_str())?.as_ptr(),
            );
        }
        finclip_params_set(
            config,
            str_to_cstr("show_loading")?.as_ptr(),
            str_to_cstr(cfg.show_loading.as_str())?.as_ptr(),
        );
        println!("set params finished.");
        let res = finclip_config_packer_add_config(packer, config);
        println!("add config. result: {}", res);
        let res = finclip_start_applet(
            str_to_cstr(cfg.appstore.as_str())?.as_ptr(),
            str_to_cstr(cfg.appid.as_str())?.as_ptr(),
        );
        println!("start applet. result: {}", res);
        Ok(())
    }
}

fn main() -> Result<()> {
    let appid = "6152b5dbfcfb4e0001448e6e";
    start_normal(&AppParams {
        appstore: "1".to_string(),
        appkey: "22LyZEib0gLTQdU3MUauAfJ/xujwNfM6OvvEqQyH4igA".to_string(),
        secret: "703b9026be3d6bc5".to_string(),
        domain: "https://finclip-testing.finogeeks.club".to_string(),
        appid: appid.to_string(),
        exe_path: "/Users/sylar/Projects/finclipsdk-desktop/build/core/Debug/FinClip.app"
            .to_string(),
        show_loading: "0".to_string(),
        params: HashMap::new(),
    })?;
    loop {
        thread::sleep(Duration::from_secs(1));
        unsafe {
            if !finclip_is_applet_open(str_to_cstr(appid)?.as_ptr()) {
                break;
            }
        }
    }
    Ok(())
}
