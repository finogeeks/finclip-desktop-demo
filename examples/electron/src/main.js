const { app, BrowserWindow, ipcMain } = require('electron');
const os = require('os');
const path = require('path');
const finclip = require('finclip');

const createMainWindow = () => {
  const win = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, 'mainPreload.js'),
    },
  });
  win.loadFile('../view/index.html');
};

const openFinClipWindow = () => {
  const finclipPath = path.resolve(__dirname, '../../../vendor/win/x64/finclip.exe');
  const result = finclip.start({
    handle: 0,
    finclipPath,
  });
  console.log(result);
};

const closeFinClipWindow = () => {
  const result = finclip.close();
  console.log(result);
};

ipcMain.on('OPEN_FINCLIP_WINDOW', (event, arg) => {
  openFinClipWindow();
});

ipcMain.on('CLOSE_FINCLIP_WINDOW', (event, arg) => {
  closeFinClipWindow();
});

ipcMain.on('SET_FINCLIP_WINDOW_SIZE', (event, arg) => {
  const { left, top, width, height } = arg;
  finclip.setAppletPos({
    left, top,
    width, height,
  });
});

app.whenReady().then(() => {
  createMainWindow();
});