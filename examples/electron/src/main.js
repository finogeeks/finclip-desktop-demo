const { app, BrowserWindow, ipcMain } = require("electron");
const os = require("os");
const path = require("path");
const finclip = require("@finogeeks2026/finclip-desktop");

let mainWindow = null;
const appstore = "1";

const sdkState = {
  isInitialized: false,
  lastConfig: null,
  paths: {
    exe: "",
    dll: "",
  },
};

function setupPaths() {
  const platform = os.platform();
  const vendorBase = path.resolve(__dirname, "../../../vendor");

  if (platform === "win32") {
    sdkState.paths.exe = path.join(
      vendorBase,
      "win",
      "x64",
      "FinClip",
      "FinClip.exe"
    );
    sdkState.paths.dll = path.join(
      vendorBase,
      "win",
      "x64",
      "FinClipSDKWrapper.dll"
    );
  } else if (platform === "darwin") {
    sdkState.paths.exe = path.join(
      vendorBase,
      "darwin",
      "arm64",
      "FinClip.app"
    );
    sdkState.paths.dll = path.join(
      vendorBase,
      "darwin",
      "arm64",
      "libFinClipSDKWrapper.so"
    );
  } else if (platform === "linux") {
    sdkState.paths.exe = path.join(vendorBase, "linux", "arm64", "FinClip");
    sdkState.paths.dll = path.join(
      vendorBase,
      "linux",
      "arm64",
      "libFinClipSDKWrapper.so"
    );
    process.env["LD_LIBRARY_PATH"] = path.join(
      vendorBase,
      "linux",
      "arm64",
      "lib"
    );
  }
}

setupPaths();
finclip.load_library(sdkState.paths.dll);

const createMainWindow = () => {
  mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    autoHideMenuBar: true,
    webPreferences: {
      preload: path.join(__dirname, "mainPreload.js"),
    },
  });
  mainWindow.loadFile("../view/index.html");
};

const createParams = (arg) => {
  const { domain, appkey, secret } = arg;
  const params = finclip.finclip_create_params();
  finclip.finclip_params_set(params, "preload_process_number", "0");
  finclip.finclip_params_set(params, "appkey", appkey);
  finclip.finclip_params_set(params, "secret", secret);
  finclip.finclip_params_set(params, "domain", domain);
  finclip.finclip_params_set(params, "development_framework", "electron");
  finclip.finclip_params_set(params, "exe_path", sdkState.paths.exe);
  return params;
};

const onSdkFirstInit = () => {
  console.log("FinClip SDK Initializing for the first time.");
  // 自定义 api
  finclip.finclip_register_api_v2(
    0,
    "test",
    async (name, params, data, id, appId) => {}
  );
  // 代理方法注册一次即可
  const proxyHandler = (appid, proxy_name, inputPtr, callbackId, params) => {};
  finclip.finclip_register_proxy("more_custom_menu", proxyHandler, "");
};

const initializeFinClipSDK = (arg) => {
  const { domain, appkey, secret } = arg;
  const params = createParams(arg);
  const newConfigSig = JSON.stringify({ domain, appkey, secret });
  const oldConfigSig = JSON.stringify(sdkState.lastConfig);

  if (!sdkState.isInitialized || newConfigSig !== oldConfigSig) {
    if (!sdkState.isInitialized) {
      onSdkFirstInit();
    }
    finclip.finclip_init_with_config(appstore, params);
    sdkState.isInitialized = true;
    sdkState.lastConfig = { domain, appkey, secret };
  }
  return params;
};

const openFinClipWindow = (arg) => {
  const { appid } = arg;

  initializeFinClipSDK(arg);

  finclip.finclip_register_lifecycle(appid, 2, () => {}, "");

  finclip.finclip_start_applet(appstore, appid);
};

const embedFinClipWindow = (arg) => {
  if (os.platform() !== "win32") return;
  if (!mainWindow) return;

  const { appid } = arg;

  const params = initializeFinClipSDK(arg);

  finclip.finclip_register_lifecycle(appid, 2, () => {}, "");

  const handleBuffer = mainWindow.getNativeWindowHandle();
  const hwnd =
    os.endianness() === "LE"
      ? handleBuffer.readInt32LE()
      : handleBuffer.readInt32BE();
  finclip.finclip_start_applet_embed(appstore, appid, params, hwnd);
};

const closeFinClipWindow = (arg) => {
  const { appid } = arg;
  finclip.finclip_close_applet(appid);
};

ipcMain.on("EMBED_FINCLIP_WINDOW", (event, arg) => {
  embedFinClipWindow(arg);
});

ipcMain.on("OPEN_FINCLIP_WINDOW", (event, arg) => {
  openFinClipWindow(arg);
});

ipcMain.on("CLOSE_FINCLIP_WINDOW", (event, arg) => {
  closeFinClipWindow(arg);
});

app.whenReady().then(() => {
  createMainWindow();
});
