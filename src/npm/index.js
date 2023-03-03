const ffi = require('ffi-napi');
const ref = require('ref-napi');

const finclip = {};
let lib = {};

const configObj = ref.types.void;
const configObjPtr = ref.refType(configObj);

const loadLibrary = libraryPath => {
  lib = ffi.Library(libraryPath, {
    'finclip_create_params': [ configObjPtr, [] ],
    'finclip_params_set': ['int', [configObjPtr, 'string', 'string' ] ],
    'finclip_init_with_config': [ 'int', [ 'string', configObjPtr ] ],
    'finclip_start_applet': [ 'int', ['string', 'string' ] ],
    'finclip_start_applet_embed': [ 'int', ['string', 'string', configObjPtr, 'int' ] ],
    'finclip_close_applet': [ 'int', ['string' ] ],
    'finclip_set_position': [ 'int', ['string', 'int', 'int', 'int', 'int' ] ],
    'finclip_register_lifecycle': [ 'int', [ 'string', 'int', 'pointer', 'string' ] ],
  });

  finclip.finclip_create_params = lib.finclip_create_params;
  finclip.finclip_params_set = lib.finclip_params_set;
  finclip.finclip_init_with_config = lib.finclip_init_with_config;
  finclip.finclip_start_applet = lib.finclip_start_applet;
  finclip.finclip_start_applet_embed = lib.finclip_start_applet_embed;
  finclip.finclip_close_applet = lib.finclip_close_applet;
  finclip.finclip_set_position = lib.finclip_set_position;

  finclip.finclip_register_lifecycle = (appid, type, fn, data) => {
    const callback = ffi.Callback('void', [ 'int', 'string', 'string' ], fn);
    return lib.finclip_register_lifecycle(appid, type, callback, data);
  };
};

finclip.load_library = loadLibrary;

module.exports = finclip;
