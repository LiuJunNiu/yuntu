# IoTLink 云兔安卓SDK HAL层开发指导

IoTLink 云兔安卓SDK HAL层开发主要下面三个方法

1、通过AT命令获取，AT命令主要依赖Phone对象，具体路径android.internal.telephony.Phone.java，
2、通过android framework中其他模块方法直接获取，例如cellid和lac可以直接通过telephony管理模块获取等
3、通过AT命令获取，AT命令直接与TTY设备进行交互（其他硬件平台备用）

#### 方法1、Phone对象下提供了下面两个接口供客户收发AT使用
```
void invokeOemRilRequestRaw(byte[] data, Message response); 
void invokeOemRilRequestStrings(String[] strings, Message response); 
```

下面主要使用invokeOemRilRequestStrings方法来实现一个AT COMMAND API来供IoTLink 云兔业务使用

1、使用invokeOemRilRequestStrings实现sendATcommand接口，示例代码如下：

```
public static Handler mResponseHandler = new Handler() {
	@Override
	public void handleMessage(Message msg) {
			for (Field f:fields){
				Log.i(TAG,"fields : "+f.getName());
			}*/
		try {
			Field exceptionField = msg.obj.getClass().getField("exception");
			Exception exception= (Exception) exceptionField.get(msg.obj);
			if (exception != null) {
				Log.e(TAG, " RIL respone emessage exception : "+exception);
				return;
			}
			Field result = msg.obj.getClass().getField("result");
			String[] results = (String[]) result.get(msg.obj);
			switch (msg.what) {
				//judge by cmdID
				case 1:
					Response_ICCID = results[0].substring(results[0].indexOf("+ICCID:")+8);
					Log.i(TAG,"Response RIL ICCID:" + Response_ICCID);
					break;
				case 2:
					...

				default:
					break;
			}
		} catch (Exception e) {
			e.printStackTrace();
			Log.e(TAG, " RIL respone message exception : "+e.getMessage());
		}
	}
};

private static void IoTLinksendATcommand(String cmd1,String cmd2,int cmdID) {
	if (mPhone == null) {
		Log.i(TAG, "mPhone null");
		return;
	}
	Method invokeOemRilRequestStrings = null;
	try {
		String command[] = {cmd1,cmd2};
		Class[] param = new Class[2];
		param[0] = String[].class;
		param[1] = Message.class;
		Class[] p = new Class[]{String[].class, Message.class};
		for (Method method : mPhone.getClass().getMethods()) {
			if (method.getName().equals("invokeOemRilRequestStrings")) {
				invokeOemRilRequestStrings = method;
			}
		}
		invokeOemRilRequestStrings.setAccessible(true);
		invokeOemRilRequestStrings.invoke(mPhone, command, mResponseHandler.obtainMessage(cmdID));
	} catch (Exception e) {
		e.printStackTrace();
		Log.i(TAG, "send AT command exception : " + e.getMessage());
	}
}

```

2、使用扩展的IoTLinksendATcommand获取SIM卡的ICCID号
```
// 函数说明：获取当前卡的ICCID
// 入参：无
// 返回：当前卡的ICCID，string类型
public static String getICCID() {
	IoTLinksendATcommand("AT+ICCID", "+ICCID:", 1)
	Log.d(TAG, "Response_ICCID ");
	return Response_ICCID;
}
```

#### 方法2、android framework中其他模块方法直接获取
	1、获取设备IMEI号，使用telephony模块的telephonyManager.getDeviceId方法
	2、获取SIM卡IMSI号，使用telephony模块的telephonyManager.getSubscriberId方法
	3、获取lac cellid,使用telephony模块的TelephonyManager.getSystemService方法
	4、获取CSQ信号强度，使用telephony模块的TelephonyManager.getSystemService方法
	5、获取GPS定位信息，使用locationManager模块（待详细确定方法）
	6、获取运营商信息，使用telephony模块,TelephonyManager.getSimOperatorName()就可以直接获取到运营商的名字;建议使用TelephonyManager.getSimOperator()获取IMSI，然后根据返回运营商
	7、获取网络制式，使用telephony模块的telephonyManager.getNetworkType方法
	
#### 方法3、通过AT命令获取，AT命令直接与TTY设备进行交互（其他硬件平台备用）

获取AT通道的tty权限，进行读写
详细方案 TODO


#### YuntuHal推荐实现方法
```
class YuntuHal {
    private final static String TAG = "iotsdk";
    // 函数说明：获取IMEI号的接口
    // 入参：无
    // 返回：IMEI，string类型
    // 实现方案推荐上面方法2
    public static String getImei() {
        Log.d(TAG, "getImei ");
        return "851234567890";
    }

    // 函数说明：获取当前信号强度
    // 入参：无
    // 返回：当前RSSI值，int型，单位dbm
    // 实现方案推荐上面方法2
    public static int getDbm() {
        Log.d(TAG, "getDbm ");
        return -60;
    }

    // 函数说明：获取当前卡的ICCID
    // 入参：无
    // 返回：当前卡的ICCID，string类型
    // 实现方案推荐上面方法1，因为随着切卡业务的执行，SIM卡的ICCID是变化的
    public static String getICCID() {
        Log.d(TAG, "getICCID ");
        return "12345678901234567890";
    }

    // 函数说明：获取当前卡的IMSI
    // 入参：无
    // 返回：当前卡的IMSI值，string类型
    // 实现方案推荐上面方法2
    public static String getIMSI() {
        Log.d(TAG, "getIMSI ");
        return "123456789012345";
    }

    // 函数说明：获取当前卡的小区ID
    // 入参：无
    // 返回：当前网络的Cell ID，json格式的string，包括lac数据和cellid数据
    // 实现方案推荐上面方法2
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
    // 实现方案推荐上面方法2
    public static int getOperator() {
        Log.d(TAG, "getOperator ");
        // 1: china mobile; 2: china unicomm; 3: china telcom
        return 1;
    }

    // 函数说明：获取当前网络的网络制式
    // 入参：无
    // 返回：当前网络的网络制式，int类型，2-2G；3-3G；4-4G
    // 实现方案推荐上面方法2
    public static int getNetworkMode() {
        // 2: 2G;  3: 3G; 4: 4G
        Log.d(TAG, "getNetworkMode ");
        return 4;
    }

    // 函数说明：获取当前网络的ps域的附着状态
    // 入参：无
    // 返回：当前网络的ps附着状态，int类型，0-分离；1-附着
    // 实现方案-TODO
    public static int getPsStatus() {
        // 0: detached  1: attached
        Log.d(TAG, "getPsStatus ");
        return 1;
    }

    // 函数说明：获取当前所在的位置
    // 入参：无
    // 返回：当前位置，json格式的string，包括lontitude和latitude
    // 实现方案推荐上面方法2
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
    // 实现方案推荐上面方法1
    public static String sendATCmd(String atCmd, int timeOut) {
        Log.d(TAG, "sendATCmd " + atCmd + " in " + timeOut);
        return "OK";
    }

    // 函数说明：自动拨号上网，在网络断开切卡成功之后，进行重新拨号
    // 入参：无
    // 返回：拨号的结果，int类型，0-成功；其他-失败
    // 实现方案-TODO
    public static int autoDialUp() {
        Log.d(TAG, "autoDialUp");
        return 0;
    }

    // 函数说明：复位modem
    // 入参：无
    // 返回：modem复位操作结果，int类型，0-成功；其他-失败
    // 实现方案-TODO
    public static int resetModem() {
        Log.d(TAG, "resetModem");
        return 0;
    }

    // 函数说明：获取当前网络时延
    // 入参：无
    // 返回：当前时延，int类型，单位ms
    // 实现方案-TODO
    public static int getNetworkDelay() {
        Log.d(TAG, "getNetworkDelay");

        return 100;
    }

    // 函数说明：保存配置到本地，设备需要提供一个云兔卡配置的保存方法
    // 入参：需要保存的配置，string类型
    // 返回：保存操作的结果，int类型，0-成功；其他-失败
    // 实现方案-TODO
    public static int saveConfig(String config) {
        Log.d(TAG, "saveConfig " + config);

        return 0;
    }

    // 函数说明：读取保存的云兔卡配置
    // 入参：无
    // 返回：配置数据，string类型
    // 实现方案-TODO
    public static String readConfig() {
        Log.d(TAG, "readConfig");

        return "Some Configuration";
    }

    // 函数说明：保存当前配置的策略
    // 入参：策略数据，string类型
    // 返回：保存的操作结果，int类型，0-成功；其他-失败
    // 实现方案-TODO
    public static int saveStrategy(String strategy) {
        Log.d(TAG, "saveStrategy " + strategy);

        return 0;
    }

    // 函数说明：读取保存的策略
    // 入参：无
    // 返回：策略数据，string类型
    // 实现方案-TODO
    public static String readStrategy() {
        Log.d(TAG, "readStrategy");

        return "Some Strategy";
    }

    // 函数说明：检查当前网络
    // 入参：用于检查网络的host地址，string类型
    // 返回：当前网络的状态，int类型，0-网络正常，其他-异常
    // 实现方案-TODO
    public static int checkNetwork(String host) {
        Log.d(TAG, "checkNetwork " + host);

        return 0;
    }

    // 函数说明：设备初始化
    // 入参：无
    // 返回：设备初始化结果，int类型，0-成功，其他-异常
    // 实现方案-TODO
    public static int deviceInit() {
        Log.d(TAG, "deviceInit");

        return 0;
    }
}
```

