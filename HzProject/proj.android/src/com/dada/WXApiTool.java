package com.dada;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.StringReader;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Random;

import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;
import org.json.JSONObject;
import org.xmlpull.v1.XmlPullParser;

import com.tencent.mm.sdk.modelmsg.SendAuth;
import com.tencent.mm.sdk.modelmsg.SendMessageToWX;
import com.tencent.mm.sdk.modelmsg.WXAppExtendObject;
import com.tencent.mm.sdk.modelmsg.WXImageObject;
import com.tencent.mm.sdk.modelmsg.WXMediaMessage;
import com.tencent.mm.sdk.modelmsg.WXTextObject;
import com.tencent.mm.sdk.modelmsg.WXWebpageObject;
import com.tencent.mm.sdk.modelpay.PayReq;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

import android.app.Activity;
import android.app.ProgressDialog;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.os.AsyncTask;
import android.util.Log;
import android.util.Xml;
import net.sourceforge.simcpux.Constants;
import net.sourceforge.simcpux.MD5;
import net.sourceforge.simcpux.Util;

public class WXApiTool {
	private static final String TAG = "WXApiTool";
	private static IWXAPI api;
	private static Activity target;
	private static PayReq req;
	private static Map<String,String> resultunifiedorder;
	private static StringBuffer sb;
	
	public static void init(Activity _target) {
		target = _target;
		api = WXAPIFactory.createWXAPI(target, Constants.APP_ID);
		api.registerApp(Constants.APP_ID);
		System.out.println("注册应用 ：" + Constants.APP_ID);
	}

	public static void share(final String url, final String title, final String description, Bitmap thumb) {
		WXWebpageObject webpage = new WXWebpageObject();
		webpage.webpageUrl = url;
		WXMediaMessage msg = new WXMediaMessage(webpage);
		msg.title = title;
		msg.description = description;
		msg.setThumbImage(thumb);
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = String.valueOf(System.currentTimeMillis());
		req.message = msg;
		req.scene = 1;
		api.sendReq(req);
	}

	public static void share2Friend() {
		
	}
	
	private static Bitmap compressImage(Bitmap image) {  
		  
        ByteArrayOutputStream baos = new ByteArrayOutputStream();  
        image.compress(Bitmap.CompressFormat.JPEG, 100, baos);//质量压缩方法，这里100表示不压缩，把压缩后的数据存放到baos中  
        int options = 100;  
        while ( baos.toByteArray().length / 1024>100) {  //循环判断如果压缩后图片是否大于100kb,大于继续压缩         
            baos.reset();//重置baos即清空baos  
            image.compress(Bitmap.CompressFormat.JPEG, options, baos);//这里压缩options%，把压缩后的数据存放到baos中  
            options -= 10;//每次都减少10  
        }  
        ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());//把压缩后的数据baos存放到ByteArrayInputStream中  
        Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, null);//把ByteArrayInputStream数据生成图片  
        return bitmap;  
    }  
	
	private static Bitmap getimage(String srcPath) {  
        BitmapFactory.Options newOpts = new BitmapFactory.Options();  
        //开始读入图片，此时把options.inJustDecodeBounds 设回true了  
        newOpts.inJustDecodeBounds = true;  
        Bitmap bitmap = BitmapFactory.decodeFile(srcPath,newOpts);//此时返回bm为空  
          
        newOpts.inJustDecodeBounds = false;  
        int w = newOpts.outWidth;  
        int h = newOpts.outHeight;  
        //现在主流手机比较多是800*480分辨率，所以高和宽我们设置为  
        float hh = 300f;//这里设置高度为800f  
        float ww = 160f;//这里设置宽度为480f  
        //缩放比。由于是固定比例缩放，只用高或者宽其中一个数据进行计算即可  
        int be = 1;//be=1表示不缩放  
        if (w > h && w > ww) {//如果宽度大的话根据宽度固定大小缩放  
            be = (int) (newOpts.outWidth / ww);  
        } else if (w < h && h > hh) {//如果高度高的话根据宽度固定大小缩放  
            be = (int) (newOpts.outHeight / hh);  
        }  
        if (be <= 0)  
            be = 1;  
        newOpts.inSampleSize = be;//设置缩放比例  
        //重新读入图片，注意此时已经把options.inJustDecodeBounds 设回false了  
        bitmap = BitmapFactory.decodeFile(srcPath, newOpts);  
        return compressImage(bitmap);//压缩好比例大小后再进行质量压缩  
    }  

	public static void login() {
		SendAuth.Req req = new SendAuth.Req();
		req.scope = "snsapi_userinfo";
		req.state = "wechat_sdk_demo_test";
		api.sendReq(req);
	}

	//分享图片到朋友圈
	public static void shareImg2Timeline(final String title, final String description, final String imagePath, Bitmap thumb) {
		WXImageObject imgObj = new WXImageObject();
		imgObj.setImagePath(imagePath);
	
		WXMediaMessage msg = new WXMediaMessage();
		msg.mediaObject = imgObj;
		msg.description = description;
		msg.setThumbImage(thumb);
		// Bitmap bmp = BitmapFactory.decodeFile(imagePath);
		// Bitmap thumbBmp = Bitmap.createScaledBitmap(bmp,true);
		// bmp.recycle();
		// msg.thumbData = Util.bmpToByteArray(thumbBmp, true);
		msg.title = title;
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = "img" + String.valueOf(System.currentTimeMillis());
		req.message = msg;
		req.scene = SendMessageToWX.Req.WXSceneTimeline;
		api.sendReq(req);
	}
	
	//向朋友发送图片
	public static void shareImg2Friend(final String title, final String description, final String imagePath, Bitmap thumb) {
		
		System.out.println("shareImg2Friend...");

		WXImageObject imgObj = new WXImageObject();
		imgObj.setImagePath(imagePath);

		WXMediaMessage msg = new WXMediaMessage();
		msg.mediaObject = imgObj;
		msg.description = description;
		//msg.setThumbImage(thumb);
		//Bitmap bmp = BitmapFactory.decodeFile(imagePath);
		//Bitmap thumbBmp = Bitmap.createScaledBitmap(bmp, 300, 167, true);
		//bmp.recycle();
		//msg.thumbData = Util.bmpToByteArray(thumbBmp, true);
		msg.setThumbImage(getimage(imagePath));
		msg.title = title;
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = "img" + String.valueOf(System.currentTimeMillis());
		req.message = msg;
		req.scene = SendMessageToWX.Req.WXSceneSession;
		api.sendReq(req);
	}
	
	
	
	public static void shareLinkToFriend(final String url,final String title, final String description, Bitmap thumb) {
		System.out.println("shareLinkToFriend...");
		WXWebpageObject webpage = new WXWebpageObject();
		webpage.webpageUrl = url;
		WXMediaMessage msg = new WXMediaMessage(webpage);
		msg.title = title;
		msg.description = description;
		msg.setThumbImage(thumb);
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = String.valueOf(System.currentTimeMillis());
		req.message = msg;
		req.scene = SendMessageToWX.Req.WXSceneSession;
		api.sendReq(req);
	}
	
public static void shareLinkToLine(final String title, final String description, Bitmap thumb) {
		
		WXWebpageObject webpage = new WXWebpageObject();
		webpage.webpageUrl = "https://a.mlinks.cc/AcAZ";
		WXMediaMessage msg = new WXMediaMessage(webpage);
		msg.title = title;
		msg.description = description;
		msg.setThumbImage(thumb);
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = String.valueOf(System.currentTimeMillis());
		req.message = msg;
		req.scene = SendMessageToWX.Req.WXSceneTimeline;
		api.sendReq(req);
	}
	

	public static void wxPay(int price) {
		WxPay pay = new WxPay(api);
		pay.request2WxPay(price);
	}
}
