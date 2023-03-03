const finclip = require('finclip');
const path = require('path');

const libraryPath = path.resolve(__dirname, '../../vendor/win/x64/FinClipSDKWrapper.dll');
const finclipPath = path.resolve(__dirname, '../../vendor/win/x64/finclip/finclip.exe');

const APP_STORE = '1';
const APP_KEY = '';
const APP_SECRET = '';
const DOMAIN = '';
const APP_ID = '';

finclip.load_library(libraryPath);
const config = finclip.finclip_create_params();
finclip.finclip_params_set(config, "appstore", APP_STORE);
finclip.finclip_params_set(config, "appkey", APP_KEY);
finclip.finclip_params_set(config, "secret", APP_SECRET);
finclip.finclip_params_set(config, "domain", DOMAIN);
finclip.finclip_params_set(config, "exe_path", finclipPath);

finclip.finclip_init_with_config(APP_STORE, config);

finclip.finclip_register_lifecycle(APP_ID, 2, console.log, "applet close");
finclip.finclip_register_lifecycle(APP_ID, 1, console.log, "applet start");

finclip.finclip_start_applet(APP_STORE, APP_ID);

setTimeout(() => {}, 5 * 60 * 1000);