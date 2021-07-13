package com.example.yuntunative;

import android.annotation.SuppressLint;
import android.app.Application;
import android.content.Context;
import android.content.pm.PackageManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.telephony.CellLocation;
import android.telephony.PhoneStateListener;
import android.telephony.ServiceState;
import android.telephony.SignalStrength;
import android.telephony.TelephonyManager;
import android.telephony.cdma.CdmaCellLocation;
import android.telephony.gsm.GsmCellLocation;
import android.util.Log;

import androidx.annotation.RequiresApi;
import androidx.core.app.ActivityCompat;

import com.blankj.utilcode.util.LogUtils;
import com.blankj.utilcode.util.StringUtils;
import com.blankj.utilcode.util.Utils;


import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.net.InetAddress;


/**
 * Created by willssong on 2021/3/24
 * willssong@tencent.com
 */
class YuntuHal {
    private final static String TAG = "iotsdk";

    private static final int QCLOUD_RET_SUCCESS = 0;
    private static final int QCLOUD_ERR_FAILURE = -1001;
    private static int lastSignal;

    private static TelephonyManager teleManager;

    public static String getImei() {
        if (teleManager == null) return "851234567890";
        String imei = teleManager.getImei();
        if (StringUtils.isEmpty(imei)) {
            imei = "851234567890";
        }
        LogUtils.d("imei=" + imei);
        return imei;
    }

    public static int getDbm() {
        Log.d(TAG, "getDbm " + lastSignal);
        return lastSignal;
    }

    public static String getICCID() {//99860619120071523724
        String iccid = teleManager.getSimSerialNumber();

        if (StringUtils.isEmpty(iccid)) {
            iccid = "99345678901234567890";
        }
        LogUtils.d("iccid=" + iccid);
        return iccid;
    }

    public static String getIMSI() {
        String imsi = teleManager.getSubscriberId();
        if (StringUtils.isEmpty(imsi)) {
            imsi = "123456789012345";
        }
        LogUtils.d("imsi=" + imsi);
        return imsi;
    }

    public static String getCellid() {
        Log.d(TAG, "getCellid ");
        CellLocation cellLocation = teleManager.getCellLocation();
        String lat = "";
        String cell = "";
        if (cellLocation instanceof GsmCellLocation) {
            cell = ((GsmCellLocation)cellLocation).getCid() + "";
            lat = ((GsmCellLocation)cellLocation).getLac() + "";
        } else if (cellLocation instanceof CdmaCellLocation) {
            cell = ((CdmaCellLocation)cellLocation).getBaseStationId() + "";
            lat = ((CdmaCellLocation)cellLocation).getNetworkId() + "";
        }
        JSONObject jsonObject = new JSONObject();
        try {
            jsonObject.put("lat", lat);
            jsonObject.put("cell", cell);
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return jsonObject.toString();
    }

    public static int getOperator() {
//        TelephonyManager telephonyManager = Utils.getApp().getSystemService(TelephonyManager.class);
//        String simOperator = telephonyManager.getSimOperator();
        int networkVendor = getNetworkVendor();
        networkVendor++;
        LogUtils.d("getOperator=" + networkVendor);
        // 1: china mobile; 2: china unicomm; 3: china telcom
        return networkVendor;
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
        @SuppressLint("MissingPermission")
        int networkType = teleManager.getNetworkType();
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
        ServiceState serviceState = teleManager.getServiceState();
        serviceState.getRoaming()
        return 1;
    }

    public static String getLocation() {
        // {"lontitude":"100", "latitude":"200"}
        Log.d(TAG, "getLocation ");
        LocationManager mlocationManager = (LocationManager) Utils.getApp().
                getSystemService(Context.LOCATION_SERVICE);

        double latitude = 0;
        double longitude = 0;
        if (mlocationManager.isProviderEnabled(LocationManager.GPS_PROVIDER)) {

            Location location = mlocationManager.getLastKnownLocation(LocationManager.GPS_PROVIDER);
            if(location != null){
                latitude = location.getLatitude();
                longitude = location.getLongitude();
            }
        } else {
            LocationListener locationListener = new LocationListener() {
                // Provider的状态在可用、暂时不可用和无服务三个状态直接切换时触发此函数
                @Override
                public void onStatusChanged(String provider, int status, Bundle extras) {

                }
                // Provider被enable时触发此函数，比如GPS被打开
                @Override
                public void onProviderEnabled(String provider) {

                }
                // Provider被disable时触发此函数，比如GPS被关闭
                @Override
                public void onProviderDisabled(String provider) {

                }
                //当坐标改变时触发此函数，如果Provider传进相同的坐标，它就不会被触发
                @Override
                public void onLocationChanged(Location location) {
                   /* if (location != null) {
                        Log.e("Map", "Location changed : Lat: "
                                + location.getLatitude() + " Lng: "
                                + location.getLongitude());
                    }*/
                }
            };
            mlocationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER,1000, 0, locationListener);
            Location location = mlocationManager.getLastKnownLocation(LocationManager.NETWORK_PROVIDER);
            if(location != null){
                latitude = location.getLatitude(); //经度
                longitude = location.getLongitude(); //纬度
            }

        }
        JSONObject jsonObject = new JSONObject();
        try {
            jsonObject.put("lontitude", longitude + "");
            jsonObject.put("latitude", latitude + "");
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return jsonObject.toString();
    }

    public static String sendATCmd(String atCmd, int timeOut) {
        Log.d(TAG, "sendATCmd " + atCmd + " in " + timeOut);
//        YunTuUtils.sendATcommand(atCmd);
        if (teleManager.hasCarrierPrivileges()) {
            return teleManager.sendEnvelopeWithStatus(atCmd);
        }
        return "OK";
    }

    public static int autoDialUp() {
        Log.d(TAG, "autoDialUp");
        teleManager.setDataEnabled(true);
        return 0;
    }

    public static int resetModem() {
        Log.d(TAG, "resetModem");

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            teleManager.setDataEnabled(false);
            Log.d(TAG, "resetModem " +teleManager.getDataState());
            teleManager.setDataEnabled(true);
            return QCLOUD_RET_SUCCESS;
        } else {
            return QCLOUD_RET_SUCCESS;
        }
    }

    public static int getNetworkDelay() {
        Log.d(TAG, "getNetworkDelay");
//        PingNetEntity pingNetEntity = new PingNetEntity("119.29.29.29", 1, 2,
//                new StringBuffer());
//        pingNetEntity = PintNet.ping(pingNetEntity);
//        String pingTime = pingNetEntity.getPingTime();
//        LogUtils.d(pingTime);
        long delay2 = new NetPingManager(Utils.getApp(), "119.29.29.29").getDelay2();
        Log.d(TAG, "getNetworkDelay " + delay2);
        return (int) delay2;
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

        try {
            int is = InetAddress.getByName(host).isReachable(3000) ? QCLOUD_RET_SUCCESS :QCLOUD_ERR_FAILURE;
            Log.d(TAG, "checkNetwork " + is);
            return is;
        } catch (IOException e) {
            return QCLOUD_ERR_FAILURE;
        }
    }

    public static int deviceInit() {
        Log.d(TAG, "deviceInit");
        teleManager = (TelephonyManager) Utils.getApp().
                getSystemService(Context.TELEPHONY_SERVICE);
        if (teleManager == null) return QCLOUD_ERR_FAILURE;
        Handler handler = new Handler(Looper.getMainLooper());
        handler.post(new Runnable() {
            @Override
            public void run() {
                teleManager.listen(new PhoneStateListener() {
                    @Override
                    public void onSignalStrengthsChanged(SignalStrength signalStrength) {
                        super.onSignalStrengthsChanged(signalStrength);
                        int asu = signalStrength.getGsmSignalStrength();
                        lastSignal = -113 + 2 * asu;
                    }
                }, PhoneStateListener.LISTEN_SIGNAL_STRENGTHS);
            }
        });
        return QCLOUD_RET_SUCCESS;
    }
}
