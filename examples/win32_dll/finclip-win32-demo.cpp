﻿// FinClip.cpp : Defines the entry point for the application.
//
#include "resource.h"

// Windows Header Files
#include <windows.h>
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

#include "finclip-win32-demo.h"
#include "json.hpp"

#define MAX_LOADSTRING 100

using namespace std;
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
HINSTANCE dll;

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

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                       _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                       _In_ int nCmdShow) {
  SetProcessDPIAware();
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_FINCLIPWIN32DEMO, szWindowClass, MAX_LOADSTRING);

  MyRegisterClass(hInstance);
  MyRegisterClass1(hInstance);
  if (InitInstance(hInstance, nCmdShow) == 0) {
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

  return static_cast<int>(msg.wParam);
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

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  hInst = hInstance;  // Store instance handle in our global variable
  HWND hwnd =
      CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                    0, 1000, 1200, nullptr, nullptr, hInstance, nullptr);
  HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
  MONITORINFO mi = {sizeof(mi)};
  if (!GetMonitorInfo(hmon, &mi)) return FALSE;
  if (hwnd == nullptr) {
    return FALSE;
  }
  int width = 450;
  int height = 1000;
  SetWindowPos(hwnd, nullptr, mi.rcMonitor.left, mi.rcMonitor.top, width,
               height, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
  gHwnd = hwnd;
  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  return TRUE;
}

void InitFinclipsdk(const std::string& app_store, const std::wstring& wappkey,
                    const std::wstring& wsecret, const std::wstring& wdomain) {
  if (is_initialized != 0) {
    return;
  }
  dll = LoadLibrary(
      LR"(C:\project\finclipsdk-desktop\build\wrapper\Release\FinClipSDKWrapper.dll)");
  if (dll == nullptr) {
    return;
  }
  auto finclip_dll_get_packer_factory =
      reinterpret_cast<findll_get_packer_factory>(
          GetProcAddress(dll, "finclip_get_packer_factory"));
  auto finclip_dll_packer_factory_get_config_packer =
      reinterpret_cast<findll_packer_factory_get_config_packer>(
          GetProcAddress(dll, "finclip_packer_factory_get_config_packer"));
  auto finclip_dll_config_packer_add_config =
      reinterpret_cast<findll_config_packer_add_config>(
          GetProcAddress(dll, "finclip_config_packer_add_config"));

  auto finclip_dll_initialize = reinterpret_cast<findll_initialize>(
      GetProcAddress(dll, "finclip_initialize"));
  auto finclip_dll_start_applet = reinterpret_cast<findll_start_applet>(
      GetProcAddress(dll, "finclip_start_applet"));
  auto finclip_dll_register_api = reinterpret_cast<findll_register_api>(
      GetProcAddress(dll, "finclip_register_api"));
  auto finclip_dll_create_params = reinterpret_cast<findll_create_params>(
      GetProcAddress(dll, "finclip_create_params"));
  auto finclip_dll_destory_params = reinterpret_cast<findll_destory_params>(
      GetProcAddress(dll, "finclip_destory_params"));
  auto finclip_dll_params_set = reinterpret_cast<findll_params_set>(
      GetProcAddress(dll, "finclip_params_set"));

  std::string appkey = Utf8Encode(wappkey);
  std::string secret = Utf8Encode(wsecret);
  std::string domain = Utf8Encode(wdomain);

  auto* factory = finclip_dll_get_packer_factory();
  auto* packer = finclip_dll_packer_factory_get_config_packer(factory);
  finclip_dll_initialize(packer);
  auto* config = finclip_dll_create_params();
  finclip_dll_params_set(config, FINCLIP_CONFIG_APPSTORE, "1");
  finclip_dll_params_set(config, FINCLIP_CONFIG_APPKEY, appkey.c_str());
  finclip_dll_params_set(config, FINCLIP_CONFIG_SECRET, secret.c_str());
  finclip_dll_params_set(config, FINCLIP_CONFIG_DOMAIN, domain.c_str());
  std::string exe_path =
      R"(C:\Users\yx\Downloads\1.2.1-alpha-debug.73 (1)\finclip\FinClip.exe)";
  finclip_dll_params_set(config, FINCLIP_CONFIG_EXE_PATH, exe_path.c_str());

  finclip_dll_config_packer_add_config(packer, config);
  // 启动
  // finclip_dll_start_applet(app_store, appid.c_str(), nullptr);
  // finclip_dll_destory_params(config);
  is_initialized = TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  switch (message) {
    case WM_COMMAND:
      if (LOWORD(wParam) == IDM_SENDMESSAGE) {
        WCHAR appid[1024];
        GetWindowText(hWnd_appid, appid, 1023);
        std::wstring wappid(appid);
        // CleanCache();
        break;
      }
      // 打开隐藏小程序进程的例子
      if (LOWORD(wParam) == IDM_SENDMESSAGE_1) {
        // 小程序启动过一次, 调用unbind后, 再次打开
        WCHAR appid[1024];
        GetWindowText(hWnd_appid, appid, 1023);
        std::wstring wappid(appid);
        // 重新显示小程序
        // StartApplet(hWnd_container, 0, Utf8Encode(wappid).c_str(), "",
        // nullptr,
        //             "", nullptr);
        // 宿主处理自己的窗口
        ShowWindow(hWnd_container, SW_SHOWNORMAL);
        break;
      }
      if (LOWORD(wParam) == IDM_SENDMESSAGE_2) {
        // 关闭但不销毁(隐藏)小程序进程,
        // CloseApplet(hWnd_container, false);
        // 宿主处理自己的窗口
        ShowWindow(hWnd_container, SW_HIDE);
        break;
      }
      // open按钮处理逻辑
      if (LOWORD(wParam) == IDM_START_APPLET) {
        WCHAR key[1024];
        GetWindowText(hWnd_appkey, key, 1023);
        WCHAR secret[1024];
        GetWindowText(hWnd_secret, secret, 1023);
        WCHAR appid[1024];
        GetWindowText(hWnd_appid, appid, 1023);
        WCHAR domain[1024];
        GetWindowText(hWnd_domain, domain, 1023);
        WCHAR type[1024];
        GetWindowText(hWnd_type, type, 1023);
        std::wstring wappkey(key);
        std::wstring wsecret(secret);
        std::wstring wappid(appid);
        std::wstring wdomain(domain);
        std::wstring wtype(type);
        std::string appstore = "1";
        if (wappkey.length() == 0) {
          MessageBox(nullptr, L"无效的appKey", L"错误", 0);
          return 0;
        }
        if (wsecret.length() == 0) {
          MessageBox(nullptr, L"无效的Secret", L"错误", 0);
          return 0;
        }
        if (wappid.length() == 0) {
          MessageBox(nullptr, L"无效的appid", L"错误", 0);
          return 0;
        }
        if (wdomain.length() == 0) {
          MessageBox(nullptr, L"无效的domain", L"错误", 0);
          return 0;
        }
        if (wtype.length() == 0) {
          MessageBox(nullptr, L"无效的type", L"错误", 0);
          return 0;
        }
        InitFinclipsdk(appstore, wappkey, wsecret, wdomain);
        auto finclip_dll_start_applet = reinterpret_cast<findll_start_applet>(
            GetProcAddress(dll, "finclip_start_applet"));
        finclip_dll_start_applet(appstore.c_str(), Utf8Encode(wappid).c_str());
      }
      break;
    case WM_SIZE: {
      if (hWnd_container == hWnd) {
        // 嵌入模式须重新设置窗口大小
        WCHAR type[1024];
        GetWindowText(hWnd_type, type, 1023);
        std::wstring wtype(type);
        if (wtype == L"1") {
          // 根据实际情况取出appid
          WCHAR appid[1024];
          GetWindowText(hWnd_appid, appid, 1023);
          // 根据实际情况获取变化后的窗口大小
          RECT rect;
          GetClientRect(hWnd_container, &rect);
          auto finclip_dll_set_position = reinterpret_cast<findll_set_position>(
              GetProcAddress(dll, "finclip_set_position"));
          finclip_dll_set_position(Utf8Encode(appid).c_str(), 0, 0,
                                   rect.right - rect.left,
                                   rect.bottom - rect.top);
        }
      }
      break;
    }
    case WM_DESTROY: {
      if (hWnd == hWnd_container) {
        // 收到WM_DESTROY消息, 表示容器窗口被销毁, 需要SDK的资源
        // CloseApplet(hWnd, true);
        hWnd_container = nullptr;
        return 0;
      }
      if (hWnd == gHwnd) {
        // 只有主窗口关闭时退出进程
        PostQuitMessage(0);
      }
      break;
    }
    case WM_CREATE: {
      if (hWnd_appkey != nullptr)
        return DefWindowProcW(hWnd, message, wParam, lParam);
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

      wstring domain(L"");
      wstring appkey(L"");
      wstring appid(L"");
      wstring secret(L"");

      auto path = std::filesystem::current_path();
      if (std::filesystem::exists("config.json")) {
        std::ifstream t("config.json");
        std::stringstream buffer;
        buffer << t.rdbuf();
        auto obj = nlohmann::json::parse(buffer.str());
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
