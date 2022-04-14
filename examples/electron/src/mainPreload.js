const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld(
  'finclip',
  {
    open: () => ipcRenderer.send('OPEN_FINCLIP_WINDOW'),
    close: () => ipcRenderer.send('CLOSE_FINCLIP_WINDOW'),
    setPosition: (payload) => ipcRenderer.send('SET_FINCLIP_POSITION', payload),
  }
)