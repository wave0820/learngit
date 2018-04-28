/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.PowerManager;  
import android.os.PowerManager.WakeLock;

import static com.fanwei.jubaosdk.common.util.SdkConst.REQUESTCODE;
import static com.fanwei.jubaosdk.common.util.SdkConst.RESULTCODE;


public class AppActivity extends Cocos2dxActivity {
	WakeLock m_wklk;
	private final static String CODE = "code";
	private final static String MESSAGE = "message";

    protected void onCreate(Bundle savedInstanceState){  
        super.onCreate(savedInstanceState);  
        
        System.out.println("onCreate.....");
        PowerManager pm = (PowerManager)getSystemService(POWER_SERVICE);
        m_wklk = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "cn");

        m_wklk.acquire(); //设置保持唤醒
    }  
      
    protected void onPause() {  
        super.onPause();  
        //程序暂停运行于后台时调用的方法，在这里加入下边的移除不锁屏功能代码。  
        if(m_wklk != null) {  
        	m_wklk.release();  
        	m_wklk = null;  
        }  
    }  
      
    protected void onResume() {  
        super.onResume();  
        //程序由后台转入前台的时候调用的方法onResume()中加入下边的关闭锁屏代码  
        if(m_wklk == null) {  
           PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);  
           m_wklk = pm.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK, "XYTEST");  
           m_wklk.acquire();  
        }  
    }  
      
    public void onDestroy() {  
        //取消设置  
        super.onDestroy();  
        if(m_wklk == null) {  
        	m_wklk.release();  
        }  
    }  
    

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
		if (requestCode == REQUESTCODE && resultCode == RESULTCODE) {
			final int code = Integer.valueOf(intent.getStringExtra(CODE)).intValue();
			final String message = intent.getStringExtra(MESSAGE);
		}
		super.onActivityResult(requestCode, resultCode, intent);
	}
 
}
