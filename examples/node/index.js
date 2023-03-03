const finclip = require('finclip');
const path = require('path');

const libraryPath = path.resolve(__dirname, '../../vendor/win/x64/FinClipSDKWrapper.dll');
const finclipPath = path.resolve(__dirname, '../../vendor/win/x64/finclip/finclip.exe');

const APP_STORE = '1';
const APP_KEY = 'Cd9iQDv+6YHGPuYz6kdXhOD1UI9UWvwhLfWaOzsyq0M=';
const APP_SECRET = 'b8d249d57482554c';
const DOMAIN = 'https://api.finclip.com';
const APP_ID = '63e30f1d3dc3390001ffbb84';

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