const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld(
  'finclip',
  {
    open: () => ipcRenderer.send('OPEN_FINCLIP_WINDOW'),
    close: () => ipcRenderer.send('CLOSE_FINCLIP_WINDOW'),
    setSize: (payload) => ipcRenderer.send('SET_FINCLIP_WINDOW_SIZE', payload),
  }
)