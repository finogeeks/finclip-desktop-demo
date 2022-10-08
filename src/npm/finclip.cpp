#include "./finclip_api.h"
#include "./finclip_api_const.h"
#include <vector>
#include <napi.h>

using namespace std;
using namespace Napi;
namespace NodeFinClip {

std::vector<IPackerFactory*> factory_vector;
std::vector<IFinConfigPacker*> packer_vector;
std::vector<FinclipParams*> config_vector;

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
  HWND hwnd = (HWND)hwnd_number.Int32Value();
  finclip_start_applet_embed(app_store.Utf8Value().c_str(), app_id.Utf8Value().c_str(), config, hwnd);
  auto result = Napi::Number::New(env, 0);
  return result;
}

Napi::ThreadSafeFunction threadSafeCallback;
bool callbackWasSet = false;

void LifecicleHandle(LifecycleType type, const char* appid, void* input) {
  auto env = (Napi::Env&)input;
  Napi::String napiMessageString = Napi::String::New(env, "Done");

  auto callback = [napiMessageString](Napi::Env env,
                                      Napi::Function jsCallback) {
    jsCallback.Call({napiMessageString});
  };

  threadSafeCallback.NonBlockingCall(callback);
  threadSafeCallback.Release();
}

Napi::Number FinclipRegisterLifecycle(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::String app_id = info[0].ToString();
  Napi::Number liftcycle = info[1].ToNumber();
  Napi::Function api_function = info[2].As<Napi::Function>();
  
  threadSafeCallback =
      Napi::ThreadSafeFunction::New(env, api_function, "Callback", 0, 1);

  finclip_register_lifecycle(app_id.Utf8Value().c_str(), (LifecycleType)liftcycle.Int32Value(), LifecicleHandle, env);

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
  exports.Set(Napi::String::New(env, "finclip_register_lifecycle"), Napi::Function::New(env, FinclipRegisterLifecycle));

  return exports;
}

NODE_API_MODULE(addon, Init)
}  // namespace NodeFinClip
