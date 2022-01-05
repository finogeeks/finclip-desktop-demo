// FinClip.cpp : Defines the entry point for the application.
//
#include "Resource.h"
#include "vendor/finclip/include/finclip_wrapper.h"
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <tchar.h>

#include "json.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#pragma comment(lib, "FinClipSDKWrapper.lib")

#define MAX_LOADSTRING 100

using namespace std;
using json = nlohmann::json;
using namespace com::finogeeks::finclip::wrapper;
HINSTANCE hInst;
HWND gHwnd;                          // current instance
WCHAR szTitle[MAX_LOADSTRING];       // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING]; // the main window class name

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
RECT g_rect_;
std::string offline_base_path;
std::string offline_applet_path;

std::string utf8_encode(const std::wstring &wstr, int CP = CP_UTF8) {
  if (wstr.empty())
    return std::string();
  int size_needed = WideCharToMultiByte(CP, 0, &wstr[0], (int)wstr.size(), NULL,
                                        0, NULL, NULL);
  std::string strTo(size_needed, 0);
  WideCharToMultiByte(CP, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed,
                      NULL, NULL);
  return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string &str, int CP = CP_UTF8) {
  if (str.empty())
    return std::wstring();
  int size_needed =
      MultiByteToWideChar(CP, 0, &str[0], (int)str.size(), NULL, 0);
  std::wstring wstrTo(size_needed, 0);
  MultiByteToWideChar(CP, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
  return wstrTo;
}

class CustomWebApi : public IApi {

  void invoke(const char *event, const char *param, IApiCallback *callback) {

    std::string data = param;
    std::string e = event;
    callback->Callback("{\"data\":\"ok\"}");
  }

  FinClipApiType GetApiType() const { return FinClipApiType::kWebView; }

  const char *apis() { return "customWebApi"; }
  size_t size() { return 1l; }
};

class CustomApi : public IApi {

  void invoke(const char *event, const char *param, IApiCallback *callback) {

    std::string data = param;
    std::string e = event;
    callback->Callback("{\"data\":\"ok\"}");
  }

  FinClipApiType GetApiType() const { return FinClipApiType::kApplet; }

  const char *apis() { return "customApi"; }
  size_t size() { return 1l; }
};
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  SetProcessDPIAware();
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_FINCLIPWIN32DEMO, szWindowClass, MAX_LOADSTRING);

  MyRegisterClass(hInstance);
  MyRegisterClass1(hInstance);
  if (!InitInstance(hInstance, nCmdShow)) {
    return FALSE;
  }

  HACCEL hAccelTable = LoadAccelerators(hInstance, NULL);

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
  wcex.hIcon = LoadIcon(hInstance, NULL);
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = nullptr;
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);

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
  wcex.hIcon = LoadIcon(hInstance, NULL);
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = nullptr;
  wcex.lpszClassName = L"child_finclip";
  wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);

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
  hInst = hInstance; // Store instance handle in our global variable
  DWORD dwStyle =
      WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX; //���ô�����ʽ

  HWND hWnd =
      CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                    0, 1000, 800, nullptr, nullptr, hInstance, nullptr);
  HMONITOR hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
  MONITORINFO mi = {sizeof(mi)};
  if (!GetMonitorInfo(hmon, &mi))
    return FALSE;
  if (!hWnd) {
    return FALSE;
  }
  int width = 450;
  int height = 500;
  SetWindowPos(hWnd, NULL, mi.rcMonitor.left, mi.rcMonitor.top, width, height,
               SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
  gHwnd = hWnd;
  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  return TRUE;
}

void init_finclipsdk(int app_store, std::wstring wappkey, std::wstring wsecret,
                     std::wstring wdomain) {
  if (is_initialized != 0) {
    return;
  }

  std::string appkey = utf8_encode(wappkey);
  std::string secret = utf8_encode(wsecret);
  std::string domain = utf8_encode(wdomain);

  IPackerFactory *factory = GetPackerFactory();
  IFinConfigPacker *configpacker = factory->GetFinConfigPacker();
  IFinConfig *config = configpacker->NewConfig();
  config->SetAppStore(app_store);
  config->SetApiPrefix("/api/v1/mop");
  config->SetAppKey(appkey.c_str());
  config->SetSecret(secret.c_str());
  config->SetDomain(domain.c_str());
  config->SetEncryptType(1);
  config->SetFinger("");
  config->SetAppWindowStyle(1);
  config->SetOfflineApplet(offline_applet_path.data());
  config->SetOfflineBaseLibrary(offline_base_path.data());
  config->SetStartFlag(StartFlags::kAppletSync);
  config->SetShowLoading(0);
  configpacker->AddConfig(config);
  auto *c_api = new CustomApi();
  configpacker->RegisterApi(c_api);
  auto *c_web_api = new CustomWebApi();
  configpacker->RegisterApi(c_web_api);
  Initialize(hInst, configpacker);
  is_initialized = TRUE;
}

void finclip_applet_callback(IEvent *event) {

  std::string buffer = event->GetBuffer();
  int i = 0;
  if (!event->IsEmpty()) {
    const char *val = event->GetStr("hWnd");
    std::string s(val);
    hWnd_applet = (HWND)strtoul(s.c_str(), NULL, 16);
  }
  event->Release();
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  HWND hwndButtonCintinue;

  switch (message) {

  case WM_COMMAND:
    if (LOWORD(wParam) == IDM_SENDMESSAGE) {
      WCHAR appid[1024];
      GetWindowText(hWnd_appid, appid, 1023);
      std::wstring wappid(appid);
      CleanCache();
      // if (hWnd_container) {
      //     DestroyWindow(hWnd_container);
      // }
      // InvokeWebApi(utf8_encode(wappid).c_str(), "test_custom_api",
      // "{params:'a'}");
      break;
    }
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
      if (wappkey.length() == 0) {
        MessageBox(NULL, L"������appKey", L"��ʾ", 0);
        return 0;
      }
      if (wsecret.length() == 0) {
        MessageBox(NULL, L"������Secret", L"��ʾ", 0);
        return 0;
      }
      if (wappid.length() == 0) {
        MessageBox(NULL, L"������appid", L"��ʾ", 0);
        return 0;
      }
      if (wdomain.length() == 0) {
        MessageBox(NULL, L"������domain", L"��ʾ", 0);
        return 0;
      }
      if (wtype.length() == 0) {
        MessageBox(NULL, L"������type", L"��ʾ", 0);
        return 0;
      }
      int appstore = 1;
      IPackerFactory *factory = GetPackerFactory();
      IFinConfigPacker *configpacker = factory->GetFinConfigPacker();
      IFinConfig *config = configpacker->GetConfig(appstore);
      config->SetAppWindowStyle(std::stol(wtype));
      IFinPacker *packer = factory->GetFinPacker();
      packer->BeginPacker();
      packer->Add("appId", utf8_encode(wappid).c_str());
      packer->Add("query", "1");
      packer->EndPacker();
      int len = packer->GetBufferSize() + 1;
      auto *ret = new unsigned char[len];
      memset(ret, 0, len);
      packer->Dump(ret, &len);
      delete[] ret;
      if (hWnd_container == nullptr) {
        // hWnd_container = CreateWindowW(L"child_finclip", L"���Դ���",
        // WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0,
        //	0, 1920, 1080, NULL, NULL,
        //	hInst, NULL);
        hWnd_container = CreateWindowW(L"child_finclip", L"���Դ���",
                                       WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0,
                                       1024, 768, NULL, NULL, hInst, NULL);
      }
      HRESULT hr =
          StartApplet(hWnd_container, appstore, utf8_encode(wappid).c_str(), "",
                      packer, "", finclip_applet_callback);

      if (hr == S_OK) {
      }
      // SetWindowPos(h, NULL, 0, 300, 400, 436, 0);
      packer->Release();
    }
    break;
  case WM_CLOSE: {
    if (hWnd == hWnd_container) {
      hWnd_container = NULL;
    }
    break;
  }

  case WM_SHOWWINDOW: {
    if (hWnd == hWnd_container) {
      /*RECT rc;
      GetWindowRect(hWnd, &rc);
      auto left = 0;
      auto top = 0;
      auto width = rc.right - rc.left;
      auto height = rc.bottom - rc.top;
      WCHAR appid[1024];
      GetWindowText(hWnd_appid, appid, 1023);
      SetAppletPos(utf8_encode(std::wstring(appid)).c_str(), 1, 0, top, width,
      height);
      */
    } else {
      WCHAR key[1024];
      GetWindowText(hWnd_appkey, key, 1023);
      WCHAR secret[1024];
      GetWindowText(hWnd_secret, secret, 1023);
      WCHAR appid[1024];
      GetWindowText(hWnd_appid, appid, 1023);
      WCHAR domain[1024];
      GetWindowText(hWnd_domain, domain, 1023);
      std::wstring wappkey(key);
      std::wstring wsecret(secret);
      std::wstring wappid(appid);
      std::wstring wdomain(domain);
      init_finclipsdk(1, wappkey, wsecret, wdomain);
    }
  } break;
  case WM_SIZE: {
    WCHAR type[1024];
    GetWindowText(hWnd_type, type, 1023);
    std::wstring wtype(type);
    if (wtype == L"1") {
      UINT width = LOWORD(lParam);
      UINT height = HIWORD(lParam);
      WCHAR appid[1024];
      GetWindowText(hWnd_appid, appid, 1023);
      RECT rect;
      GetClientRect(hWnd_container, &rect);
      SetAppletPos(utf8_encode(std::wstring(appid)).c_str(), 0, 0,
                   rect.right - rect.left, rect.bottom - rect.top, true);
    }

    break;
  }
  // case WM_PAINT:
  // {
  //     // WCHAR type[1024];
  //     // GetWindowText(hWnd_type, type, 1023);
  //     // std::wstring wtype(type);
  //     // if (wtype == L"1") {
  //     //     RECT rect;
  //     //     GetClientRect(hWnd_container, &rect);
  //     //     WCHAR appid[1024];
  //     //     GetWindowText(hWnd_appid, appid, 1023);
  //     //     SetAppletPos(
  //     //         utf8_encode(std::wstring(appid)).c_str(), 0, 0, rect.right -
  //     rect.left, rect.bottom - rect.top,
  //     //         true);
  //     // }
  //     break;
  // }
  case WM_DESTROY: {
    PostQuitMessage(0);
    break;
  }
  case WM_CREATE: {
    if (hWnd_appkey)
      return DefWindowProcW(hWnd, message, wParam, lParam);
    CreateWindowW(L"static", L"AppKEY", WS_CHILD | WS_VISIBLE, 20, 20, 60, 30,
                  hWnd, (HMENU)1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

    CreateWindowW(L"static", L"Secret", WS_CHILD | WS_VISIBLE, 20, 60, 60, 30,
                  hWnd, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

    CreateWindowW(L"static", L"appid", WS_CHILD | WS_VISIBLE, 20, 100, 60, 30,
                  hWnd, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
    CreateWindowW(L"static", L"domain", WS_CHILD | WS_VISIBLE, 20, 140, 60, 30,
                  hWnd, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

    CreateWindowW(L"static", L"window_type", WS_CHILD | WS_VISIBLE, 20, 180, 60,
                  30, hWnd, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance,
                  NULL);
    HMONITOR hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi = {sizeof(mi)};
    if (!GetMonitorInfo(hmon, &mi))
      return false;
    auto left = mi.rcMonitor.left;
    auto top = mi.rcMonitor.top;
    auto width = mi.rcMonitor.right - mi.rcMonitor.left;
    auto height = mi.rcMonitor.bottom - mi.rcMonitor.top;
    // hWnd_container = CreateWindowW(L"static", L"", WS_CHILD | WS_VISIBLE,
    // left,
    //	top, width, height, hWnd, (HMENU)2,
    //	((LPCREATESTRUCT)lParam)->hInstance, NULL);

    wstring domain(L"https://finchat-mop-b.finogeeks.club");
    wstring appkey(L"22LyZEib0gLTQdU3MUauAQVLIkNNhTSGIN42gXzlAsk=");
    wstring appid(L"60e3c059949a5300014d0c07");
    wstring secret(L"ae55433be2f62915");

    // ��exe����, config.json����exeͬһĿ¼
    // ��vs����, config.json����Finclip/
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
        ((LPCREATESTRUCT)lParam)->hInstance, NULL);

    hWnd_secret = CreateWindowW(
        L"EDIT", secret.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
        100, 60, 300, 30, hWnd, (HMENU)IDM_APPLET_SECRET,
        ((LPCREATESTRUCT)lParam)->hInstance, NULL);
    // 5ea0401463cb900001d73865 wxdemo
    // 5ea0412663cb900001d73867 webview
    // 5ea6d3bad262a7000141280d quote
    hWnd_appid = CreateWindowW(L"EDIT", appid.c_str(),
                               WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT, 100,
                               100, 300, 30, hWnd, (HMENU)IDM_APPLET_APPID,
                               ((LPCREATESTRUCT)lParam)->hInstance, NULL);
    hWnd_domain = CreateWindowW(
        L"EDIT", domain.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT,
        100, 140, 300, 30, hWnd, (HMENU)IDM_APPLET_DOMAIN,
        ((LPCREATESTRUCT)lParam)->hInstance, NULL);

    hWnd_type = CreateWindowW(L"EDIT", L"0",
                              WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT, 100,
                              180, 300, 30, hWnd, (HMENU)IDM_APPLET_DOMAIN,
                              ((LPCREATESTRUCT)lParam)->hInstance, NULL);

    HWND hwndButton =
        CreateWindowW(L"BUTTON", L"open", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                      100, 250, 200, 50, hWnd, (HMENU)IDM_START_APPLET,
                      ((LPCREATESTRUCT)lParam)->hInstance, NULL);
    CreateWindowW(L"BUTTON", L"InvokeWebApi",
                  WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 100, 350, 200, 50,
                  hWnd, (HMENU)IDM_SENDMESSAGE,
                  ((LPCREATESTRUCT)lParam)->hInstance, NULL);

  } break;
  }
  return DefWindowProcW(hWnd, message, wParam, lParam);
}
