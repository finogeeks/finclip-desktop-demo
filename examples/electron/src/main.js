const { app, BrowserWindow, ipcMain } = require('electron');
const os = require('os');
const path = require('path');
const finclip = require('./finclip');

let appid_ = "";
let config_;
let mainWindow = null;
let isOpen = false;

let finclipPath;
let libraryPath;

if (os.platform() === 'win32') {
  finclipPath = path.resolve(__dirname, '../../../vendor/win/x64/FinClip/FinClip.exe');
  libraryPath = path.resolve(__dirname, '../../../vendor/win/x64', 'FinClipSDKWrapper.dll');
} else if (os.platform() === 'darwin') {
  finclipPath = path.resolve(__dirname, '../../../vendor/mac/x64/FinClip.app');
  libraryPath = path.resolve(__dirname, '../../../vendor/mac/x64', 'libFinClipSDKWrapper.so');
}

finclip.load_library(libraryPath);

finclip.finclip_register_api(0, 'custom_api_sample', (name, params, data, id) => {
  const message = finclip.finclip_create_params();
  finclip.finclip_params_set(message, 'foo', 'bar');
  finclip.finclip_callback_success(id, message);
}, "");

finclip.finclip_register_proxy('preview_media', (appid, name, res, callbackId, data) => {
  console.log(JSON.parse(data));
});

const createMainWindow = () => {
  mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    autoHideMenuBar: true,
    webPreferences: {
      preload: path.join(__dirname, 'mainPreload.js'),
    },
  });
  mainWindow.loadFile('../view/index.html');
};

const openFinClipWindow = (arg) => {
  if (isOpen) return;
  const { domain, appkey, appid, secret } = arg;
  appid_ = appid;
  const params = finclip.finclip_create_params();
  finclip.finclip_params_set(params, "preload_process_number", "0");
  finclip.finclip_params_set(params, "appkey", appkey);
  finclip.finclip_params_set(params, "secret", secret);
  finclip.finclip_params_set(params, "domain", domain);
  finclip.finclip_params_set(params, "exe_path", finclipPath);
  finclip.finclip_init_with_config("1", params);
  config_ = params;

  finclip.finclip_register_lifecycle(appid, 2, () => {
    isOpen = false;
  }, "");

  finclip.finclip_register_more_btn_handler(appid, "menu-id", "菜单A", "", 0, (appid, menuId) => {
    console.log("more btn clicked", appid, menuId);
  }, '');

  finclip.finclip_start_applet("1", appid);
  isOpen = true;
};

const closeFinClipWindow = () => {
  if (!isOpen) return;
  finclip.finclip_close_applet(appid_);
  isOpen = false;
};

ipcMain.on('OPEN_FINCLIP_WINDOW', (event, arg) => {
  openFinClipWindow(arg);
});

ipcMain.on('CLOSE_FINCLIP_WINDOW', (event, arg) => {
  closeFinClipWindow();
});

app.whenReady().then(() => {
  createMainWindow();
});