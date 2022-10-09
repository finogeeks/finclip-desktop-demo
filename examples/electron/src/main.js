const { app, BrowserWindow, ipcMain } = require('electron');
const os = require('os');
const path = require('path');
const finclip = require('finclip');

let hwnd = 0;
let appid_ = "";
let config_;

const createMainWindow = () => {
  const win = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, 'mainPreload.js'),
    },
  });
  win.loadFile('../view/index.html');
  const handleBuffer = win.getNativeWindowHandle();
  hwnd = os.endianness() == 'LE' ? handleBuffer.readInt32LE() : handleBuffer.readInt32BE();
  console.log(hwnd);
};

const openFinClipWindow = (arg) => {
  const { domain, appkey, appid, secret } = arg;
  appid_ = appid;
  const factory = finclip.finclip_get_packer_factory();
  const packer = finclip.finclip_packer_factory_get_config_packer(factory);
  finclip.finclip_initialize(packer);
  const config = finclip.finclip_create_params();
  finclip.finclip_params_set(config, "appstore", "1");
  finclip.finclip_params_set(config, "appkey", appkey);
  finclip.finclip_params_set(config, "secret", secret);
  finclip.finclip_params_set(config, "domain", domain);
  const finclipPath = path.resolve(__dirname, '../../../vendor/win/x64/finclip.exe');
  finclip.finclip_params_set(config, "exe_path", finclipPath);
  finclip.finclip_config_packer_add_config(packer, config);
  finclip.finclip_params_set(config, "window_type", "1");
  config_ = config;
  finclip.finclip_start_applet("1", appid);
};

const embedFinClipWindow = () => {
  finclip.finclip_start_applet_embed("1", appid_, config_, hwnd);
};

const closeFinClipWindow = () => {
  const result = finclip.close();
  console.log(result);
};

ipcMain.on('OPEN_FINCLIP_WINDOW', (event, arg) => {
  openFinClipWindow(arg);
});

ipcMain.on('EMBED_FINCLIP_WINDOW', (event, arg) => {
  embedFinClipWindow(arg);
});

ipcMain.on('CLOSE_FINCLIP_WINDOW', (event, arg) => {
  closeFinClipWindow();
});

ipcMain.on('SET_FINCLIP_POSITION', (event, arg) => {
  const { left, top, width, height } = arg;
  const result = finclip.setAppletPos({
    left, top,
    width, height,
  });
  console.log(result);
});

app.whenReady().then(() => {
  createMainWindow();
});