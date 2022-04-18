#include "finclip-qt-demo.h"
#include "finclip_api.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::string appkey = "22LyZEib0gLTQdU3MUauAQVLIkNNhTSGIN42gXzlAsk=";
    std::string secret = "ae55433be2f62915";
    std::string domain = "https://finchat-mop-b.finogeeks.club:443";
    int app_store = 1;

    auto* factory = finclip_get_packer_factory();
    auto* packer = finclip_packer_factory_get_config_packer(factory);
    auto* config = finclip_config_packer_new_config(packer);
    finclip_config_packer_add_config(packer, config);
    finclip_config_set_app_store(config, app_store);
    finclip_config_set_app_key(config, appkey.c_str());
    finclip_config_set_secret(config, secret.c_str());
    finclip_config_set_domain(config, domain.c_str());
    finclip_config_set_start_flag(config, kAppletSync);
    finclip_config_set_show_loading(config, false);

    finclip_initialize(packer);
    finclip_start_applet(app_store, "60e3c059949a5300014d0c07");
    FinclipQtDemo w;
    w.show();
    return a.exec();
}
