package com.example.yuntunative;

import android.app.Application;

import com.blankj.utilcode.util.Utils;

public class YuntuApplication extends Application {

    @Override
    public void onCreate() {
        super.onCreate();
        Utils.init(this);
//        try {
//            YunTuUtils.defaultPhone();
//        } catch (Exception e) {
//            e.printStackTrace();
//        }
    }
}
