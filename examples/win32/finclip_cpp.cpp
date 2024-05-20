
#include "finclip_api.h"
#include "finclip_api_const.h"
#include <iostream>

using namespace std;

int main() {
  const string appkey = "";
  const string secret = "";
  const string domain = "";
  const string appid = "";
  const string exe_path = "";
  const string app_store = "1";

  std::cout << "Hello FinClip!\n";

  FinclipParams *config = finclip_create_params();
  finclip_params_set(config, FINCLIP_CONFIG_APPKEY, appkey.c_str());
  finclip_params_set(config, FINCLIP_CONFIG_SECRET, secret.c_str());
  finclip_params_set(config, FINCLIP_CONFIG_DOMAIN, domain.c_str());
  finclip_params_set(config, FINCLIP_CONFIG_EXE_PATH, exe_path.c_str());

  finclip_init_with_config(app_store.c_str(), config);
  finclip_start_applet(app_store.c_str(), appid.c_str());

  std::cin.get();
  return 0;
}
