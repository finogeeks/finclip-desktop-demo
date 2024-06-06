const { app, BrowserWindow, ipcMain } = require('electron');
const os = require('os');
const path = require('path');
const finclip = require('./finclip');

let appid_ = "";
const appstore = "1";
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
} else if (os.platform() === 'linux') {
  finclipPath = path.resolve(__dirname, '../../../vendor/linux/arm64/FinClip');
  libraryPath = path.resolve(__dirname, '../../../vendor/linux/arm64', 'libFinClipSDKWrapper.so');
  process.env['LD_LIBRARY_PATH'] = path.resolve(__dirname, '../../../vendor/linux/arm64/lib');
}

finclip.load_library(libraryPath);

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
  finclip.finclip_params_set(params, "development_framework", "electron");
  finclip.finclip_params_set(params, "exe_path", finclipPath);
  finclip.finclip_init_with_config(appstore, params);
  config_ = params;

  finclip.finclip_register_lifecycle(appid, 2, () => {
    isOpen = false;
  }, "");

  const startSuccess = finclip.finclip_start_applet(appstore, appid);
  isOpen = (startSuccess === 0);
};

const embedFinClipWindow = (arg) => {
  if (os.platform() !== 'win32') return;
  if (isOpen) return;
  const { domain, appkey, appid, secret } = arg;
  appid_ = appid;
  const params = finclip.finclip_create_params();
  finclip.finclip_params_set(params, "preload_process_number", "0");
  finclip.finclip_params_set(params, "appkey", appkey);
  finclip.finclip_params_set(params, "secret", secret);
  finclip.finclip_params_set(params, "domain", domain);
  finclip.finclip_params_set(params, "development_framework", "electron");
  finclip.finclip_params_set(params, "exe_path", finclipPath);
  finclip.finclip_init_with_config(appstore, params);
  config_ = params;

  finclip.finclip_register_lifecycle(appid, 2, () => {
    isOpen = false;
  }, "");

  const handleBuffer = mainWindow.getNativeWindowHandle();
  hwnd = os.endianness() == 'LE' ? handleBuffer.readInt32LE() : handleBuffer.readInt32BE();
  const startSuccess = finclip.finclip_start_applet_embed(appstore, appid, params, hwnd);
  isOpen = (startSuccess === 0);
};

const closeFinClipWindow = () => {
  if (!isOpen) return;
  finclip.finclip_close_applet(appid_);
  isOpen = false;
};

ipcMain.on('EMBED_FINCLIP_WINDOW', (event, arg) => {
  embedFinClipWindow(arg);
});

ipcMain.on('OPEN_FINCLIP_WINDOW', (event, arg) => {
  openFinClipWindow(arg);
});

ipcMain.on('CLOSE_FINCLIP_WINDOW', (event, arg) => {
  closeFinClipWindow();
});

app.whenReady().then(() => {
  createMainWindow();
});