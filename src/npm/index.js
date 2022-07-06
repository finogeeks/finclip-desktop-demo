const path = require('path');
const finclip = require('./build/Release/_finclip.node');
finclip.setDomain('https://finclip-testing.finogeeks.club');
finclip.setAppkey('22LyZEib0gLTQdU3MUauAfJ/xujwNfM6OvvEqQyH4igA');
finclip.setAppid('6152b5dbfcfb4e0001448e6e');
finclip.setSecret('703b9026be3d6bc5');
const finclipPath = path.resolve(__dirname, '../../vendor/win/x64/finclip.exe');
console.log('finclipPath', finclipPath);
const result = finclip.start({
  handle: 0,
  finclipPath,
});
setTimeout(() => {
}, 1000 * 60 * 60);