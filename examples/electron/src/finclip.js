const koffi = require("koffi");
const os = require("os");

const finclip = {};
const _lifecycleCallbackCache = new Map();
const proxyCallbackCache = new Map();

let lib;

const loadLibrary = (libraryPath) => {
  lib = koffi.load(libraryPath);
  koffi.pointer("POINT", koffi.opaque());
  const lifecycleCallback = koffi.proto("lifecycleCallback", "void", [
    "int",
    "string",
    "void*",
  ]);
  const customApiCallback = koffi.proto("customApiCallback", "void", [
    "string",
    "string",
    "string",
    "int",
  ]);
  const proxyCallback = koffi.proto("proxyCallback", "void", [
    "string",
    "string",
    "void*",
    "int",
    "string",
  ]);
  const invokeApiCallback = koffi.proto("invokeApiCallback", "void", [
    "string",
    "void*",
  ]);
  const customApiCallbackV2 = koffi.proto("customApiCallbackV2", "void", [
    "string",
    "string",
    "string",
    "int",
    "string",
  ]);

  finclip.finclip_create_params = lib.func(
    "finclip_create_params",
    "POINT",
    []
  );
  finclip.finclip_destory_params = lib.func("finclip_destory_params", "int", [
    "POINT",
  ]);
  finclip.finclip_params_set = lib.func("finclip_params_set", "int", [
    "POINT",
    "string",
    "string",
  ]);
  finclip.finclip_params_set_object = lib.func(
    "finclip_params_set_object",
    "int",
    ["POINT", "string", "string"]
  );
  finclip.finclip_init_with_config = lib.func(
    "finclip_init_with_config",
    "int",
    ["string", "POINT"]
  );
  finclip.finclip_start_applet = lib.func("finclip_start_applet", "int", [
    "string",
    "string",
  ]);
  finclip.finclip_close_applet = lib.func("finclip_close_applet", "int", [
    "string",
  ]);
  finclip.finclip_close_all_applet = lib.func(
    "finclip_close_all_applet",
    "int",
    []
  );
  finclip.finclip_set_position = lib.func("finclip_set_position", "int", [
    "string",
    "int",
    "int",
    "int",
    "int",
  ]);
  finclip.finclip_show_applet = lib.func("finclip_show_applet", "int", [
    "string",
  ]);
  finclip.finclip_callback_success = lib.func(
    "finclip_callback_success",
    "int",
    ["int", "POINT"]
  );
  finclip.finclip_callback_failure = lib.func(
    "finclip_callback_failure",
    "int",
    ["int", "POINT"]
  );
  finclip.finclip_start_applet_qrcode = lib.func(
    "finclip_start_applet_qrcode",
    "int",
    ["string", "string", "string"]
  );
  finclip.finclip_params_set_int = lib.func("finclip_params_set_int", "int", [
    "POINT",
    "string",
    "int",
  ]);

  if (os.platform() === "win32") {
    finclip.finclip_start_applet_embed = lib.func(
      "finclip_start_applet_embed",
      "int",
      ["string", "string", "POINT", "int"]
    );
  }

  finclip.finclip_register_lifecycle = (appid, type, fn, data) => {
    if (!_lifecycleCallbackCache.has(appid)) {
      _lifecycleCallbackCache.set(appid, new Map());
    }
    const appCallbacks = _lifecycleCallbackCache.get(appid);
    if (appCallbacks.has(type)) {
      const oldCallbackPointer = appCallbacks.get(type);
      console.log(
        `Replacing existing lifecycle callback for appid=${appid}, type=${type}.`
      );
      try {
        koffi.unregister(oldCallbackPointer);
      } catch (e) {
        console.error("Koffi: Failed to unregister previous callback.", e);
      }
    }

    const newCallbackPointer = koffi.register(
      fn,
      koffi.pointer(lifecycleCallback)
    );

    appCallbacks.set(type, newCallbackPointer);

    const finclip_register_lifecycle_c = lib.func(
      "finclip_register_lifecycle",
      "int",
      ["string", "int", koffi.pointer(lifecycleCallback), "void*"]
    );
    return finclip_register_lifecycle_c(appid, type, newCallbackPointer, data);
  };
  finclip.finclip_unregister_lifecycle = (appid) => {
    const appCallbacks = _lifecycleCallbackCache.get(appid);
    if (appCallbacks) {
      console.log(
        `Unregistering all lifecycle callbacks for appid=${appid}...`
      );
      for (const [type, callbackPointer] of appCallbacks.entries()) {
        try {
          koffi.unregister(callbackPointer);
        } catch (e) {
          console.error(
            `Koffi: Failed to unregister callback for appid=${appid}, type=${type}.`,
            e
          );
        }
      }
      _lifecycleCallbackCache.delete(appid);
    }
  };
  finclip.finclip_register_api = (type, name, fn, data) => {
    const callback = koffi.register(fn, koffi.pointer(customApiCallback));
    const finclip_register_api = lib.func("finclip_register_api", "int", [
      "int",
      "string",
      koffi.pointer(customApiCallback),
      "string",
    ]);
    return finclip_register_api(type, name, callback, data);
  };

  finclip.finclip_register_api_v2 = (type, name, fn, data) => {
    const callback = koffi.register(fn, koffi.pointer(customApiCallbackV2));
    const finclip_register_api_v2 = lib.func("finclip_register_api_v2", "int", [
      "int",
      "string",
      koffi.pointer(customApiCallbackV2),
      "string",
    ]);
    return finclip_register_api_v2(type, name, callback, data);
  };

  finclip.finclip_register_proxy = (name, fn, data) => {
    if (proxyCallbackCache.has(name)) {
      const oldCallback = proxyCallbackCache.get(name);
      koffi.unregister(oldCallback);
      console.warn(
        `finclip: Re-registering proxy for "${name}". The old one has been released.`
      );
    }

    const callback = koffi.register(fn, koffi.pointer(proxyCallback));
    proxyCallbackCache.set(name, callback);
    const finclip_register_proxy_native = lib.func(
      "finclip_register_proxy",
      "int",
      ["string", koffi.pointer(proxyCallback), "string"]
    );

    return finclip_register_proxy_native(name, callback, data);
  };

  finclip.finclip_invoke_api = (type, appid, api_name, params, fn, data) => {
    if (typeof fn !== "function") {
      const finclip_invoke_api_no_cb = lib.func("finclip_invoke_api", "int", [
        "int",
        "string",
        "string",
        "string",
        koffi.pointer(invokeApiCallback),
        "void*",
      ]);
      return finclip_invoke_api_no_cb(
        type,
        appid,
        api_name,
        params,
        null,
        data
      );
    }

    let callbackPointer;

    const wrapperCallback = (...args) => {
      try {
        koffi.unregister(callbackPointer);
      } catch (e) {
        console.error("Koffi: Failed to unregister callback.", e);
      }
      fn(...args);
    };

    callbackPointer = koffi.register(
      wrapperCallback,
      koffi.pointer(invokeApiCallback)
    );

    const finclip_invoke_api = lib.func("finclip_invoke_api", "int", [
      "int",
      "string",
      "string",
      "string",
      koffi.pointer(invokeApiCallback),
      "void*",
    ]);
    return finclip_invoke_api(
      type,
      appid,
      api_name,
      params,
      callbackPointer,
      data
    );
  };

  finclip.finclip_call_applet_event = (appid, event, params, fn, data) => {
    if (typeof fn !== "function") {
      const finclip_call_applet_event_no_cb = lib.func(
        "finclip_call_applet_event",
        "int",
        [
          "string",
          "string",
          "string",
          koffi.pointer(invokeApiCallback),
          "void*",
        ]
      );
      return finclip_call_applet_event_no_cb(appid, event, params, null, data);
    }

    let callbackPointer;

    const wrapperCallback = (...args) => {
      try {
        koffi.unregister(callbackPointer);
      } catch (e) {
        console.error("Koffi: Failed to unregister callback.", e);
      }
      fn(...args);
    };

    callbackPointer = koffi.register(
      wrapperCallback,
      koffi.pointer(invokeApiCallback)
    );

    const finclip_call_applet_event = lib.func(
      "finclip_call_applet_event",
      "int",
      ["string", "string", "string", koffi.pointer(invokeApiCallback), "void*"]
    );
    return finclip_call_applet_event(
      appid,
      event,
      params,
      callbackPointer,
      data
    );
  };
  finclip.finclip_start_applet_params = lib.func(
    "finclip_start_applet_params",
    "int",
    ["string", "string", "POINT"]
  );
  finclip.finclip_show_applet_v2 = lib.func("finclip_show_applet_v2", "int", [
    "string",
    "int",
  ]);
};

finclip.load_library = loadLibrary;

module.exports = finclip;
