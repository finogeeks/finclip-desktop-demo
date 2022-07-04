#![allow(unused)]

extern crate serde;
extern crate serde_json;

use fltk::app::App;
use fltk::dialog;
use fltk::enums::Event;
use fltk::{
    app, button::Button, enums::Align, frame::Frame, prelude::*, text::*, window::Window,
};
use fltk_sys::dialog::*;
use serde_json::{json, Value};
use std::borrow::{Borrow, BorrowMut};
use std::cell::{Cell, RefCell, RefMut};
use std::ffi::{CStr, CString};
use std::fs;
use std::os::raw::{c_char, c_int, c_void};
use std::rc::Rc;
use std::sync::{Arc, Mutex};

#[repr(C)]
struct IPackerFactory;

#[repr(C)]
struct FinclipParams;

#[repr(C)]
struct IFinConfigPacker;

type Callback = extern "C" fn(
    event: *const c_char,
    param: *const c_char,
    input: *const c_void,
    res: *mut c_void,
);

type FinclipApiCallback = extern "C" fn(res: *const c_char, input: *const c_void);

#[allow(improper_ctypes)]
#[link(name = "FinClipSDKWrapper")]
extern "C" {
    fn finclip_get_packer_factory() -> *mut IPackerFactory;
    fn finclip_packer_factory_get_config_packer(
        factory: *mut IPackerFactory,
    ) -> *mut IFinConfigPacker;
    fn finclip_initialize(configpacker: *mut IFinConfigPacker) -> c_int;
    fn finclip_create_params() -> *mut FinclipParams;
    fn finclip_destory_params(params: *mut FinclipParams);
    fn finclip_params_set(params: *mut FinclipParams, key: *const c_char, value: *const c_char);
    fn finclip_params_del(params: *mut FinclipParams, key: *const c_char);
    fn finclip_config_packer_add_config(
        packer: *mut IFinConfigPacker,
        config: *mut FinclipParams,
    ) -> c_int;
    fn finclip_config_packer_new_config(packer: *mut IFinConfigPacker) -> *mut FinclipParams;
    fn finclip_config_packer_get_config(
        packer: *mut IFinConfigPacker,
        appstore: *const c_char,
    ) -> *mut FinclipParams;
    fn finclip_set_position(
        appid: *const c_char,
        left: c_int,
        top: c_int,
        width: c_int,
        height: c_int,
    );
    fn finclip_start_applet(appstore: *const c_char, appid: *const c_char) -> c_int;
    fn finclip_register_api(
        packer: *mut IFinConfigPacker,
        typ: c_int,
        apis: *const c_char,
        handle: Callback,
        input: *mut c_void,
    );
    fn finclip_invoke_api(
        typ: c_int,
        app_id: *const c_char,
        api_name: *const c_char,
        params: *const c_char,
        callback: FinclipApiCallback,
        input: *mut c_void,
    ) -> c_int;
    fn finclip_close_applet(appid: *const c_char) -> c_int;
    fn finclip_close_all_applet() -> c_int;
}

extern "C" fn web_api_callback(res: *const c_char, input: *const c_void) {
    unsafe {
        println!(
            ">> web_api_callback {} {}",
            CStr::from_ptr(res).to_str().unwrap(),
            input as c_int
        );
    }
}

extern "C" fn web_api_example(
    event: *const c_char,
    param: *const c_char,
    input: *const c_void,
    _res: *mut c_void,
) {
    unsafe {
        println!(
            ">> web_api_example {} {} {}",
            CStr::from_ptr(event).to_str().unwrap(),
            CStr::from_ptr(param).to_str().unwrap(),
            input as c_int
        );
    }
}

extern "C" fn app_api_example(
    event: *const c_char,
    param: *const c_char,
    input: *const c_void,
    _res: *mut c_void,
) {
    unsafe {
        println!(
            ">> app_api_example {} {} {}",
            CStr::from_ptr(event).to_str().unwrap(),
            CStr::from_ptr(param).to_str().unwrap(),
            input as c_int
        );
    }
}

fn string_to_cstr(k: &String) -> CString {
    let v = CString::new(k.as_bytes()).expect("CString::new failed");
    return v;
}

fn str_to_cstr(k: &str) -> CString {
    let v = CString::new(k.as_bytes()).expect("CString::new failed");
    return v;
}

struct AppConfig {
    appstore: String,
    appkey: String,
    secret: String,
    domain: String,
    appid: String,
    exe_path: String,
}

fn start_normal(cfg: &AppConfig) {
    unsafe {
        let factory = finclip_get_packer_factory();
        let packer = finclip_packer_factory_get_config_packer(factory);
        println!("get config packer.");
        let res1 = finclip_initialize(packer);
        println!("initialize packer. result: {}", res1);
        let config = finclip_create_params();
        println!("create params.");
        finclip_params_set(
            config,
            str_to_cstr("appstore").as_ptr(),
            string_to_cstr(&cfg.appstore).as_ptr(),
        );
        finclip_params_set(
            config,
            str_to_cstr("appkey").as_ptr(),
            string_to_cstr(&cfg.appkey).as_ptr(),
        );
        finclip_params_set(
            config,
            str_to_cstr("secret").as_ptr(),
            string_to_cstr(&cfg.secret).as_ptr(),
        );
        finclip_params_set(
            config,
            str_to_cstr("domain").as_ptr(),
            string_to_cstr(&cfg.domain).as_ptr(),
        );
        finclip_params_set(
            config,
            str_to_cstr("app_id").as_ptr(),
            string_to_cstr(&cfg.appid).as_ptr(),
        );
        finclip_params_set(
            config,
            str_to_cstr("exe_path").as_ptr(),
            string_to_cstr(&cfg.exe_path).as_ptr(),
        );
        println!("set params finished.");
        finclip_register_api(
            packer,
            1,
            str_to_cstr("test").as_ptr(),
            web_api_example,
            23 as *mut c_void,
        );
        finclip_register_api(
            packer,
            0,
            str_to_cstr("test").as_ptr(),
            app_api_example,
            32 as *mut c_void,
        );
        let res2 = finclip_config_packer_add_config(packer, config);
        println!("add config. result: {}", res2);
        let res3 = finclip_start_applet(
            string_to_cstr(&cfg.appstore).as_ptr(),
            string_to_cstr(&cfg.appid).as_ptr(),
        );
        println!("start applet. result: {}", res3);
    }
}

fn close_all_applet() {
    unsafe {
        finclip_close_all_applet();
    }
}

fn call_api(appid: String) {
    unsafe {
        let res4 = finclip_invoke_api(
            1,
            string_to_cstr(&appid).as_ptr(),
            str_to_cstr("test").as_ptr(),
            str_to_cstr(r#"{"hello":"world"}"#).as_ptr(),
            web_api_callback,
            33 as *mut c_void,
        );
        println!("invoke api. result: {}", res4);
    }
}

fn txt_buf(s: &str) -> TextBuffer {
    let mut buf = TextBuffer::default();
    buf.set_text(s);
    buf
}

fn set_text_display(e: &TextDisplay, txt: &str) {
    e.buffer().unwrap().set_text(txt);
}

#[derive(Clone)]
struct WinElem {
    appid: TextEditor,
    appkey: TextEditor,
    secret: TextEditor,
    domain: TextEditor,
    exe: TextDisplay,
    param: TextEditor,
    but_exe: Button,
    but_start: Button,
    but_close: Button,
    but_call: Button,
}

impl WinElem {
    fn new() -> WinElem {
        let mut wind = Window::new(0, 0, 655, 460, "rustdemo");
        Frame::new(10, 10, 100, 20, "配置").with_align(Align::Left | Align::Inside);
    
        Frame::new(10, 42, 100, 20, "appid").with_align(Align::Left | Align::Inside);
        Frame::new(10, 82, 100, 20, "appkey").with_align(Align::Left | Align::Inside);
        Frame::new(10, 122, 100, 20, "secret").with_align(Align::Left | Align::Inside);
        Frame::new(10, 162, 100, 20, "domain").with_align(Align::Left | Align::Inside);
        Frame::new(10, 202, 100, 20, "可执行文件").with_align(Align::Left | Align::Inside);
        Frame::new(10, 242, 100, 20, "参数").with_align(Align::Left | Align::Inside);
        let mut ret = WinElem {
            appid: TextEditor::new(110, 40, 540, 25, ""),
            appkey: TextEditor::new(110, 80, 540, 25, ""),
            secret: TextEditor::new(110, 120, 540, 25, ""),
            domain: TextEditor::new(110, 160, 540, 25, ""),
            exe: TextDisplay::new(110, 200, 440, 25, ""),
            param: TextEditor::new(110, 240, 540, 170, ""),
            but_exe: Button::new(550, 200, 100, 25, "设置app路径"),
            but_start: Button::new(110, 420, 130, 25, "启动"),
            but_close: Button::new(255, 420, 130, 25, "关闭所有小程序"),
            but_call: Button::new(400, 420, 130, 25, "调用webview api"),
        };
        ret.appid.set_scrollbar_align(Align::Clip);
        ret.appid.set_cursor_style(Cursor::Simple);
        ret.appkey.set_scrollbar_align(Align::Clip);
        ret.secret.set_scrollbar_align(Align::Clip);
        ret.domain.set_scrollbar_align(Align::Clip);
        ret.exe.set_scrollbar_align(Align::Clip);
        ret.param.wrap_mode(WrapMode::AtColumn, 0);

        ret.appid.set_buffer(TextBuffer::default());
        ret.appkey.set_buffer(TextBuffer::default());
        ret.secret.set_buffer(TextBuffer::default());
        ret.domain.set_buffer(TextBuffer::default());
        ret.exe.set_buffer(TextBuffer::default());
        ret.param.set_buffer(TextBuffer::default());

        ret.but_start.take_focus();

        wind.end();
        wind.show();
        ret
    }

    fn init(this: Arc<Mutex<Self>>) {
        let elems = &mut this.lock().unwrap();

        // elems.appid.handle({
        //     let data = this.clone();
        //     move |t, ev| match ev {
        //         Event::Unfocus => {
        //             let d = &data.lock().unwrap();
        //             d.save_config();
        //             true
        //         },
        //         _ => false,
        //     }
        // });
        elems.but_start.set_callback({
            let data = this.clone();
            move |_| {
                let d = &mut data.lock().unwrap();
                let cfg = AppConfig {
                    appstore: String::from("1"),
                    appkey: d.appkey.buffer().unwrap().text(),
                    secret: d.secret.buffer().unwrap().text(),
                    domain: d.domain.buffer().unwrap().text(),
                    appid: d.appid.buffer().unwrap().text(),
                    exe_path: d.exe.buffer().unwrap().text(),
                };
                start_normal(&cfg);
            }
        });
        elems.but_close.set_callback(move |_| close_all_applet());
        elems.but_call.set_callback({
            let data = this.clone();
            move |_| {
                let d = &mut data.lock().unwrap();
                call_api(d.appid.buffer().unwrap().text());
            }
        });

        elems.but_exe.set_callback({
            let data = this.clone();
            move |_| {
                let d = &mut data.lock().unwrap();
                let mut chooser = dialog::FileChooser::new(
                    ".",
                    "*",
                    dialog::FileChooserType::Single | dialog::FileChooserType::Directory,
                    "设置app路径",
                );
                chooser.show();
                chooser.window().set_pos(300, 300);
                while chooser.shown() {
                    app::wait();
                }
                if chooser.value(1).is_none() {
                    return;
                }
                set_text_display(&d.exe, &chooser.value(1).unwrap());
                d.save_config();
            }
        });

        elems.load_config();
    }

    fn load_config(&mut self) {
        let fd = fs::read_to_string("./config.json").unwrap_or_else(|_| "".to_string());
        if (fd.len() > 0) {
            let a: Value = serde_json::from_str(fd.as_str()).unwrap();
            self.appid.set_buffer(txt_buf(a["app_id"].as_str().unwrap()));
            self.appkey.set_buffer(txt_buf(a["appkey"].as_str().unwrap()));
            self.secret.set_buffer(txt_buf(a["secret"].as_str().unwrap()));
            self.domain.set_buffer(txt_buf(a["domain"].as_str().unwrap()));
            self.exe.set_buffer(txt_buf(a["exe_path"].as_str().unwrap()));
            self.param.set_buffer(txt_buf(a["params"].as_str().unwrap()));
        }
    }

    fn save_config(&self) {
        let v = json!({
            "appstore": "1",
            "appkey": self.appkey.buffer().unwrap().text(),
            "secret": self.secret.buffer().unwrap().text(),
            "domain": self.domain.buffer().unwrap().text(),
            "app_id": self.appid.buffer().unwrap().text(),
            "exe_path": self.exe.buffer().unwrap().text(),
            "params": self.param.buffer().unwrap().text(),
        });
        let s = serde_json::to_string(&v).unwrap();
        fs::write("./config.json", s);
    }
}

fn main() {
    let app = app::App::default();
    let elems_arc = Arc::new(Mutex::new(WinElem::new()));
    WinElem::init(elems_arc.clone());
    app.run().unwrap();
    elems_arc.lock().unwrap().save_config();
}
