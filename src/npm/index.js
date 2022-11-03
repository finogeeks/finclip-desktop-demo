const path = require('path');
const finclip = require('./build/Release/finclip.node');
const finclipPath = path.resolve(__dirname, '../../vendor/win/x64/finclip.exe');

const factory = finclip.finclip_get_packer_factory();
const packer = finclip.finclip_packer_factory_get_config_packer(factory);
finclip.finclip_initialize(packer);
const config = finclip.finclip_create_params();
finclip.finclip_params_set(config, "appstore", "1");
finclip.finclip_params_set(config, "appkey", "22LyZEib0gLTQdU3MUauAfJ/xujwNfM6OvvEqQyH4igA");
finclip.finclip_params_set(config, "secret", "703b9026be3d6bc5");
finclip.finclip_params_set(config, "domain", "https://finclip-testing.finogeeks.club");
finclip.finclip_params_set(config, "exe_path", finclipPath);
finclip.finclip_config_packer_add_config(packer, config);
finclip.finclip_params_set(config, "window_type", "1");
finclip.finclip_register_lifecycle("6152b5dbfcfb4e0001448e6e", 1, console.log, {a:1});
finclip.finclip_start_applet("1", "6152b5dbfcfb4e0001448e6e");

setTimeout(() => {}, 600 * 1000);