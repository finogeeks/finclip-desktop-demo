const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld(
  'finclip',
  {
    open: () => ipcRenderer.send('OPEN_FINCLIP_WINDOW'),
  }
)