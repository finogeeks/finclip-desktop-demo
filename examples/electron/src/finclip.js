const koffi = require('koffi');
const os = require('os');

const finclip = {};
let lib;

const loadLibrary = libraryPath => {
  lib = koffi.load(libraryPath);
  koffi.pointer('POINT', koffi.opaque());
  const lifecycleCallback = koffi.callback('lifecycleCallback', 'void', ['int', 'string', 'void*']);
  const customApiCallback = koffi.callback('customApiCallback', 'void', ['string', 'string', 'string', 'int']);

  finclip.finclip_create_params = lib.func('finclip_create_params', 'POINT', []);
  finclip.finclip_destory_params = lib.func('finclip_destory_params', 'int', ['POINT']);
  finclip.finclip_params_set = lib.func('finclip_params_set', 'int', ['POINT', 'string', 'string']);
  finclip.finclip_init_with_config = lib.func('finclip_init_with_config', 'int', ['string', 'POINT']);
  finclip.finclip_start_applet = lib.func('finclip_start_applet', 'int', ['string', 'string']);
  if (os.platform() === 'win32') {
    finclip.finclip_start_applet_embed = lib.func('finclip_start_applet_embed', 'int', ['string', 'string', 'POINT', 'int']);
  }
  finclip.finclip_close_applet = lib.func('finclip_close_applet', 'int', ['string']);
  finclip.finclip_set_position = lib.func('finclip_set_position', 'int', ['string', 'int', 'int', 'int', 'int']);
  finclip.finclip_callback_success = lib.func('finclip_callback_success', 'int', ['int', 'POINT']);
  finclip.finclip_callback_failure = lib.func('finclip_callback_failure', 'int', ['int', 'POINT']);

  finclip.finclip_register_lifecycle = (appid, type, fn, data) => {
    const callback = koffi.register(fn, koffi.pointer(lifecycleCallback));
    const finclip_register_lifecycle = lib.func('finclip_register_lifecycle', 'int', ['string', 'int', koffi.pointer(lifecycleCallback), 'void*']);
    return finclip_register_lifecycle(appid, type, callback, data);
  };

  finclip.finclip_register_api = (type, name, fn, data) => {
    const callback = koffi.register(fn, koffi.pointer(customApiCallback));
    const finclip_register_api = lib.func('finclip_register_api', 'int', ['int', 'string', koffi.pointer(customApiCallback), 'POINT']);
    return finclip_register_api(type, name, callback, data);
  };
};

finclip.load_library = loadLibrary;

module.exports = finclip;
