const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld(
  'finclip',
  {
    open: (payload) => ipcRenderer.send('OPEN_FINCLIP_WINDOW', payload),
    embed: () => ipcRenderer.send('EMBED_FINCLIP_WINDOW'),
    close: () => ipcRenderer.send('CLOSE_FINCLIP_WINDOW'),
    setPosition: (payload) => ipcRenderer.send('SET_FINCLIP_POSITION', payload),
  }
)