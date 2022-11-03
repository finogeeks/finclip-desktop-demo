#include "./finclip_api.h"
#include "./finclip_api_const.h"
#include <utility>
#include <vector>
#include <map>
#include <napi.h>

using namespace std;
using namespace Napi;
namespace NodeFinClip {

std::vector<IPackerFactory*> factory_vector;
std::vector<IFinConfigPacker*> packer_vector;
std::vector<FinclipParams*> config_vector;
std::map<int, Napi::ThreadSafeFunction> lifecycle_callback;
std::map<int, Napi::ObjectReference> lifecycle_value;

Napi::Object FinclipGetPackerFactory(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  auto *factory = finclip_get_packer_factory();
  factory_vector.push_back(factory);
  auto result = Napi::Object::New(env);
  result.Set("id", factory_vector.size() - 1);
  return result;
}

Napi::Object FinclipPackerFactoryGetConfigPacker(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object factory_object = info[0].ToObject();
  int factory_id = factory_object.Get("id").ToNumber();
  auto *factory = factory_vector.at(factory_id);
  auto *packer = finclip_packer_factory_get_config_packer(factory);
  packer_vector.push_back(packer);
  auto result = Napi::Object::New(env);
  result.Set("id", packer_vector.size() - 1);
  return result;
}

Napi::Number FinclipInitialize(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object packer_object = info[0].ToObject();
  int packer_id = packer_object.Get("id").ToNumber();
  auto *packer = packer_vector.at(packer_id);
  finclip_initialize(packer);
  auto result = Napi::Number::New(env, 0);
  return result;
}

Napi::Object FinclipCreateParams(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  auto *config = finclip_create_params();
  config_vector.push_back(config);
  auto result = Napi::Object::New(env);
  result.Set("id", config_vector.size() - 1);
  return result;
}

Napi::Number FinclipParamsSet(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object config_object = info[0].ToObject();
  Napi::String key = info[1].ToString();
  Napi::String value = info[2].ToString();
  int config_id = config_object.Get("id").ToNumber();
  auto *config = config_vector.at(config_id);
  finclip_params_set(config, key.Utf8Value().c_str(), value.Utf8Value().c_str());
  auto result = Napi::Number::New(env, 0);
  return result;
}

Napi::Number FinclipConfigPackerAddConfig(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object packer_object = info[0].ToObject();
  int packer_id = packer_object.Get("id").ToNumber();
  auto *packer = packer_vector.at(packer_id);
  Napi::Object config_object = info[1].ToObject();
  int config_id = config_object.Get("id").ToNumber();
  auto *config = config_vector.at(config_id);
  finclip_config_packer_add_config(packer, config);
  auto result = Napi::Number::New(env, 0);
  return result;
}

Napi::Number FinclipStartApplet(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::String app_store = info[0].ToString();
  Napi::String app_id = info[1].ToString();
  finclip_start_applet(app_store.Utf8Value().c_str(), app_id.Utf8Value().c_str());
  auto result = Napi::Number::New(env, 0);
  return result;
}

Napi::Number FinclipStartAppletEmbed(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::String app_store = info[0].ToString();
  Napi::String app_id = info[1].ToString();
  Napi::Object config_object = info[2].ToObject();
  int config_id = config_object.Get("id").ToNumber();
  auto *config = config_vector.at(config_id);
  Napi::Number hwnd_number = info[3].ToNumber();
  HWND hwnd = (HWND)hwnd_number.Int64Value();
  finclip_start_applet_embed(app_store.Utf8Value().c_str(), app_id.Utf8Value().c_str(), config, hwnd);
  auto style = GetWindowLongPtrA(hwnd, GWL_STYLE);
  if (!(style & WS_CLIPCHILDREN)) {
    SetWindowLongPtrA(hwnd, GWL_STYLE, style ^ WS_CLIPCHILDREN ^ WS_CLIPSIBLINGS);
  }
  auto result = Napi::Number::New(env, 0);
  return result;
}

Napi::Number FinclipEmbedApplet(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::String app_store = info[0].ToString();
  Napi::String app_id = info[1].ToString();
  Napi::Number hwnd_number = info[2].ToNumber();
  HWND hwnd = (HWND)hwnd_number.Int64Value();
  finclip_embed_applet(app_store.Utf8Value().c_str(), app_id.Utf8Value().c_str(), hwnd);
  auto style = GetWindowLongPtrA(hwnd, GWL_STYLE);
  if (!(style & WS_CLIPCHILDREN)) {
    SetWindowLongPtrA(hwnd, GWL_STYLE, style ^ WS_CLIPCHILDREN ^ WS_CLIPSIBLINGS);
  }
  auto result = Napi::Number::New(env, 0);
  return result;
}

Napi::Number FinclipSetPosition(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::String app_id = info[0].ToString();
  Napi::Number left = info[1].ToNumber();
  Napi::Number top = info[2].ToNumber();
  Napi::Number width = info[3].ToNumber();
  Napi::Number height = info[4].ToNumber();
  finclip_set_position(app_id.Utf8Value().c_str(), left.Int64Value(), top.Int64Value(), width.Int64Value(), height.Int64Value());
  auto result = Napi::Number::New(env, 0);
  return result;
}

Napi::Number FinclipCloseApplet(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::String app_id = info[0].ToString();
  finclip_close_applet(app_id.Utf8Value().c_str());
  auto result = Napi::Number::New(env, 0);
  return result;
}

Napi::Number FinclipCloseAllApplet(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  finclip_close_all_applet();
  auto result = Napi::Number::New(env, 0);
  return result;
}

Napi::Number FinclipHideApplet(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::String app_id = info[0].ToString();
  finclip_hide_applet(app_id.Utf8Value().c_str());
  auto result = Napi::Number::New(env, 0);
  return result;
}

Napi::Number FinclipShowApplet(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::String app_id = info[0].ToString();
  finclip_show_applet(app_id.Utf8Value().c_str());
  auto result = Napi::Number::New(env, 0);
  return result;
}

void LifecicleHandle(LifecycleType type, const char* appid, void* input) {
  auto callback = [type](Napi::Env env,
                     Napi::Function jsCallback) {
    auto value_it = lifecycle_value.find(type);
    if (value_it != lifecycle_value.end()) {
      auto& value = value_it->second;
      jsCallback.Call({value.Value()});
    }
  };

  auto it = lifecycle_callback.find(type);
  if (it != lifecycle_callback.end()) {
    auto& thread_safe_Callback = it->second;
    thread_safe_Callback.NonBlockingCall(callback);
    thread_safe_Callback.Release();
  }
}

Napi::Number FinclipRegisterLifecycle(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::String app_id = info[0].ToString();
  Napi::Number liftcycle = info[1].ToNumber();
  Napi::Function api_function = info[2].As<Napi::Function>();
  Napi::Object value = info[3].ToObject();
  
  lifecycle_callback[liftcycle.Int32Value()] =
      Napi::ThreadSafeFunction::New(env, api_function, "Callback", 0, 1);
  lifecycle_value[liftcycle.Int32Value()] = Napi::Persistent(value);
      
  finclip_register_lifecycle(app_id.Utf8Value().c_str(),
    (LifecycleType)liftcycle.Int32Value(),
    LifecicleHandle,
    env
  );

  auto result = Napi::Number::New(env, 0);
  return result;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "finclip_get_packer_factory"), Napi::Function::New(env, FinclipGetPackerFactory));
  exports.Set(Napi::String::New(env, "finclip_packer_factory_get_config_packer"), Napi::Function::New(env, FinclipPackerFactoryGetConfigPacker));
  exports.Set(Napi::String::New(env, "finclip_initialize"), Napi::Function::New(env, FinclipInitialize));
  exports.Set(Napi::String::New(env, "finclip_create_params"), Napi::Function::New(env, FinclipCreateParams));
  exports.Set(Napi::String::New(env, "finclip_params_set"), Napi::Function::New(env, FinclipParamsSet));
  exports.Set(Napi::String::New(env, "finclip_config_packer_add_config"), Napi::Function::New(env, FinclipConfigPackerAddConfig));
  exports.Set(Napi::String::New(env, "finclip_start_applet"), Napi::Function::New(env, FinclipStartApplet));
  exports.Set(Napi::String::New(env, "finclip_start_applet_embed"), Napi::Function::New(env, FinclipStartAppletEmbed));
  exports.Set(Napi::String::New(env, "finclip_embed_applet"), Napi::Function::New(env, FinclipEmbedApplet));
  exports.Set(Napi::String::New(env, "finclip_set_position"), Napi::Function::New(env, FinclipSetPosition));
  exports.Set(Napi::String::New(env, "finclip_close_applet"), Napi::Function::New(env, FinclipCloseApplet));
  exports.Set(Napi::String::New(env, "finclip_close_all_applet"), Napi::Function::New(env, FinclipCloseAllApplet));
  exports.Set(Napi::String::New(env, "finclip_hide_applet"), Napi::Function::New(env, FinclipHideApplet));
  exports.Set(Napi::String::New(env, "finclip_show_applet"), Napi::Function::New(env, FinclipShowApplet));
  exports.Set(Napi::String::New(env, "finclip_register_lifecycle"), Napi::Function::New(env, FinclipRegisterLifecycle));
  
  return exports;
}

NODE_API_MODULE(addon, Init)
}  // namespace NodeFinClip
