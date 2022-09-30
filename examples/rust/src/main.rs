extern crate anyhow;
extern crate serde;
extern crate serde_json;

mod finclip;

use anyhow::Result;
use finclip::*;
use finclip::FinClipApiType;
use fltk::dialog;
use fltk::enums::{Align, Event};
use fltk::window::Window;
use fltk::{app, button::Button, frame::Frame, prelude::*, text::*};
use serde_json::{json, Value};
use std::collections::HashMap;
use std::ffi::CStr;
use std::fs;
use std::os::raw::{c_char, c_void};
use std::sync::{Arc, Mutex};

extern "C" fn web_api_callback(res: *const c_char, input: *mut c_void) {
    unsafe {
        let appid = {
            let data = &*(input as *mut Arc<Mutex<WinElem>>);
            let mut d = data.lock().unwrap();
            d.btn_call.set_label("调用自定义api成功");
            d.appid.buffer().unwrap().text()
        };
        println!(
            ">> web_api_callback {} {}",
            CStr::from_ptr(res).to_str().unwrap(),
            appid,
        );
    }
}

extern "C" fn openapi_search(
    event: *const c_char,
    param: *const c_char,
    input: *mut c_void,
    _res: *mut c_void,
) {
    unsafe {
        let data = &*(input as *mut Arc<Mutex<WinElem>>);
        let appid = {
            let d = &mut data.lock().unwrap();
            d.appid.buffer().unwrap().text()
        };
        wrapper::batch_app_info(&appid, 
            "62f5c1e08f13e800017fa823,62f47da680b7ba0001de5701,62f47ce980b7ba0001de56fc,62f479dc80b7ba0001de56f8,62f625408f13e800017fa873",
            batch_app_info_callback, input).unwrap();
        println!(
            ">> openapi_search {} {} {}",
            CStr::from_ptr(event).to_str().unwrap(),
            CStr::from_ptr(param).to_str().unwrap(),
            appid,
        );
    }
}

extern "C" fn mock_login(
    event: *const c_char,
    param: *const c_char,
    input: *mut c_void,
    _res: *mut c_void,
) {
    unsafe {
        let data = &*(input as *mut Arc<Mutex<WinElem>>);
        println!(
            ">> app_api_example {} {} {}",
            CStr::from_ptr(event).to_str().unwrap(),
            CStr::from_ptr(param).to_str().unwrap(),
            data.lock().unwrap().appid.buffer().unwrap().text(),
        );
    }
}

extern "C" fn navigate_to_mini_program(
    event: *const c_char,
    param: *const c_char,
    input: *mut c_void,
    _res: *mut c_void,
) {
    unsafe {
        let data = &*(input as *mut Arc<Mutex<WinElem>>);
        println!(
            ">> app_api_example {} {} {}",
            CStr::from_ptr(event).to_str().unwrap(),
            CStr::from_ptr(param).to_str().unwrap(),
            data.lock().unwrap().appid.buffer().unwrap().text(),
        );
    }
}

extern "C" fn get_domain(
    event: *const c_char,
    param: *const c_char,
    input: *mut c_void,
    _res: *mut c_void,
) {
    unsafe {
        let data = &*(input as *mut Arc<Mutex<WinElem>>);
        println!(
            ">> app_api_example {} {} {}",
            CStr::from_ptr(event).to_str().unwrap(),
            CStr::from_ptr(param).to_str().unwrap(),
            data.lock().unwrap().appid.buffer().unwrap().text(),
        );
    }
}

extern "C" fn batch_app_info_callback(res: *const c_char, input: *mut c_void) {
    unsafe {
        let data = &*(input as *mut Arc<Mutex<WinElem>>);
        let appid = 
            data.lock().unwrap().appid.buffer().unwrap().text();
        
        println!(
            ">> batch_app_info_callback {}",
            CStr::from_ptr(res).to_str().unwrap()
        );
        let result = wrapper::create_params();
        wrapper::params_set(result, "data", CStr::from_ptr(res).to_str().unwrap()).unwrap();
        //wrapper::callback_res(&appid, 0, result).unwrap(); //TODO:
        wrapper::destory_params(result);
    }
}

extern "C" fn search_app_callback(res: *const c_char, _input: *mut c_void) {
    unsafe {
        println!(
            ">> search app {}",
            CStr::from_ptr(res).to_str().unwrap()
        );
    }
}

fn start_normal(win_elems: Arc<Mutex<WinElem>>, cfg: &finclip::AppParams) -> Result<()> {
    let factory = wrapper::get_packer_factory();
    let packer = wrapper::packer_factory_get_config_packer(factory);
    println!("get config packer.");
    let cfg_ptr = &win_elems as *const _ as *mut c_void;
    wrapper::register_api(
        packer,
        FinClipApiType::Applet,
        "openapi_search",
        openapi_search,
        cfg_ptr,
    )?;
    wrapper::register_api(
        packer,
        FinClipApiType::Applet,
        "mock_login",
        mock_login,
        cfg_ptr,
    )?;
    wrapper::register_api(
        packer,
        FinClipApiType::Applet,
        "navigate_to_mini_program",
        navigate_to_mini_program,
        cfg_ptr,
    )?;
    wrapper::register_api(
        packer,
        FinClipApiType::Applet,
        "get_domain",
        get_domain,
        cfg_ptr,
    )?;
    let res = wrapper::initialize(packer);
    println!("initialize packer. result: {}", res);
    let config = wrapper::create_params();
    println!("create params.");
    wrapper::applet_set_params(config, cfg)?;
    println!("set params finished.");
    let res = wrapper::config_packer_add_config(packer, config);
    println!("add config. result: {}", res);
    let res = wrapper::start_applet(cfg.appstore.as_str(), cfg.appid.as_str())?;
    println!("start applet. result: {}", res);
    Ok(())
}

fn close_all_applet() {
    wrapper::close_all_applet();
}

fn call_api(win_elems: Arc<Mutex<WinElem>>, appid: String) -> Result<()> {
    let cfg_ptr = &win_elems as *const _ as *mut c_void;
    let res = wrapper::invoke_api(
        FinClipApiType::WebView,
        appid.as_str(),
        "test",
        r###"{"hello":"world"}"###,
        web_api_callback,
        cfg_ptr,
    )?;
    println!("invoke api. result: {}", res);
    Ok(())
}

fn set_buffer(buf: Option<TextBuffer>, v: &Value) -> Option<()> {
    buf?.set_text(v.as_str()?);
    Some(())
}

fn set_buffer_str(buf: Option<TextBuffer>, txt: &str) -> Option<()> {
    buf?.set_text(txt);
    Some(())
}

fn center(w: i32, h: i32) -> (i32, i32) {
    let screens = app::Screen::all_screens();
    for s in screens {
        if s.is_valid() {
            return ((s.w() - w) / 2, (s.h() - h) / 2);
        }
    }
    (0, 0)
}

#[derive(Clone)]
struct WinElem {
    wind: Window,
    appid: TextEditor,
    appkey: TextEditor,
    secret: TextEditor,
    domain: TextEditor,
    exe: TextDisplay,
    param: TextEditor,
    btn_exe: Button,
    btn_start: Button,
    btn_close: Button,
    btn_call: Button,
    btn_batch: Button,
    btn_search: Button,
}

impl WinElem {
    fn new() -> Result<WinElem> {
        let wind = {
            const SIZE: (i32, i32) = (655, 490);
            let (x, y) = center(SIZE.0, SIZE.1);
            Window::new(x, y, SIZE.0, SIZE.1, "rustdemo")
        };
        let mut ret = WinElem {
            wind,
            appid: TextEditor::new(100, 40, 540, 25, ""),
            appkey: TextEditor::new(100, 80, 540, 25, ""),
            secret: TextEditor::new(100, 120, 540, 25, ""),
            domain: TextEditor::new(100, 160, 540, 25, ""),
            exe: TextDisplay::new(100, 200, 435, 25, ""),
            param: TextEditor::new(100, 240, 540, 170, ""),
            btn_exe: Button::new(540, 200, 100, 25, "设置app路径"),
            btn_start: Button::new(100, 420, 130, 25, "启动"),
            btn_close: Button::new(255, 420, 130, 25, "关闭所有小程序"),
            btn_call: Button::new(400, 420, 130, 25, "调用webview api"),
            btn_batch: Button::new(100, 455, 130, 25, "批量获取app"),
            btn_search: Button::new(255, 455, 130, 25, "搜索app"),
        };
        let align = Align::Left | Align::Inside;
        Frame::new(10, 42, 100, 20, "appid").with_align(align);
        Frame::new(10, 82, 100, 20, "appkey").with_align(align);
        Frame::new(10, 122, 100, 20, "secret").with_align(align);
        Frame::new(10, 162, 100, 20, "domain").with_align(align);
        Frame::new(10, 202, 100, 20, "可执行文件").with_align(align);
        Frame::new(10, 242, 100, 20, "参数").with_align(align);
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

        ret.btn_start.take_focus()?;

        let cb = move |t: &mut TextEditor, ev| match ev {
            Event::Leave => {
                t.buffer().unwrap().unselect();
                true
            }
            Event::Enter => {
                t.buffer().unwrap().select(0, t.buffer().unwrap().length());
                t.take_focus().unwrap();
                true
            }
            _ => false,
        };
        ret.appid.handle(cb);
        ret.appkey.handle(cb);
        ret.secret.handle(cb);
        ret.domain.handle(cb);
        ret.param.handle(move |t: &mut TextEditor, ev| match ev {
            Event::Leave => {
                t.buffer().unwrap().unselect();
                true
            }
            Event::Enter => {
                t.take_focus().unwrap();
                true
            }
            _ => false,
        });
        ret.exe.handle(move |t, ev| match ev {
            Event::Unfocus => {
                t.buffer().unwrap().unselect();
                true
            }
            _ => false,
        });

        ret.wind.end();
        ret.wind.show();
        Ok(ret)
    }

    fn init(this: Arc<Mutex<Self>>) -> Result<()> {
        let elems = &mut this.lock().unwrap();
        elems.btn_start.set_callback({
            let data = this.clone();
            move |_| {
                let (appid, appkey, secret, domain, exe, param) = {
                    let d = &mut data.lock().unwrap();
                    (
                        d.appid.buffer(),
                        d.appkey.buffer(),
                        d.secret.buffer(),
                        d.domain.buffer(),
                        d.exe.buffer(),
                        d.param.buffer(),
                    )
                };
                let text = param.unwrap().text();
                let mut params: Value =
                    serde_json::from_str(text.as_str()).unwrap_or_else(|_| json!({}));
                if !params.is_object() {
                    params = json!({});
                }
                let mut param_map = HashMap::new();
                for (k, v) in params.as_object().unwrap() {
                    if v.is_string() {
                        param_map.insert(k.clone(), v.as_str().unwrap().to_string());
                    }
                }
                start_normal(
                    data.clone(),
                    &finclip::AppParams {
                        appstore: String::from("1"),
                        appid: appid.unwrap().text(),
                        appkey: appkey.unwrap().text(),
                        secret: secret.unwrap().text(),
                        domain: domain.unwrap().text(),
                        exe_path: exe.unwrap().text(),
                        show_loading: "0".to_string(),
                        params: param_map,
                    },
                )
                .unwrap();
            }
        });

        elems.btn_close.set_callback(move |_| close_all_applet());

        elems.btn_call.set_callback({
            let data = this.clone();
            move |_| {
                let txt = {
                    let d = &mut data.lock().unwrap();
                    d.appid.buffer().unwrap().text()
                };
                call_api(data.clone(), txt).unwrap();
            }
        });

        elems.btn_exe.set_callback({
            let data = this.clone();
            move |_| {
                let (x, y, buffer) = {
                    let d = &mut data.lock().unwrap();
                    (d.wind.x(), d.wind.y(), d.exe.buffer())
                };
                let mut chooser = dialog::FileChooser::new(
                    ".",
                    "*",
                    dialog::FileChooserType::Single | dialog::FileChooserType::Directory,
                    "设置app路径",
                );
                chooser.show();
                chooser.window().set_pos(x + 75, y + 50);
                while chooser.shown() {
                    app::wait();
                }
                if chooser.value(1).is_none() {
                    return;
                }
                set_buffer_str(buffer, &chooser.value(1).unwrap());
            }
        });

        elems.btn_batch.set_callback({
            let data = this.clone();
            move |_| {
                let appid = {
                    let d = &mut data.lock().unwrap();
                    d.appid.buffer().unwrap().text()
                };
                let cfg_ptr = &data as *const _ as *mut c_void;
                let res = wrapper::batch_app_info(
                    appid.as_str(),
                    "62f5c1e08f13e800017fa823,62f47da680b7ba0001de5701,62f47ce980b7ba0001de56fc,62f479dc80b7ba0001de56f8,62f625408f13e800017fa873",
                    batch_app_info_callback,
                    cfg_ptr,
                )
                .unwrap();
                println!("batch app info. result: {}", res);
            }
        });

        elems.btn_search.set_callback({
            let data = this.clone();
            move |_| {
                let appid = {
                    let d = &mut data.lock().unwrap();
                    d.appid.buffer().unwrap().text()
                };
                let cfg_ptr = &data as *const _ as *mut c_void;
                let res = wrapper::search_app(
                    appid.as_str(),
                    "实例",
                    search_app_callback,
                    cfg_ptr,
                )
                .unwrap();
                println!("search app. result: {}", res);
            }
        });

        elems.load_config()?;
        Ok(())
    }

    fn load_config(&mut self) -> Result<()> {
        let content = fs::read_to_string("./config.json").unwrap_or_else(|_| "".to_string());
        if content.len() > 0 {
            let a: Value = serde_json::from_str(content.as_str()).unwrap_or_else(|_| json!({}));
            set_buffer(self.appid.buffer(), &a["app_id"]);
            set_buffer(self.appkey.buffer(), &a["appkey"]);
            set_buffer(self.secret.buffer(), &a["secret"]);
            set_buffer(self.domain.buffer(), &a["domain"]);
            set_buffer(self.exe.buffer(), &a["exe_path"]);
            set_buffer(self.param.buffer(), &a["params"]);
        }
        Ok(())
    }

    fn save_config(&self) -> Result<()> {
        let v = json!({
            "appstore": "1",
            "appkey": self.appkey.buffer().unwrap().text(),
            "secret": self.secret.buffer().unwrap().text(),
            "domain": self.domain.buffer().unwrap().text(),
            "app_id": self.appid.buffer().unwrap().text(),
            "exe_path": self.exe.buffer().unwrap().text(),
            "params": self.param.buffer().unwrap().text(),
        });
        let s = serde_json::to_string(&v)?;
        fs::write("./config.json", s)?;
        Ok(())
    }
}

fn main() -> Result<()> {
    let app = app::App::default();
    let elems_arc = Arc::new(Mutex::new(WinElem::new()?));
    WinElem::init(elems_arc.clone())?;
    app.run()?;
    elems_arc.lock().unwrap().save_config()?;
    Ok(())
}
