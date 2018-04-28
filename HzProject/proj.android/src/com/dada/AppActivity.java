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
package com.dada;

//import com.fanwei.sdk.api.PaySdk;
import java.util.Map;
import java.util.concurrent.TimeUnit;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import cn.magicwindow.MLink;
import cn.magicwindow.MLinkAPIFactory;
import cn.magicwindow.MWConfiguration;
import cn.magicwindow.MagicWindowSDK;
import cn.magicwindow.Session;
import cn.magicwindow.common.base.MWActivity;
import cn.magicwindow.mlink.MLinkCallback;
import cn.magicwindow.mlink.MLinkIntentBuilder;

import com.dada.audios.AudioRecordTools;
import com.dada.tools.Utils;
import com.fanwei.jubaosdk.common.util.SdkConst;
import com.fanwei.jubaosdk.shell.FWPay;
import com.fanwei.jubaosdk.shell.PayOrder;
import com.mj958.chwh.R;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.database.Observable;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Bundle;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.view.Gravity;
import android.widget.FrameLayout;
import android.widget.Toast;
import net.sourceforge.simcpux.Constants;

public class AppActivity extends Cocos2dxActivity {

	public static AppActivity instance = null;

	public native static void SystemInfoNative(int type, int value);

	FrameLayout m_webLayout;
	static String g_orderStr = null;
	private static final int REQUEST_CODE_GET = 90;
	private final static String CODE = "code";
	private final static String MESSAGE = "message";

	public native static void sendPayInfor(String playerID, String payID, int mount);

	public native static void payResult(int InfoID, String payID, int num);
	
	public native static void  getReturnRoomId(String roomId);

	private static int payMon = 0;
	private static Bitmap thumb;

	private BatteryReceiver batteryReceiver;

	private NetTypeReceiver mNetworkReceiver;
	WakeLock m_wklk;
	private static boolean m_bOpenListensSystemInfo = false;
	
	private static boolean m_iRegisterMWLink = false;
	

	@Override
	protected void onCreate(Bundle saveInstanceState) {

		System.out.println("AppActivity  onCreate...");

		super.onCreate(saveInstanceState);
		m_webLayout = new FrameLayout(this);
		FrameLayout.LayoutParams lytp = new FrameLayout.LayoutParams(800, 640);
		lytp.gravity = Gravity.CENTER;
		addContentView(m_webLayout, lytp);
		instance = this;

		AudioRecordTools.init(this);
		WXApiTool.init(AppActivity.this);
		//FWPay.initialize(this, true);

		thumb = BitmapFactory.decodeResource(this.getContext().getResources(), R.drawable.icon);

		System.out.println("onCreate.....");
		PowerManager pm = (PowerManager) getSystemService(POWER_SERVICE);
		m_wklk = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "cn");
		m_wklk.acquire(); // 设置保持唤醒
		initMWSDK();
		//MLinkAPIFactory.createAPI(this).deferredRouter();
		//MLinkAPIFactory.createAPI(this).registerWithAnnotation(this);
		 //注册SDK
	    registerLinks(this);
	    Uri mLink = getIntent().getData();
	    //如果从浏览器传来 则进行路由操作
	    if(mLink != null){
	      MLink.getInstance(this).router(this, mLink);
	      finish();
	    }

	}

	private class NetTypeReceiver extends BroadcastReceiver {
		@Override
		public void onReceive(Context context, Intent intent) {
			String action = intent.getAction();
			if (action.equals(ConnectivityManager.CONNECTIVITY_ACTION)) {
				ConnectivityManager connectivityManager = (ConnectivityManager)

				getSystemService(Context.CONNECTIVITY_SERVICE);
				NetworkInfo info = connectivityManager.getActiveNetworkInfo();
				int NetworkType = -1;
				if (info != null && info.isAvailable()) {
					String name = info.getTypeName();
					if (name.equals("WIFI")) {
						NetworkType = 1;
					} else {
						NetworkType = 4;
					}
					System.out.println("当前网络名称：" + name);
				} else {
					NetworkType = 0;
					System.out.println("没有可用网络");
				}

				SystemInfoNative(2, NetworkType);
				/*
				 * int NetworkType = Utils.GetNetworkType(context); if
				 * (m_bOpenListensSystemInfo) { SystemInfoNative(2,
				 * NetworkType); }
				 */
			}
		}
	}
	
	/**
	 * 广播接受者
	 */
	class BatteryReceiver extends BroadcastReceiver {

		@Override
		public void onReceive(Context context, Intent intent) {
			// TODO Auto-generated method stub
			// 判断它是否是为电量变化的Broadcast Action
			if (Intent.ACTION_BATTERY_CHANGED.equals(intent.getAction())) {
				System.out.println("监听到电池电量状态变化...");
				// 获取当前电量
				int level = intent.getIntExtra("level", 0);
				// 电量的总刻度
				int scale = intent.getIntExtra("scale", 100);
				// 把它转成百分比

				if (m_bOpenListensSystemInfo) {
					SystemInfoNative(1, ((level * 100) / scale));
				}
				System.out.println("电池电量为" + ((level * 100) / scale) + "%");
			}
		}

	}

	///////////////////////////////////
	// public static Activity activity;

	public static void openListensSystemInfo() {
		m_bOpenListensSystemInfo = true;

		IntentFilter netIntentFilter = new IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION);
		instance.mNetworkReceiver = instance.new NetTypeReceiver();
		instance.registerReceiver(instance.mNetworkReceiver, netIntentFilter);

		// 注册广播接受者java代码
		IntentFilter intentFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
		// 创建广播接受者对象
		instance.batteryReceiver = instance.new BatteryReceiver();
		// 注册receiver
		instance.registerReceiver(instance.batteryReceiver, intentFilter);
	}

	public static void stopListensSystemInfo() {
		m_bOpenListensSystemInfo = false;

		if (instance.batteryReceiver != null) {
			instance.unregisterReceiver(instance.batteryReceiver);
			instance.batteryReceiver = null;
		}

		if (instance.mNetworkReceiver != null) {
			instance.unregisterReceiver(instance.mNetworkReceiver);
			instance.mNetworkReceiver = null;
		}
	}

	public static void loginWeixin() {
		System.out.println("进入登录回调！！！！！");
		WXApiTool.login();
	}

	public static void shareImg() {

		System.out.println("进入图片回调！！！！！");
		// savePath.Format("%s\\CustomFace\\%d.png",CBcfFile::GetAppPath(),msg->dwUserID);

		WXApiTool.shareImg2Timeline(Constants.APP_NAME, Constants.APP_NAME, "/sdcard/share.png", thumb);

	}

	public static void shareImg2Timeline(String filePath) {
		WXApiTool.shareImg2Timeline(Constants.APP_NAME, Constants.APP_NAME, filePath, thumb);
	}

	public static void shareImg2Friend(String filePath) {
		WXApiTool.shareImg2Friend(Constants.APP_NAME, Constants.APP_NAME, filePath, thumb);
	}

	public static void shareLink2Friend(String roomNum, String zmCount,String roomCountNum) {
		System.out.println("分享内容给朋友");
		String title = new StringBuffer().append(Constants.APP_NAME).append(" 【房号:").append(roomNum).append("】")
				.toString();

		String zmMsg = " 房间抓" + zmCount + "个鸟";
		if (zmCount.equals("1")) {
			zmMsg = " 房间一码全中";
		}
		String url=new StringBuffer().append("https://a.mlinks.cc/AcAZ?roomId=").append(roomNum).toString();
		String content = new StringBuffer().append("我在").append(Constants.APP_NAME).append("玩牌,房号:").append(roomNum)
				.append(zmMsg).append("，快来和我一起玩吧！").toString();
		WXApiTool.shareLinkToFriend(url,title, content, thumb);
	}

	public static void shareLinkToTimeLine(String roomNum, String zmCount) {
		System.out.println("分享内容给朋友");
		String title = new StringBuffer().append(Constants.APP_NAME).append(" 【房号:").append(roomNum).append("】")
				.toString();
		String zmMsg = " ，房间抓" + zmCount + "个鸟";
		if (zmCount.equals("1")) {
			zmMsg = " ，房间一码全中";
		}
		String content = new StringBuffer().append("我在").append(Constants.APP_NAME).append("玩牌,房号:").append(roomNum)
				.append(zmMsg).append("，快来和我一起玩吧！").toString();
		//WXApiTool.shareLinkToFriend(title, content, thumb);
	}

	public static void onPay(int price) {
		System.out.println("请求支付...");
		WXApiTool.wxPay(price);
	}

	protected void onPause() {
		Session.onPause(this);
		m_iRegisterMWLink=true;
		super.onPause();
		// 程序暂停运行于后台时调用的方法，在这里加入下边的移除不锁屏功能代码。
		if (m_wklk != null) {
			m_wklk.release();
			m_wklk = null;
		}
	}

	protected void onResume() {
		Session.onResume(this);
		//registerLinks(this);
		super.onResume();
		// 程序由后台转入前台的时候调用的方法onResume()中加入下边的关闭锁屏代码
		if (m_wklk == null) {
			PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
			m_wklk = pm.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK, "XYTEST");
			m_wklk.acquire();
		}
	}

	public static void doPay(int mon) {
		payMon = mon;
		g_orderStr = String.valueOf(System.currentTimeMillis());
		PayOrder order = new PayOrder()
				// AppId(必需)
				.setAppId("2593732002")
				// 金额(必需)
				.setAmount(String.valueOf(mon))
				// 商品名称(必需)
				.setGoodsName("购买商品")
				// 商户订单号(必需)
				.setPayId(String.valueOf(g_orderStr))
				// 玩家Id(必需)
				.setPlayerId("16060335582088867190");
		sendPayInfor("16060335582088867190", g_orderStr, mon);

		System.out.println("instance : " + instance);
		FWPay.pay(instance, order);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
		if (requestCode == SdkConst.REQUESTCODE && resultCode == SdkConst.RESULTCODE) {
			payResult(payMon, g_orderStr, resultCode);
			showToast("[" + intent.getStringExtra("message") + "]");
		}
		super.onActivityResult(requestCode, 0, intent);
	}

	private void showToast(String message) {
		Toast.makeText(instance, message, Toast.LENGTH_LONG).show();
	}

	public void onDestroy() {
		// 取消设置
		super.onDestroy();
		if (m_wklk == null) {
			m_wklk.release();
		}

		if (batteryReceiver != null) {
			unregisterReceiver(batteryReceiver);
			batteryReceiver = null;
		}

		if (mNetworkReceiver != null) {
			unregisterReceiver(mNetworkReceiver);
			mNetworkReceiver = null;
		}
	}

	public static Object gotoActivity() {

		return null;
	}
	
	
	private void initMWSDK()
	{
		MWConfiguration config = new MWConfiguration(this);
		// //
		config.setDebugModel(true)//打开魔窗 Log 信息
		.setPageTrackWithFragment(true)//是否统计 Fragment 页面 .setCustomWebViewTitleBarOn()//代码内定义 webview 标题
		.setSharePlatform(MWConfiguration.ORIGINAL); 
		MagicWindowSDK.initSDK(config);
	}
	private void registerLinks(Context context) {
	    MLink.getInstance(context).registerDefault(new MLinkCallback() {
	      @Override
	      public void execute(Map paramMap, Uri uri, Context context) {
	    	  System.out.println("11111111111111111111111");
	        //默认的路由 如果没有匹配则转跳到 MainActivity 为你的首页
	        MLinkIntentBuilder.buildIntent(paramMap, context,   AppActivity.class);
	      }
	    });
	    // testKey:  mLink 的 key, mLink的唯一标识,用于进行路由操作
	    MLink.getInstance(context).register("HTGAME", new MLinkCallback() {
	      public void execute(Map paramMap, Uri uri, Context context) {
	    	  showToast("[" + paramMap.get("roomId") + "]");
	    	  getReturnRoomId(paramMap.get("roomId") .toString());
	    	  if(!m_iRegisterMWLink)
	    	  {
	    		  m_iRegisterMWLink=true;
	    		  MLinkIntentBuilder.buildIntent(paramMap, context,   AppActivity.class);
	    	  }
	        //!!!!!!!!注意 此处有坑,如果你的SplashActivity转跳有延迟，那么在此处转跳的延迟必须大于前者转跳时间！！！！！！！！
//	        Observable.timer(1050,TimeUnit.MILLISECONDS)
//	            .observeOn(AndroidSchedulers.mainThread())
//	            .subscribe(aVoid->{
//	              MLinkIntentBuilder.buildIntent(paramMap, context, DetailsActivity.class);
//
//	            });

	      }
	    });
	  }

}
