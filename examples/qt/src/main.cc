#include <QtGui/qscreen.h>

#include <QApplication>
#include <QDesktopServices>
#include <QThread>
#include <QUrl>
#include <iostream>
#include <string>

#include "finclip_api.h"
#include "finclip_api_const.h"

using namespace std;
#define CHECK_RESULT(func)                         \
  do {                                             \
    int result = func;                             \
    if (result == 0) {                             \
      std::cout << "Result is 0" << std::endl;     \
    } else {                                       \
      std::cout << "Result is not 0" << std::endl; \
    }                                              \
  } while (0)
#define ASSERT_RESULT(result)                                          \
  do {                                                                 \
    if (result != 0) std::cout << "Result is " << result << std::endl; \
    assert(result == 0);                                               \
  } while (0)
void fc_lifecycle_close(enum LifecycleType type, const char* appid,
                        void* input) {
  _Exit(0);
}
void fc_lifecycle_crash(enum LifecycleType type, const char* appid,
                        void* input) {
  _Exit(0);
}

void example_api(const char* event, const char* param, void* input,
                 int callbackid, const char* appid) {
  auto* params = finclip_create_params();
  // YOUR CODE HERE
  finclip_callback_success(callbackid, params);
  finclip_destory_params(params);
}

int main(int argc, char* argv[]) {
  const QApplication app(argc, argv);
  const string appkey = "";
  const string secret = "";
  const string domain = "";
  const string appid = "";
  const string exe_path = "";
  const string app_store = "1";

  FinclipParams* config = finclip_create_params();
  ASSERT_RESULT(
      finclip_params_set(config, FINCLIP_CONFIG_APPSTORE, app_store.c_str()));
  ASSERT_RESULT(
      finclip_params_set(config, FINCLIP_CONFIG_APPKEY, appkey.c_str()));
  ASSERT_RESULT(
      finclip_params_set(config, FINCLIP_CONFIG_SECRET, secret.c_str()));
  ASSERT_RESULT(
      finclip_params_set(config, FINCLIP_CONFIG_DOMAIN, domain.c_str()));
  ASSERT_RESULT(
      finclip_params_set(config, FINCLIP_CONFIG_EXE_PATH, exe_path.c_str()));
  ASSERT_RESULT(finclip_register_lifecycle(appid.c_str(), kLifecycleClosed,
                                           fc_lifecycle_close, nullptr));
  ASSERT_RESULT(finclip_register_lifecycle(appid.c_str(), kLifecycleCrashed,
                                           fc_lifecycle_crash, nullptr));
  finclip_register_api_v2(kApplet, "example_api", example_api, nullptr);
  finclip_register_api_v2(kWebView, "example_api1", example_api, nullptr);
  auto* params = finclip_create_params();
  ASSERT_RESULT(finclip_init_with_config(app_store.c_str(), config));

  ASSERT_RESULT(
      finclip_start_applet_params(app_store.c_str(), appid.data(), params));

  app.exec();
  return 0;
}
