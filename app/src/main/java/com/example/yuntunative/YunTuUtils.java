package com.example.yuntunative;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.telephony.TelephonyManager;
import android.util.Log;

import com.blankj.utilcode.util.Utils;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class YunTuUtils {

    private static final String TAG = YunTuUtils.class.getSimpleName();

    private static Object sPhone;

    /**
     * 获取默认的phone对象
     * android.internal.telephony.Phone
     *
     * @return
     */
    public static Object defaultPhone() throws Exception {
        Class phoneFactoryClazz = Class.forName("com.android.internal.telephony.PhoneFactory");
        Method getDefaultPhone = phoneFactoryClazz.getMethod("getDefaultPhone");
        sPhone = getDefaultPhone.invoke(null);
        return sPhone;
    }


    private static Handler sResponseHandler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(Message msg) {
            try {
                Field exceptionField = msg.obj.getClass().getField("exception");
                Exception exception = (Exception) exceptionField.get(msg.obj);
                if (exception != null) {
                    Log.e(TAG, " RIL respone emessage exception : " + exception);
                    return;
                }
                Field result = msg.obj.getClass().getField("result");
                String[] results = (String[]) result.get(msg.obj);
                switch (msg.what) {
                    //judge by cmdID
                    case 1:
                        String Response_ICCID =
                                results[0].substring(results[0].indexOf("+ICCID:") + 8);
                        Log.i(TAG, "Response RIL ICCID:" + Response_ICCID);
                        break;
                    default:
                        break;
                }
            } catch (Exception e) {
                e.printStackTrace();
                Log.e(TAG, " RIL respone message exception : " + e.getMessage());
            }
        }
    };

    public static void IoTLinksendATcommand(String cmd1, String cmd2, int cmdID) {
        if (sPhone == null) {
            Log.i(TAG, "sPhone null");
            return;
        }
        Method invokeOemRilRequestStrings = null;
        try {
            String command[] = {cmd1, cmd2};
            for (Method method : sPhone.getClass().getMethods()) {
                if (method.getName().equals("invokeOemRilRequestStrings")) {
                    invokeOemRilRequestStrings = method;
                }
            }
            invokeOemRilRequestStrings.setAccessible(true);
            invokeOemRilRequestStrings.invoke(sPhone, command,
                    sResponseHandler.obtainMessage(cmdID));
        } catch (Exception e) {
            e.printStackTrace();
            Log.i(TAG, "send AT command exception : " + e.getMessage());
        }
    }

    public static void sendATcommand(String cmd) {
        if (sPhone == null) {
            Log.i(TAG, "sPhone null");
            return;
        }
        Method invokeOemRilRequestStrings = null;
        try {
            String command[] = {cmd};
            for (Method method : sPhone.getClass().getMethods()) {
                if (method.getName().equals("invokeOemRilRequestStrings")) {
                    invokeOemRilRequestStrings = method;
                }
            }
            invokeOemRilRequestStrings.setAccessible(true);
            invokeOemRilRequestStrings.invoke(sPhone, command,
                    sResponseHandler.obtainMessage());
        } catch (Exception e) {
            e.printStackTrace();
            Log.i(TAG, "send AT command exception : " + e.getMessage());
        }
    }
}
