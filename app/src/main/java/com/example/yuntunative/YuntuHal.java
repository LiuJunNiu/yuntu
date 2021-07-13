package com.example.yuntunative;

import android.annotation.SuppressLint;
import android.telephony.TelephonyManager;
import android.util.Log;

import com.blankj.utilcode.util.LogUtils;
import com.blankj.utilcode.util.StringUtils;
import com.blankj.utilcode.util.Utils;


import org.json.JSONException;
import org.json.JSONObject;


/**
 * Created by willssong on 2021/3/24
 * willssong@tencent.com
 */
class YuntuHal {
    private final static String TAG = "iotsdk";

    public static String getImei() {
        TelephonyManager telephonyManager = Utils.getApp().getSystemService(TelephonyManager.class);
        String imei = telephonyManager.getImei();
        if (StringUtils.isEmpty(imei)) {
            imei = "851234567890";
        }
        LogUtils.d("imei=" + imei);
        return imei;
    }

    public static int getDbm() {
        Log.d(TAG, "getDbm ");
        return -60;
    }

    public static String getICCID() {
        TelephonyManager telephonyManager = Utils.getApp().getSystemService(TelephonyManager.class);
        String iccid = telephonyManager.getSimSerialNumber();

        if (StringUtils.isEmpty(iccid)) {
            iccid = "99345678901234567890";
        }
        LogUtils.d("iccid=" + iccid);
        return iccid;
    }

    public static String getIMSI() {
        TelephonyManager telephonyManager = Utils.getApp().getSystemService(TelephonyManager.class);
        String imsi = telephonyManager.getSubscriberId();
        if (StringUtils.isEmpty(imsi)) {
            imsi = "123456789012345";
        }
        LogUtils.d("imsi=" + imsi);
        return imsi;
    }

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

    public static int getOperator() {
        TelephonyManager telephonyManager = Utils.getApp().getSystemService(TelephonyManager.class);
        String simOperator = telephonyManager.getSimOperator();
        LogUtils.d("getOperator=" + simOperator);
        // 1: china mobile; 2: china unicomm; 3: china telcom
        return 1;
    }

    /**
     * NETWORK_TYPE_UNKNOWN, NETWORK_TYPE_GPRS, NETWORK_TYPE_EDGE, NETWORK_TYPE_UMTS,
     * NETWORK_TYPE_HSDPA, NETWORK_TYPE_HSUPA, NETWORK_TYPE_HSPA, NETWORK_TYPE_CDMA,
     * NETWORK_TYPE_EVDO_0, NETWORK_TYPE_EVDO_A, NETWORK_TYPE_EVDO_B, NETWORK_TYPE_1xRTT,
     * NETWORK_TYPE_IDEN, NETWORK_TYPE_LTE, NETWORK_TYPE_EHRPD, NETWORK_TYPE_HSPAP, NETWORK_TYPE_NR
     *
     * @return
     */
    public static int getNetworkMode() {
        TelephonyManager telephonyManager = Utils.getApp().getSystemService(TelephonyManager.class);
        @SuppressLint("MissingPermission")
        int networkType = telephonyManager.getNetworkType();
        int networkMode = 2;
        // 2: 2G;  3: 3G; 4: 4G
        Log.d(TAG, "getNetworkMode ");
        if (networkType >= 13) {
            networkMode = 4;
        } else if (networkType >= 4) {
            networkMode = 3;
        } else {
            networkMode = 2;
        }
        LogUtils.d("getNetworkMode=" + networkMode);
        return networkMode;
    }

    public static int getNetworkVendor() {
        // 0: mobile;  1: unicom; 2: telecom
        String imsi = getIMSI();
        int networkVendor = 0;
        if (imsi.length() > 0) {
            if (imsi.substring(0, 5).equals("46000") || imsi.substring(0, 5).equals("46002")) {
                //通过前五位判断连的wifi或者数据流量是移动、联通还是电信
                //移动
                networkVendor = 0;
            } else if (imsi.substring(0, 5).equals("46001")) {
                //联通
                networkVendor = 1;
            } else if (imsi.substring(0, 5).equals("46003")) {
                //电信
                networkVendor = 2;
            }
        }
        LogUtils.d("getNetworkVendor=" + networkVendor);
        return networkVendor;
    }

    public static int getPsStatus() {
        // 0: detached  1: attached
        Log.d(TAG, "getPsStatus ");

        return 1;
    }

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

    public static String sendATCmd(String atCmd, int timeOut) {
        Log.d(TAG, "sendATCmd " + atCmd + " in " + timeOut);
//        YunTuUtils.sendATcommand(atCmd);

        return "OK";
    }

    public static int autoDialUp() {
        Log.d(TAG, "autoDialUp");
        return 0;
    }

    public static int resetModem() {
        Log.d(TAG, "resetModem");
        return 0;
    }

    public static int getNetworkDelay() {
        Log.d(TAG, "getNetworkDelay");
        PingNetEntity pingNetEntity = new PingNetEntity("119.29.29.29", 1, 2,
                new StringBuffer());
        pingNetEntity = PintNet.ping(pingNetEntity);
        String pingTime = pingNetEntity.getPingTime();
        LogUtils.d(pingTime);
        return 100;
    }

    public static int saveConfig(String config) {
        Log.d(TAG, "saveConfig " + config);

        return 0;
    }

    public static String readConfig() {
        Log.d(TAG, "readConfig");

        return "Some Configuration";
    }

    public static int saveStrategy(String strategy) {
        Log.d(TAG, "saveStrategy " + strategy);

        return 0;
    }

    public static String readStrategy() {
        Log.d(TAG, "readStrategy");

        return "Some Strategy";
    }

    public static int checkNetwork(String host) {
        Log.d(TAG, "checkNetwork " + host);
        int i = PintNet.checkNetwork(host);
        return 0;
    }

    public static int deviceInit() {
        Log.d(TAG, "deviceInit");

        return 0;
    }
}
