using System;
using System.Collections.Generic;
using System.DirectoryServices.ActiveDirectory;
using System.Linq;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WpfApp
{
    
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("FinClipSDKWrapper.dll", SetLastError = true)]
        public static extern IntPtr finclip_create_params();
        [DllImport("FinClipSDKWrapper.dll", SetLastError = true)]
        public static extern Boolean finclip_params_set(IntPtr p, [MarshalAs(UnmanagedType.LPUTF8Str)] string a, [MarshalAs(UnmanagedType.LPUTF8Str)] string b);
        [DllImport("FinClipSDKWrapper.dll", SetLastError = true)]
        public static extern Int32 finclip_init_with_config([MarshalAs(UnmanagedType.LPUTF8Str)] string a, IntPtr config);
        [DllImport("FinClipSDKWrapper.dll", SetLastError = true)]
        public static extern Int32 finclip_start_applet([MarshalAs(UnmanagedType.LPUTF8Str)] string app_store, [MarshalAs(UnmanagedType.LPUTF8Str)] string appid);
        private string DOMAIN = "";
        private string APP_KEY = "";
        private string SECRET = "";
        private string APPID = "";
        private string appstore = "test";

        private void OnLoaded(object sender, RoutedEventArgs routedEventArgs)
        {
            int res = finclip_start_applet(appstore, APPID);
        }
        public MainWindow()
        {
            IntPtr config = finclip_create_params();
            finclip_params_set(config, "appkey", APP_KEY);
            finclip_params_set(config, "secret", SECRET);
            finclip_params_set(config, "domain", DOMAIN);
            // 下载sdk后, 将working dir 设置到FinClipSDKWrapper.dll所在的文件夹
            // 这样相对路径才可以生效, 如果有所改动, 请自行调整参数
            finclip_params_set(config, "exe_path", "finclip/FinClip.exe");

            finclip_init_with_config(appstore, config);
            InitializeComponent();
            Loaded += OnLoaded;
            this.Title = "FinClip Desktop Demo";
        }
    }
}
