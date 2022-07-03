const { app, BrowserWindow, ipcMain } = require('electron');
const os = require('os');
const path = require('path');
const finclip = require('finclip');

let hwnd = 0;

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
};

const openFinClipWindow = (arg) => {
  const { domain, appkey, appid, secret } = arg;
  finclip.setDomain(domain);
  finclip.setAppkey(appkey);
  finclip.setAppid(appid);
  finclip.setSecret(secret);
  const finclipPath = path.resolve(__dirname, '../../../vendor/win/x64/finclip.exe');
  const result = finclip.start({
    handle: 0,
    finclipPath,
  });
};

const embedFinClipWindow = () => {
  finclip.embed({ handle: hwnd });
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