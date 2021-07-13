package com.example.yuntunative;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.blankj.utilcode.util.GsonUtils;
import com.blankj.utilcode.util.LogUtils;
import com.yanzhenjie.permission.AndPermission;


public class MainActivity extends AppCompatActivity {
    public TextView tv;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
        Log.e("TAG", "加载so完成");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        AndPermission.with(this)
                .runtime()
                .permission(new String[]{Manifest.permission.READ_PHONE_STATE,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE,Manifest.permission.INTERNET,
                        "android.permission.READ_PRIVILEGED_PHONE_STATE"})
                .onGranted(strings->{
                    String iccid = YuntuHal.getICCID();
                    LogUtils.d("iccid="+iccid);
                })
                .onDenied(strings->{})
                .start();

        Log.d("iotsdk", "test started");

        // Example of a call to a native method
        tv = findViewById(R.id.sample_text);
        String className = YuntuHal.class.getName();
        Log.d("iotsdk", "className is " + className);
        String res = startYuntu(className);
        Log.d("iotsdk", "res is " + res);
        tv.setText(res);
        //tv.setText("Yuntu Tested");


    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String startYuntu(String className);
}