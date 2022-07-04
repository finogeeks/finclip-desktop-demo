#include "../finclip_api.h"
#include "./finclip.h"
#include <windows.h>
#include <napi.h>
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

using namespace std;
using json = nlohmann::json;

namespace NodeFinClip {
using namespace Napi;
std::string domain;
std::string appkey;
std::string appid;
std::string secret;
char app_store[] = "1";

Napi::String start(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object args = info[0].ToObject();
  int handle = args.Get("handle").ToNumber().Int32Value();

  std::string path = args.Get("finclipPath").ToString();

  auto *factory = finclip_get_packer_factory();
  auto *packer = finclip_packer_factory_get_config_packer(factory);
  finclip_initialize(packer);
  FinclipParams *config;
  config = finclip_config_packer_get_config(packer, app_store);
  if (config == nullptr) config = finclip_create_params();

  finclip_params_set(config, FINCLIP_CONFIG_EXE_PATH, path.c_str());
  finclip_params_set(config, FINCLIP_CONFIG_APPSTORE, "1");
  finclip_params_set(config, FINCLIP_CONFIG_APPKEY, appkey.c_str());
  finclip_params_set(config, FINCLIP_CONFIG_SECRET, secret.c_str());
  finclip_params_set(config, FINCLIP_CONFIG_DOMAIN, domain.c_str());
  // 可选参数
  finclip_params_set(config, FINCLIP_CONFIG_SHOW_LOADING, "0");
  finclip_config_packer_add_config(packer, config);

  if (handle) {
    finclip_start_applet_embed(app_store, appid.c_str(), (HWND)handle);
  } else {
    finclip_start_applet(app_store, appid.c_str());
  }

  return Napi::String::New(env, "success");
}

Napi::String embed(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object args = info[0].ToObject();
  int handle = args.Get("handle").ToNumber().Int32Value();
  finclip_embed_applet(app_store, appid.c_str(), (HWND)handle);
  return Napi::String::New(env, "success");
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
  finclip_set_position(appid.c_str(), left, top, width, height);
  return Napi::String::New(env, "success");
}

Napi::String createWindow(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object args = info[0].ToObject();
  CreateWindowW(L"child_finclip", L"test", WS_VISIBLE, 0, 0, 1024, 768, nullptr,
                nullptr, nullptr, nullptr);
  return Napi::String::New(env, "success");
}

Napi::String setDomain(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  domain = info[0].ToString();
  return Napi::String::New(env, "success");
}

Napi::String setAppid(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  appid = info[0].ToString();
  return Napi::String::New(env, "success");
}

Napi::String setAppkey(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  appkey = info[0].ToString();
  return Napi::String::New(env, "success");
}

Napi::String setSecret(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  secret = info[0].ToString();
  return Napi::String::New(env, "success");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "start"), Napi::Function::New(env, start));
  exports.Set(Napi::String::New(env, "close"), Napi::Function::New(env, close));

  exports.Set(Napi::String::New(env, "setDomain"), Napi::Function::New(env, setDomain));
  exports.Set(Napi::String::New(env, "setAppid"), Napi::Function::New(env, setAppid));
  exports.Set(Napi::String::New(env, "setAppkey"), Napi::Function::New(env, setAppkey));
  exports.Set(Napi::String::New(env, "setSecret"), Napi::Function::New(env, setSecret));

  exports.Set(Napi::String::New(env, "setAppletPos"),
              Napi::Function::New(env, setAppletPos));
  exports.Set(Napi::String::New(env, "createWindow"),
              Napi::Function::New(env, createWindow));
  exports.Set(Napi::String::New(env, "embed"),
              Napi::Function::New(env, embed));
  return exports;
}

NODE_API_MODULE(addon, Init)
}  // namespace NodeFinClip