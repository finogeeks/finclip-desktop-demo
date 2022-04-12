const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld(
  'finclip',
  {
    close: () => ipcRenderer.send('CLOSE_FINCLIP_WINDOW'),
  }
)