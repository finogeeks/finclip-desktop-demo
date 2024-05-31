const koffi = require('koffi');
const os = require('os');

const finclip = {};
let lib;

const loadLibrary = libraryPath => {
  lib = koffi.load(libraryPath);
  koffi.pointer('POINT', koffi.opaque());
  const lifecycleCallback = koffi.proto('lifecycleCallback', 'void', ['int', 'string', 'void*']);
  const customApiCallback = koffi.proto('customApiCallback', 'void', ['string', 'string', 'string', 'int']);
  const proxyCallback = koffi.proto('proxyCallback', 'void', ['string', 'string', 'void*', 'int', 'string']);

  finclip.finclip_create_params = lib.func('finclip_create_params', 'POINT', []);
  finclip.finclip_destory_params = lib.func('finclip_destory_params', 'int', ['POINT']);
  finclip.finclip_params_set = lib.func('finclip_params_set', 'int', ['POINT', 'string', 'string']);
  finclip.finclip_init_with_config = lib.func('finclip_init_with_config', 'int', ['string', 'POINT']);
  finclip.finclip_start_applet = lib.func('finclip_start_applet', 'int', ['string', 'string']);
  finclip.finclip_close_applet = lib.func('finclip_close_applet', 'int', ['string']);
  finclip.finclip_set_position = lib.func('finclip_set_position', 'int', ['string', 'int', 'int', 'int', 'int']);
  finclip.finclip_callback_success = lib.func('finclip_callback_success', 'int', ['int', 'POINT']);
  finclip.finclip_callback_failure = lib.func('finclip_callback_failure', 'int', ['int', 'POINT']);

  if (os.platform() === 'win32') {
    finclip.finclip_start_applet_embed = lib.func('finclip_start_applet_embed', 'int', ['string', 'string', 'POINT', 'int']);
  }

  finclip.finclip_register_lifecycle = (appid, type, fn, data) => {
    const callback = koffi.register(fn, koffi.pointer(lifecycleCallback));
    const finclip_register_lifecycle = lib.func('finclip_register_lifecycle', 'int', ['string', 'int', koffi.pointer(lifecycleCallback), 'void*']);
    return finclip_register_lifecycle(appid, type, callback, data);
  };

  finclip.finclip_register_api = (type, name, fn, data) => {
    const callback = koffi.register(fn, koffi.pointer(customApiCallback));
    const finclip_register_api = lib.func('finclip_register_api', 'int', ['int', 'string', koffi.pointer(customApiCallback), 'string']);
    return finclip_register_api(type, name, callback, data);
  };

  finclip.finclip_register_proxy = (name, fn, data) => {
    const callback = koffi.register(fn, koffi.pointer(proxyCallback));
    const finclip_register_proxy = lib.func('finclip_register_proxy', 'int', ['string', koffi.pointer(proxyCallback), 'string']);
    return finclip_register_proxy(name, callback, data);
  };
};

finclip.load_library = loadLibrary;

module.exports = finclip;
