# 云兔卡Android示例工程
## 整体架构
[](https://main.qcloudimg.com/raw/47c75c1370c080a135223bddf481c429.jpg)
本工程对应的是Native中的Android Wrapper层和Java层
1. 在Android Wrapper层主要做了Java层回调函数的管理
2. 在Java层实现了一个Modem操作相关的类，里面封装了需要设备商实现的静态接口函数，这些函数会在Native中的Android Wrapper层被调用

Android SDK复用了C-SDK，将C-SDK作为native层的一个so，这样做的优点是：
1. 总体上只需要维护一套核心逻辑的SDK，后续的维护更新成本低
2. 核心逻辑可以以动态库的形式提供，和客户二次开发的业务层区隔开便于软件授权的管理（后续c-sdk需要支持通过三元组的传入作为设备标识）
Android版本会分为两个工程来管理：
1. Android NDK工程：依赖C-SDK，通过ndk编译环境，自动编译出Android版本的so，编译出的动态库复制到app/src/main/libs
2. Android APP工程：基于Android NDK编译出的so(app/src/main/libs目录下)，需要传入对应modem操作回调函数，然后实现切卡、自动策略逻辑管理

## 代码说明
Yuntu SDK依赖系统提供的modem操作接口，对于Android系统，有两种实现方式：
1. 在Native层使用C/C++实现，但native层但依赖root权限以及厂家的接口说明，通用性不好
2. 在Application层使用Java语言实现，可以通过Android RIL发送指令到modem，通用性好
回调函数封装在一个类中，所有回调函数以public static形式定义，这些API和qcloud-iot-sdk-ytcard-embedded-c中QCLOUD_YTCARD_HAL_API_S里面的C语言的API是一一对应的。
```
class YuntuHal {
    private final static String TAG = "iotsdk";
    // 函数说明：获取IMEI号的接口
    // 入参：无
    // 返回：IMEI，string类型
    public static String getImei() {
        Log.d(TAG, "getImei ");
        return "851234567890";
    }

    // 函数说明：获取当前信号强度
    // 入参：无
    // 返回：当前RSSI值，int型，单位dbm
    public static int getDbm() {
        Log.d(TAG, "getDbm ");
        return -60;
    }

    // 函数说明：获取当前卡的ICCID
    // 入参：无
    // 返回：当前卡的ICCID，string类型
    public static String getICCID() {
        Log.d(TAG, "getICCID ");
        return "12345678901234567890";
    }

    // 函数说明：获取当前卡的IMSI
    // 入参：无
    // 返回：当前卡的IMSI值，string类型
    public static String getIMSI() {
        Log.d(TAG, "getIMSI ");
        return "123456789012345";
    }

    // 函数说明：获取当前卡的小区ID
    // 入参：无
    // 返回：当前网络的Cell ID，json格式的string，包括lac数据和cellid数据
    public static String getCellid() {
        Log.d(TAG, "getCellid ");
        JSONObject jsonObject = new JSONObject();
        try {
            jsonObject.put("lat", "1234");
            jsonObject.put("cell", "200");
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return jsonObject.toString();
    }

    // 函数说明：获取当前网络的运营商
    // 入参：无
    // 返回：网络运营商，int类型，1-中国移动；2-中国联通；3-中国电信
    public static int getOperator() {
        Log.d(TAG, "getOperator ");
        // 1: china mobile; 2: china unicomm; 3: china telcom
        return 1;
    }

    // 函数说明：获取当前网络的网络制式
    // 入参：无
    // 返回：当前网络的网络制式，int类型，2-2G；3-3G；4-4G
    public static int getNetworkMode() {
        // 2: 2G;  3: 3G; 4: 4G
        Log.d(TAG, "getNetworkMode ");
        return 4;
    }

    // 函数说明：获取当前网络的ps域的附着状态
    // 入参：无
    // 返回：当前网络的ps附着状态，int类型，0-分离；1-附着
    public static int getPsStatus() {
        // 0: detached  1: attached
        Log.d(TAG, "getPsStatus ");
        return 1;
    }

    // 函数说明：获取当前所在的位置
    // 入参：无
    // 返回：当前位置，json格式的string，包括lontitude和latitude
    public static String getLocation() {
        // {"lontitude":"100", "latitude":"200"}
        Log.d(TAG, "getLocation ");
        JSONObject jsonObject = new JSONObject();
        try {
            jsonObject.put("lontitude", "100");
            jsonObject.put("latitude", "200");
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return jsonObject.toString();
    }

    // 函数说明：发送AT指令
    // 入参：atCmd：string类型，AT指令；timeOut：int类型，超时时间
    // 返回：AT指令的执行结果，string类型
    public static String sendATCmd(String atCmd, int timeOut) {
        Log.d(TAG, "sendATCmd " + atCmd + " in " + timeOut);
        return "OK";
    }

    // 函数说明：自动拨号上网，在网络断开切卡成功之后，进行重新拨号
    // 入参：无
    // 返回：拨号的结果，int类型，0-成功；其他-失败
    public static int autoDialUp() {
        Log.d(TAG, "autoDialUp");
        return 0;
    }

    // 函数说明：复位modem
    // 入参：无
    // 返回：modem复位操作结果，int类型，0-成功；其他-失败
    public static int resetModem() {
        Log.d(TAG, "resetModem");
        return 0;
    }

    // 函数说明：获取当前网络时延
    // 入参：无
    // 返回：当前时延，int类型，单位ms
    public static int getNetworkDelay() {
        Log.d(TAG, "getNetworkDelay");

        return 100;
    }

    // 函数说明：保存配置到本地，设备需要提供一个云兔卡配置的保存方法
    // 入参：需要保存的配置，string类型
    // 返回：保存操作的结果，int类型，0-成功；其他-失败
    public static int saveConfig(String config) {
        Log.d(TAG, "saveConfig " + config);

        return 0;
    }

    // 函数说明：读取保存的云兔卡配置
    // 入参：无
    // 返回：配置数据，string类型
    public static String readConfig() {
        Log.d(TAG, "readConfig");

        return "Some Configuration";
    }

    // 函数说明：保存当前配置的策略
    // 入参：策略数据，string类型
    // 返回：保存的操作结果，int类型，0-成功；其他-失败
    public static int saveStrategy(String strategy) {
        Log.d(TAG, "saveStrategy " + strategy);

        return 0;
    }

    // 函数说明：读取保存的策略
    // 入参：无
    // 返回：策略数据，string类型
    public static String readStrategy() {
        Log.d(TAG, "readStrategy");

        return "Some Strategy";
    }

    // 函数说明：检查当前网络
    // 入参：用于检查网络的host地址，string类型
    // 返回：当前网络的状态，int类型，0-网络正常，其他-异常
    public static int checkNetwork(String host) {
        Log.d(TAG, "checkNetwork " + host);

        return 0;
    }

    // 函数说明：设备初始化
    // 入参：无
    // 返回：设备初始化结果，int类型，0-成功，其他-异常
    public static int deviceInit() {
        Log.d(TAG, "deviceInit");

        return 0;
    }
}
```
## TODO
1. 在Java层和RIL接口的对接还没做？
2. 自动化编译构建
