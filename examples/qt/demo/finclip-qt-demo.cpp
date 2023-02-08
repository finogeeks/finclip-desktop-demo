#include "finclip-qt-demo.h"
#include <qevent.h>

std::string appid = "";
FinclipQtDemo::FinclipQtDemo(QWidget *parent)
    : QMainWindow(parent)
{
    this->show();
    std::string appkey = "";
    std::string secret = "";
    std::string domain = "";

    std::string app_store = "1";

    FinclipParams *config = finclip_create_params();
    finclip_params_set(config, FINCLIP_CONFIG_APPSTORE, "1");
    finclip_params_set(config, FINCLIP_CONFIG_APPKEY, appkey.c_str());
    finclip_params_set(config, FINCLIP_CONFIG_SECRET, secret.c_str());
    finclip_params_set(config, FINCLIP_CONFIG_DOMAIN, domain.c_str());
    finclip_init_with_config(app_store, config);

    auto* params = finclip_create_params();
    finclip_params_set(params, "window_type", "1");
    finclip_start_applet_embed(app_store.c_str(), appid.c_str(),params, (HWND)this->effectiveWinId());
}

void FinclipQtDemo::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   finclip_set_position(appid.c_str(), 0, 0, event->size().height(),event->size().width() );

}

FinclipQtDemo::~FinclipQtDemo()
{
}

