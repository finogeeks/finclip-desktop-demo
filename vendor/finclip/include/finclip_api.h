#ifndef __H_FINCLIP_WRAPPER_API_H__
#define __H_FINCLIP_WRAPPER_API_H__
#ifdef _WIN32
#    ifndef FINSTDMETHODCALLTYPE
#        define FINSTDMETHODCALLTYPE __stdcall
#        define DLL_EXPORT _declspec(dllexport)
#    endif
#else
#    define FINSTDMETHODCALLTYPE
#    define DLL_EXPORT
#endif

#define FIN_SID const char*
#define FIN_OK 0
#define FIN_FAIL 1
#include <windows.h>
namespace com::finogeeks::finclip::wrapper {

/**
 * @brief 启动机制设置
 * kAsync: 异步启动
 * kBaseLibrarySync: 基础库同步加载
 * kAppletSync: 小程序同步加载
 */
enum StartFlags
{
    kAsync           = 0,
    kBaseLibrarySync = 1 << 0,
    kAppletSync      = 1 << 1,
};
/**
 * @brief 自定义API类型，分别用于小程序和jssdk
 *
 */
enum class FinClipApiType
{
    kApplet,
    kWebView
};
/**
 * @brief 接口基类
 *
 */
struct IKnown
{};

/// SDK统一事件接口

/**
 * @brief API返回结果
{
 "size": 100,
 "data": [[{},{}...],[{},{}...]]
}

 */
struct IResultSet : public IKnown
{
public:
    ///取字段数
    /**@return 返回字段数.
     */
    virtual int GetColCount() = 0;

    ///取字段名
    /** @param column:字段序号(以0为基数)
     * @return 返回字段名  下标越界 返回NULL
     */
    virtual const char* GetColName(int column) = 0;

    //按字段名，取字段值(字符串)
    /**@param columnName: 字段名
     *@return 字符串型字段值,不存在返回NULL
     */
    virtual const char* GetStr(const char* columnName) = 0;

    ///取字段名对应的字段序号
    /**@param columnName: 字段名
     *@return 返回字段序号. 不存在返回-1
     */
    virtual int FindColIndex(const char* columnName) = 0;

    //
    //按字段序号(以0为基数)，取字段值(字符串)
    /**@param column:字段序号(以0为基数)
     *@return 字符串型字段值,下标越界返回NULL
     */
    virtual const char* GetStrByIndex(int column) = 0;

    ///按字段序号获得字段值,二进制数据
    /**@param column: 字段序号(以0为基数)
     *@param   lpRawLen: [out]数据长度
     *@return : 数据首地址
     */
    virtual void* GetRawByIndex(int column, int* lpRawLen) = 0;

    ///按字段名，取字段值
    /**@param columnName:字段名
     *@param   lpRawLen: [out]数据长度
     *@return : 数据首地址
     */
    virtual void* GetRaw(const char* columnName, int* lpRawLen) = 0;

    ///最后一次取的字段值是否为NULL
    /**@return 0 是， 1不是
     */
    virtual int WasNull() = 0;

    ///取下一条记录
    virtual void Next() = 0;

    ///判断是否为结尾
    /**@return 1 是，0 不是;
     */
    virtual int IsEOF() = 0;

    ///判断是否为空
    /**@return 1 是，0 不是;
     */
    virtual int IsEmpty() = 0;

    virtual void* Destroy() = 0;
};
/**
 * @brief SDK返回数据
 *
 */
struct IEvent : public IResultSet
{

    ///取结果集个数
    virtual int GetDatasetCount() = 0;

    ///设置当前结果集
    /**
     *@param  int nIndex				结果集编号
     *@return int						非0 表示成功，否则为失败
     */
    virtual int SetCurrentDatasetByIndex(int nIndex) = 0;

    ///结果集行记录游标接口：取结果集的首条记录
    virtual void First() = 0;

    ///结果集行记录游标接口：取结果集的最后一条记录
    virtual void Last() = 0;

    ///结果集行记录游标接口：取结果集的第n条记录，取值范围[1, GetRowCount()]
    virtual void Go(int nRow) = 0;

    virtual const char* GetBuffer() = 0;

    virtual void Release() = 0;
};

struct IApiCallback
{

public:
    virtual void Callback(const char* event) = 0;
};
//函数指针回调
/**
 * @params ret 调用结果 0成功,1失败
 * @params event 返回结果 IEvent对像
 * 如下格式
 *
 */
typedef void (*FinClipSDKCallback)(IEvent*);

///数据打包器接口
struct IFinPacker : public IKnown
{
public:
    /**
     * @brief 打包准备
     */
    virtual void BeginPacker() = 0;
    /**
     * @brief 往数据包增加内容
     * @detail 分别为KEY,VALUE
     *
     * @param field 数据字段
     * @param value 数据值
     * @return 0成功，1失败
     */
    virtual void Add(const char* field, const char* value) = 0;
    /**
     * @brief 结束打包
     */
    virtual void EndPacker() = 0;

    /**
     * @brief 释放打包器
     */
    virtual void Release() = 0;

    /**
     * @brief 取数据缓冲大小
     * @return 返回数据缓冲大小
     */
    virtual int GetBufferSize() = 0;

    /**
     * @brief 导出数据缓冲
     * @param buffer 存放数据缓冲，由调用方预申请空间
     * @param size   存放数据缓冲长度
     * @return 0成功，1失败
     */
    virtual int Dump(unsigned char* buffer, int* size) = 0;
};

///配置信息接口
struct IFinConfig
{
public:
    /**
     * 功能：设置应用市场
     * SDK支持多应用市场，可以打开不同应用市场的小程序;
     * 不同的应用市场对应不同的domain;
     * 打开小程序时需要指定应用市场
     *@param       app_store：应用市场类型，由开发者自主设置
     *@return void
     */
    virtual void SetAppStore(int app_store) = 0;
    /**
     * @brief 设置加密类型
     * @param encrypt_type 0:标准加密 1:国密
     * @return void
     */
    virtual void SetEncryptType(int encrypt_type) = 0;
    /**
     * @brief 设置应用市场域名
     * @param domain 形如: https://api.finogeeks.com
     * @return void
     */
    virtual void SetDomain(const char* domain) = 0;
    /**
     * @brief 设置Api前辍
     * @param apiprefix /api/v1/mop
     * @return void
     */
    virtual void SetApiPrefix(const char* apiprefix) = 0;
    /**
     * @brief 设置SDK的秘钥
     * @param appkey
     * @return void
     */
    virtual void SetAppKey(const char* appkey) = 0;
    /**
     * @brief 设置SDK的私钥
     * @param secret
     * @return void
     */
    virtual void SetSecret(const char* secret) = 0;
    /**
     * @brief 设置SDK的指纹
     * @param finger
     * @return void
     */
    virtual void SetFinger(const char* finger) = 0;
    /**
     * @brief 设置小程序窗口类型
     * @param type 0:独立窗口，1:子窗口
     * @return void
     */
    virtual void SetAppWindowStyle(int type) = 0;
    /**
     * @brief 离线基础库zip包路径
     * @param path
     * @return void
     */
    virtual void SetOfflineBaseLibrary(const char* path) = 0;
    /**
     * @brief 离线小程序zip包路径
     * @param path
     * @return void
     */
    virtual void SetOfflineApplet(const char* path) = 0;
    /**
     * @brief 设置小程序窗口类型
     * @param type 0:独立窗口，1:子窗口
     * @return void
     */
    virtual void SetShowLoading(int type) = 0;
    /**
     * @brief 设置启动机制
     * @param flag: StartFlags
     * @return void
     */
    virtual void SetStartFlag(int flag) = 0;
};
/*
 * 接口注入api
 */
struct IApi
{
    /**
     * @brief 执行回调
     *
     * @param event 事件名
     * @param param 参数
     * @param callback 结果回调返回
     */
    virtual void invoke(const char* event, const char* param, IApiCallback* callback) = 0;

    /**
     * @brief Api类型，分为小程序api和webview api
     *
     * @return FinClipApiType Api类型
     */
    virtual FinClipApiType GetApiType() const = 0;

    /**
     * @brief 注入的Api名称
     *
     * @return char** 字符串数组
     */
    virtual const char* apis() = 0;
    /**
     * @brief 注入的Api数量
     *
     * @return size_t 数量
     */
    virtual size_t size() = 0;
};

///配置打包器接口
struct IFinConfigPacker : public IKnown
{
public:
    /**
     * @brief 获取配置信息实例
     * @return 返回配置信息实例
     */
    virtual IFinConfig* NewConfig() = 0;

    /**
     * @brief 增加配置信息
     * @return 负数表示失败，0表示成功
     */
    virtual int AddConfig(IFinConfig* config) = 0;
    /**
     * @brief 增加供小程序使用的自定义api
     * @return 负数表示失败，0表示成功
     */
    virtual int RegisterApi(IApi* config) = 0;
    /**
     * @brief 获取配置信息
     * @return 返回配置信息
     */
    virtual IFinConfig* GetConfig(int type) = 0;
    /**
     * @brief 获取配置信息个数
     * @return 返回配置信息个数
     */
    virtual int GetConfigSize() = 0;
    /**
     * @brief 通过索引获取配置信息
     * @return 返回配置信息
     */
    virtual IFinConfig* GetConfigByIndex(int index) = 0;
    /**
     * @brief 获取供小程序使用的自定义api个数
     * @return 返回供小程序使用的自定义api个数
     */
    virtual int GetApiSize() = 0;
    /**
     * @brief 通过索引获取获取供小程序使用的自定义api
     * @return 返回供小程序使用的自定义api
     */
    virtual IApi* GetApiByIndex(int index) = 0;
};

struct IPackerFactory : public IKnown
{
public:
    /**
     * @brief 获取配置打包器
     * @detail 获取配置打包器实例
     *
     * @return 返回配置打包器实例
     */
    virtual IFinConfigPacker* GetFinConfigPacker() = 0;

    /**
     * @brief 获取数据打包器
     * @detail 获取数据打包器实例
     *
     * @return 返回数据打包器实例
     */
    virtual IFinPacker* GetFinPacker() = 0;
};
}   // namespace com::finogeeks::finclip::wrapper
#endif   // !__H_FINCLIPAPI_H__
