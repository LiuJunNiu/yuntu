package com.example.yuntunative;

import android.content.Context;

import androidx.test.platform.app.InstrumentationRegistry;
import androidx.test.ext.junit.runners.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

import static org.junit.Assert.*;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
public class ExampleInstrumentedTest {
    @Test
    public void useAppContext() {
        // Context of the app under test.
        Context appContext = InstrumentationRegistry.getInstrumentation().getTargetContext();
        assertEquals("com.example.yuntunative", appContext.getPackageName());
    }

    @Test
    public void pintTest() {
        PingNetEntity pingNetEntity = new PingNetEntity("119.29.29.29", 3, 5, new StringBuffer());
        pingNetEntity = PintNet.ping(pingNetEntity);
        String pingTime = pingNetEntity.getPingTime();
        System.out.println(GsonUtils.tojson(pingNetEntity));
    }
}