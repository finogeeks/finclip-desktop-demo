#include "finclip-qt-demo.h"
#include "finclip_api.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::string appkey = "";
    std::string secret = "";
    std::string domain = "";
    std::string app_store = "1";

    auto *factory = finclip_get_packer_factory();
    auto *packer = finclip_packer_factory_get_config_packer(factory);
    finclip_initialize(packer);
    FinclipParams *config = finclip_create_params();
    finclip_params_set(config, FINCLIP_CONFIG_APPSTORE, "1");
    finclip_params_set(config, FINCLIP_CONFIG_APPKEY, appkey.c_str());
    finclip_params_set(config, FINCLIP_CONFIG_SECRET, secret.c_str());
    finclip_params_set(config, FINCLIP_CONFIG_DOMAIN, domain.c_str());
    finclip_config_packer_add_config(packer, config);

    finclip_start_applet(app_store.c_str(), "");
    FinclipQtDemo w;
    w.show();
    return a.exec();
}
