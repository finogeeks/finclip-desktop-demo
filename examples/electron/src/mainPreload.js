const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld(
  'finclip',
  {
    open: (payload) => ipcRenderer.send('OPEN_FINCLIP_WINDOW', payload),
    close: () => ipcRenderer.send('CLOSE_FINCLIP_WINDOW'),
  }
)