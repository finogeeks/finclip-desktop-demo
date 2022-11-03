const { app, BrowserWindow, ipcMain } = require('electron');
const os = require('os');
const path = require('path');
const finclip = require('finclip');

let hwnd = 0;
let appid_ = "";
let config_;
let mainWindow = null;
let childWindow = null;
let embed = { x: 300, y: 0 };
let isOpen = false;
let isEmbed = false;

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

const createChildWindow = () => {
  childWindow = new BrowserWindow({ 
    parent: mainWindow,
    transparent: true,
    frame: false,
    autoHideMenuBar: true,
    webPreferences: {
      preload: path.join(__dirname, 'mainPreload.js'),
    },
  });
  const handleBuffer = childWindow.getNativeWindowHandle();
  hwnd = os.endianness() == 'LE' ? handleBuffer.readInt32LE() : handleBuffer.readInt32BE();
  console.log(hwnd);
};

const openFinClipWindow = (arg) => {
  if (isOpen) return;
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
  isOpen = true;
};

const embedFinClipWindow = () => {
  if (!isOpen) return;
  createChildWindow();
  finclip.finclip_start_applet_embed("1", appid_, config_, hwnd);
  resizeChildWindow();
  subscribe();
  isEmbed = true;
};

const closeFinClipWindow = () => {
  if (!isOpen) return;
  finclip.finclip_close_applet(appid_);
  isOpen = false;
  if (isEmbed) {
    unsubscribe();
  }
  isEmbed = false;
};

const onResize = () => {
  const bounds = childWindow.getContentBounds();
  finclip.finclip_set_position(appid_, bounds.x, bounds.y, bounds.width, bounds.height);
};

const subscribe = () => {
  mainWindow.on('move', resizeChildWindow);
  mainWindow.on('resize', resizeChildWindow);
  mainWindow.on('minimize', childWindow.minimize);
  mainWindow.on('restore', restoreChildWindow);

  childWindow.on('resize', onResize);
}

const unsubscribe = () => {
  mainWindow.removeListener('move', resizeChildWindow);
  mainWindow.removeListener('resize', resizeChildWindow);
  mainWindow.removeListener('minimize', childWindow.minimize);
  mainWindow.removeListener('restore', restoreChildWindow);

  childWindow.removeListener('resize', onResize);

  childWindow.close();
}

const resizeChildWindow = () => {
  const parentBounds = mainWindow.getContentBounds();
  childWindow.setBounds({
    x: parentBounds.x + embed.x,
    y: parentBounds.y + embed.y,
    width: parentBounds.width - embed.x,
    height: parentBounds.height - embed.y,
  });
};

const restoreChildWindow = () => {
  childWindow.restore();
  resizeChildWindow();
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
});

app.whenReady().then(() => {
  createMainWindow();
});