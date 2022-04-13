const { app, BrowserWindow, ipcMain } = require('electron');
const os = require('os');
const path = require('path');
const finclip = require('finclip');

let finClipWindow;

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
  const win = new BrowserWindow({
    width: 540,
    height: 990,
    autoHideMenuBar: true,
    show: false,
    webPreferences: {
      preload: path.join(__dirname, 'finclipPreload.js'),
    },
  });
  win.loadFile('../view/finclip.html');
  win.on('resize', () => {
    const bounds = win.getBounds();
    const left = 0;
    const top = 30;
    const width = bounds.width - 30;
    const height = bounds.height - 60;
    finclip.setAppletPos({
      left, top, width, height,
    });
  });
  finClipWindow = win;
  const handleBuffer = finClipWindow.getNativeWindowHandle();
  const handle = os.endianness() == 'LE' ? handleBuffer.readInt32LE() : handleBuffer.readInt32BE();
  const finclipPath = path.resolve(__dirname, '../../../vendor/win/x64/finclip.exe');
  const result = finclip.start({
    handle,
    finclipPath,
  });
  console.log(result);
};

const closeFinClipWindow = () => {
  const result = finclip.close();
  finClipWindow && finClipWindow.destroy();
  finClipWindow = undefined;
  console.log(result);
};

ipcMain.on('OPEN_FINCLIP_WINDOW', (event, arg) => {
  if (finClipWindow) return;
  openFinClipWindow();
});

ipcMain.on('CLOSE_FINCLIP_WINDOW', (event, arg) => {
  closeFinClipWindow();
});

app.whenReady().then(() => {
  createMainWindow();
});