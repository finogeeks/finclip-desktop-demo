#include "../finclip_api.h"
#include "resource.h"
// Windows Header Files
#include <windows.h>
// #include <node_api.h>
#include <napi.h>
// C RunTime Header Files
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <winuser.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "json.hpp"

#pragma comment(lib, "FinClipSDKWrapper.lib")

#define MAX_LOADSTRING 100
#define IDM_SENDMESSAGE_1 32784
#define IDM_SENDMESSAGE_2 32785
#define IDC_FINCLIP 109

using namespace std;
using json = nlohmann::json;
using namespace Napi;

HINSTANCE hInst;
HWND gHwnd;                           // current instance
WCHAR szTitle[MAX_LOADSTRING];        // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];  // the main window class name

ATOM MyRegisterClass(HINSTANCE hInstance);
ATOM MyRegisterClass1(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND hWnd_appkey;
HWND hWnd_secret;
HWND hWnd_appid;
HWND hWnd_domain;
HWND hWnd_type;
HWND hWnd_container;
HWND hWnd_applet;
BOOL is_initialized = FALSE;
std::string offline_base_path;
std::string offline_applet_path;
std::wstring current_appid;

std::string Utf8Encode(const std::wstring& wstr, int cp = CP_UTF8) {
  if (wstr.empty()) return std::string();
  int size_needed =
      WideCharToMultiByte(cp, 0, &wstr[0], static_cast<int>(wstr.size()),
                          nullptr, 0, nullptr, nullptr);
  std::string str(size_needed, 0);
  WideCharToMultiByte(cp, 0, &wstr[0], static_cast<int>(wstr.size()), &str[0],
                      size_needed, nullptr, nullptr);
  return str;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring Utf8Decode(const std::string& str, int cp = CP_UTF8) {
  if (str.empty()) return std::wstring();
  int size_needed = MultiByteToWideChar(
      cp, 0, &str[0], static_cast<int>(str.size()), nullptr, 0);
  std::wstring wstr(size_needed, 0);
  MultiByteToWideChar(cp, 0, &str[0], static_cast<int>(str.size()), &wstr[0],
                      size_needed);
  return wstr;
}

void CustomApi(const char* event, const char* param,
               FinclipApiCallback callback) {
  std::string data = param;
  std::string e = event;
  std::string res = R"({"data":"ok"})";
  callback(res.c_str());
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  SetProcessDPIAware();
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_FINCLIP, szWindowClass, MAX_LOADSTRING);

  MyRegisterClass(hInstance);
  MyRegisterClass1(hInstance);
  if (!InitInstance(hInstance, nCmdShow)) {
    return FALSE;
  }

  HACCEL hAccelTable = LoadAccelerators(hInstance, nullptr);

  MSG msg;

  while (GetMessage(&msg, nullptr, 0, 0)) {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, nullptr);
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = nullptr;
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, nullptr);

  return RegisterClassExW(&wcex);
}

ATOM MyRegisterClass1(HINSTANCE hInstance) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, nullptr);
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = nullptr;
  wcex.lpszClassName = L"child_finclip";
  wcex.hIconSm = LoadIcon(wcex.hInstance, nullptr);

  return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  hInst = hInstance;  // Store instance handle in our global variable
  HWND hWnd =
      CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                    0, 1000, 1200, nullptr, nullptr, hInstance, nullptr);
  HMONITOR hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
  MONITORINFO mi = {sizeof(mi)};
  if (!GetMonitorInfo(hmon, &mi)) return FALSE;
  if (!hWnd) {
    return FALSE;
  }
  int width = 450;
  int height = 1000;
  SetWindowPos(hWnd, nullptr, mi.rcMonitor.left, mi.rcMonitor.top, width,
               height, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
  gHwnd = hWnd;
  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return TRUE;
}

void InitFinclipsdk(int app_store, const std::wstring& wappkey,
                    const std::wstring& wsecret, const std::wstring& wdomain,
                    const char* path) {
  if (is_initialized != 0) {
    return;
  }

  std::string appkey = Utf8Encode(wappkey);
  std::string secret = Utf8Encode(wsecret);
  std::string domain = Utf8Encode(wdomain);

  auto* factory = finclip_get_packer_factory();
  auto* packer = finclip_packer_factory_get_config_packer(factory);
  auto* config = finclip_config_packer_new_config(packer);
  finclip_config_packer_add_config(packer, config);
  finclip_config_set_app_store(config, app_store);
  finclip_config_set_app_key(config, appkey.c_str());
  finclip_config_set_secret(config, secret.c_str());
  finclip_config_set_domain(config, domain.c_str());
  finclip_config_set_start_flag(config, kAppletSync);
  finclip_config_set_show_loading(config, false);
  finclip_config_set_exe_path(config, path);
  finclip_register_callback(packer, kApplet, "api", CustomApi);
  finclip_register_callback(packer, kWebView, "webapi", CustomApi);

  finclip_initialize(packer);
  is_initialized = TRUE;
}

bool toggle = false;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  switch (message) {
    case WM_COMMAND:
      break;
    case WM_SHOWWINDOW:
      break;
    case WM_SIZE: {
      WCHAR type[1024];
      GetWindowText(hWnd_type, type, 1023);
      std::wstring wtype(type);
      break;
    }
    case WM_DESTROY: {
      if (hWnd == gHwnd) {
        PostQuitMessage(0);
      }
      break;
    }
    case WM_CREATE: {
      if (hWnd_appkey) return DefWindowProcW(hWnd, message, wParam, lParam);
      CreateWindowW(L"static", L"AppKEY", WS_CHILD | WS_VISIBLE, 20, 20, 60, 30,
                    hWnd, (HMENU)1, ((LPCREATESTRUCT)lParam)->hInstance,
                    nullptr);

      CreateWindowW(L"static", L"Secret", WS_CHILD | WS_VISIBLE, 20, 60, 60, 30,
                    hWnd, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance,
                    nullptr);

      CreateWindowW(L"static", L"appid", WS_CHILD | WS_VISIBLE, 20, 100, 60, 30,
                    hWnd, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance,
                    nullptr);
      CreateWindowW(L"static", L"domain", WS_CHILD | WS_VISIBLE, 20, 140, 60,
                    30, hWnd, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance,
                    nullptr);

      CreateWindowW(L"static", L"window_type", WS_CHILD | WS_VISIBLE, 20, 180,
                    60, 30, hWnd, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance,
                    nullptr);
      HMONITOR hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
      MONITORINFO mi = {sizeof(mi)};
      if (!GetMonitorInfo(hmon, &mi)) return 0;

      wstring domain(L"https://finchat-mop-b.finogeeks.club");
      wstring appkey(L"22LyZEib0gLTQdU3MUauAQVLIkNNhTSGIN42gXzlAsk=");
      wstring appid(L"60e3c059949a5300014d0c07");
      wstring secret(L"ae55433be2f62915");

      // exe, config.jsonexeͬһĿ¼
      // vs, config.jsonFinclip/
      auto path = std::filesystem::current_path();
      if (std::filesystem::exists("config.json")) {
        std::ifstream t("config.json");
        std::stringstream buffer;
        buffer << t.rdbuf();
        auto obj = json::parse(buffer.str());
        auto field = obj.find("domain");
        if (field != obj.end() && field.value().is_string()) {
          auto s = field.value().get<std::string>();
          domain = wstring(s.begin(), s.end());
        }
        field = obj.find("appkey");
        if (field != obj.end() && field.value().is_string()) {
          auto s = field.value().get<std::string>();
          appkey = wstring(s.begin(), s.end());
        }
        field = obj.find("appid");
        if (field != obj.end() && field.value().is_string()) {
          auto s = field.value().get<std::string>();
          appid = wstring(s.begin(), s.end());
        }
        field = obj.find("secret");
        if (field != obj.end() && field.value().is_string()) {
          auto s = field.value().get<std::string>();
          secret = wstring(s.begin(), s.end());
        }
        field = obj.find("offline_applet_path");
        if (field != obj.end() && field.value().is_string()) {
          auto s = field.value().get<std::string>();
          offline_applet_path = string(s.begin(), s.end());
        }
        field = obj.find("offline_base_path");
        if (field != obj.end() && field.value().is_string()) {
          auto s = field.value().get<std::string>();
          offline_base_path = string(s.begin(), s.end());
        }
      }
      hWnd_appkey = CreateWindowW(
          L"EDIT", appkey.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
          100, 20, 300, 30, hWnd, (HMENU)IDM_APPLET_APPKEY,
          ((LPCREATESTRUCT)lParam)->hInstance, nullptr);

      hWnd_secret = CreateWindowW(
          L"EDIT", secret.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
          100, 60, 300, 30, hWnd, (HMENU)IDM_APPLET_SECRET,
          ((LPCREATESTRUCT)lParam)->hInstance, nullptr);
      // 5ea0401463cb900001d73865 wxdemo
      // 5ea0412663cb900001d73867 webview
      // 5ea6d3bad262a7000141280d quote
      hWnd_appid = CreateWindowW(
          L"EDIT", appid.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
          100, 100, 300, 30, hWnd, (HMENU)IDM_APPLET_APPID,
          ((LPCREATESTRUCT)lParam)->hInstance, nullptr);
      hWnd_domain = CreateWindowW(
          L"EDIT", domain.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
          100, 140, 300, 30, hWnd, (HMENU)IDM_APPLET_DOMAIN,
          ((LPCREATESTRUCT)lParam)->hInstance, nullptr);

      hWnd_type = CreateWindowW(
          L"EDIT", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT, 100, 180,
          300, 30, hWnd, (HMENU)IDM_APPLET_DOMAIN,
          ((LPCREATESTRUCT)lParam)->hInstance, nullptr);
      int button_y = 240;
      CreateWindowW(L"BUTTON", L"open", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    100, button_y, 200, 50, hWnd, (HMENU)IDM_START_APPLET,
                    ((LPCREATESTRUCT)lParam)->hInstance, nullptr);
      button_y += 60;
      CreateWindowW(L"BUTTON", L"cleancache",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 100, button_y, 200,
                    50, hWnd, (HMENU)IDM_SENDMESSAGE,
                    ((LPCREATESTRUCT)lParam)->hInstance, nullptr);
      button_y += 60;
      CreateWindowW(L"BUTTON", L"bind_applet",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 100, button_y, 200,
                    50, hWnd, (HMENU)IDM_SENDMESSAGE_1,
                    ((LPCREATESTRUCT)lParam)->hInstance, nullptr);
      button_y += 60;
      CreateWindowW(L"BUTTON", L"unbind_applet",
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 100, button_y, 200,
                    50, hWnd, (HMENU)IDM_SENDMESSAGE_2,
                    ((LPCREATESTRUCT)lParam)->hInstance, nullptr);

    } break;
  }
  return DefWindowProcW(hWnd, message, wParam, lParam);
}

namespace NodeFinClip {

Napi::String start(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object args = info[0].ToObject();
  int handle = args.Get("handle").ToNumber().Int32Value();
  int appstore = 1;
  string domain("https://finchat-mop-b.finogeeks.club");
  string appkey("22LyZEib0gLTQdU3MUauAQVLIkNNhTSGIN42gXzlAsk=");
  string appid("60e3c059949a5300014d0c07");
  string secret("ae55433be2f62915");
  string type("1");
  auto path = args.Get("finclipPath").ToString().Utf8Value().c_str();

  auto* factory = finclip_get_packer_factory();
  auto* packer = finclip_packer_factory_get_config_packer(factory);
  auto* config = finclip_config_packer_new_config(packer);
  finclip_config_packer_add_config(packer, config);
  finclip_config_set_app_store(config, appstore);
  finclip_config_set_app_key(config, appkey.c_str());
  finclip_config_set_secret(config, secret.c_str());
  finclip_config_set_domain(config, domain.c_str());
  finclip_config_set_start_flag(config, kAppletSync);
  finclip_config_set_show_loading(config, false);
  finclip_config_set_exe_path(config, path);
  finclip_register_callback(packer, kApplet, "api", CustomApi);
  finclip_register_callback(packer, kWebView, "webapi", CustomApi);

  finclip_initialize(packer);
  finclip_start_applet(appstore, appid.c_str());

  //   hWnd_container = GetForegroundWindow();
  //   SetWindowPos(hWnd_container, HWND_TOP, 0, 0, 1024, 768,
  //   SWP_ASYNCWINDOWPOS);
  //   // hWnd_container = (HWND) handle;
  //   // hWnd_container = GetFocus();
  //   HRESULT hr = StartApplet(
  //       hWnd_container, appstore, Utf8Encode(wappid).c_str(), "", packer,
  //       args.Get("finclipPath").ToString().Utf8Value().c_str(),
  //       FinclipAppletCallback);
  current_appid = Utf8Decode(appid);
  //   SetAppletPos(Utf8Encode(wappid).c_str(), 0, 30, 540, 960, true);
  //   packer->Release();
  return Napi::String::New(
      env, args.Get("finclipPath").ToString().Utf8Value().c_str());
}

Napi::String close(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  finclip_close_all_applet();
  hWnd_container = nullptr;
  return Napi::String::New(env, "success");
}

Napi::String setAppletPos(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object args = info[0].ToObject();
  int left = args.Get("left").ToNumber().Int32Value();
  int top = args.Get("top").ToNumber().Int32Value();
  int width = args.Get("width").ToNumber().Int32Value();
  int height = args.Get("height").ToNumber().Int32Value();
  //   SetAppletPos(Utf8Encode(current_appid).c_str(), left, top, width, height,
  //                true);
  return Napi::String::New(env, "success");
}

Napi::String createWindow(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::Object args = info[0].ToObject();
  CreateWindowW(L"child_finclip", L"test", WS_VISIBLE, 0, 0, 1024, 768, nullptr,
                nullptr, nullptr, nullptr);
  return Napi::String::New(env, "success");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "start"), Napi::Function::New(env, start));
  exports.Set(Napi::String::New(env, "close"), Napi::Function::New(env, close));
  exports.Set(Napi::String::New(env, "setAppletPos"),
              Napi::Function::New(env, setAppletPos));
  exports.Set(Napi::String::New(env, "createWindow"),
              Napi::Function::New(env, createWindow));
  return exports;
}

NODE_API_MODULE(addon, Init)
}  // namespace NodeFinClip