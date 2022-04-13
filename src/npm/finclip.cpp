#include "../finclip_api.h"
#include "resource.h"
// Windows Header Files
#include <windows.h>
// #include <node_api.h>
#include <napi.h>
// C RunTime Header Files
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <winuser.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

#include "json.hpp"

#pragma comment(lib, "FinClipSDKWrapper.lib")

#define MAX_LOADSTRING 100
#define IDM_SENDMESSAGE_1 32784
#define IDM_SENDMESSAGE_2 32785
#define IDC_FINCLIP 109

using namespace std;
using json = nlohmann::json;

std::string Utf8Encode(const std::wstring& wstr, int cp = CP_UTF8) {
  if (wstr.empty()) return std::string();
  int size_needed =
      WideCharToMultiByte(cp, 0, &wstr[0], static_cast<int>(wstr.size()),
                          nullptr, 0, nullptr, nullptr);
  std::string str(size_needed, 0);
  WideCharToMultiByte(cp, 0, &wstr[0], static_cast<int>(wstr.size()), &str[0],
                      size_needed, nullptr, nullptr);
  return str;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring Utf8Decode(const std::string& str, int cp = CP_UTF8) {
  if (str.empty()) return std::wstring();
  int size_needed = MultiByteToWideChar(
      cp, 0, &str[0], static_cast<int>(str.size()), nullptr, 0);
  std::wstring wstr(size_needed, 0);
  MultiByteToWideChar(cp, 0, &str[0], static_cast<int>(str.size()), &wstr[0],
                      size_needed);
  return wstr;
}

void CustomApi(const char* event, const char* param,
               FinclipApiCallback callback) {
  std::string data = param;
  std::string e = event;
  std::string res = R"({"data":"ok"})";
  callback(res.c_str());
}

namespace NodeFinClip {
using namespace Napi;
std::wstring current_appid;

Napi::String start(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object args = info[0].ToObject();
  int handle = args.Get("handle").ToNumber().Int32Value();
  int appstore = 1;
  string domain("https://finchat-mop-b.finogeeks.club");
  string appkey("22LyZEib0gLTQdU3MUauAQVLIkNNhTSGIN42gXzlAsk=");
  string appid("60e3c059949a5300014d0c07");
  string secret("ae55433be2f62915");
  string type("1");
  std::string path = args.Get("finclipPath").ToString();
  auto* factory = finclip_get_packer_factory();
  auto* packer = finclip_packer_factory_get_config_packer(factory);
  auto* config = finclip_config_packer_new_config(packer);
  finclip_config_packer_add_config(packer, config);
  finclip_config_set_app_store(config, appstore);
  finclip_config_set_app_key(config, appkey.c_str());
  finclip_config_set_secret(config, secret.c_str());
  finclip_config_set_domain(config, domain.c_str());
  finclip_config_set_start_flag(config, kAppletSync);
  finclip_config_set_show_loading(config, false);
  finclip_config_set_exe_path(config, path.c_str());
  finclip_register_callback(packer, kApplet, "api", CustomApi);
  finclip_register_callback(packer, kWebView, "webapi", CustomApi);
  finclip_initialize(packer);
  finclip_start_applet(appstore, appid.c_str());

  //   hWnd_container = GetForegroundWindow();
  //   SetWindowPos(hWnd_container, HWND_TOP, 0, 0, 1024, 768,
  //   SWP_ASYNCWINDOWPOS);
  //   // hWnd_container = (HWND) handle;
  //   // hWnd_container = GetFocus();
  //   HRESULT hr = StartApplet(
  //       hWnd_container, appstore, Utf8Encode(wappid).c_str(), "", packer,
  //       args.Get("finclipPath").ToString().Utf8Value().c_str(),
  //       FinclipAppletCallback);
  current_appid = Utf8Decode(appid);
  //   SetAppletPos(Utf8Encode(wappid).c_str(), 0, 30, 540, 960, true);
  //   packer->Release();
  return Napi::String::New(env, path);
}

Napi::String close(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  finclip_close_all_applet();
  return Napi::String::New(env, "success");
}

Napi::String setAppletPos(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object args = info[0].ToObject();
  int left = args.Get("left").ToNumber().Int32Value();
  int top = args.Get("top").ToNumber().Int32Value();
  int width = args.Get("width").ToNumber().Int32Value();
  int height = args.Get("height").ToNumber().Int32Value();
  //   SetAppletPos(Utf8Encode(current_appid).c_str(), left, top, width, height,
  //                true);
  return Napi::String::New(env, "success");
}

Napi::String createWindow(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object args = info[0].ToObject();
  CreateWindowW(L"child_finclip", L"test", WS_VISIBLE, 0, 0, 1024, 768, nullptr,
                nullptr, nullptr, nullptr);
  return Napi::String::New(env, "success");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "start"), Napi::Function::New(env, start));
  exports.Set(Napi::String::New(env, "close"), Napi::Function::New(env, close));
  exports.Set(Napi::String::New(env, "setAppletPos"),
              Napi::Function::New(env, setAppletPos));
  exports.Set(Napi::String::New(env, "createWindow"),
              Napi::Function::New(env, createWindow));
  return exports;
}

NODE_API_MODULE(addon, Init)
}  // namespace NodeFinClip