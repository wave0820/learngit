package com.mj958.chwh.wxapi;


import net.sourceforge.simcpux.Constants;

import com.tencent.mm.sdk.constants.ConstantsAPI;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class WXPayEntryActivity extends Activity implements IWXAPIEventHandler{
	
	private static final String TAG = "MicroMsg.SDKSample.WXPayEntryActivity";
	
	public native static void buyRoomCardCallBack(String errorCode,String errorMsg);

    private IWXAPI api;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        TextView tv = new TextView(this);
        tv.setText("支付结果");
        setContentView(tv);
        
    	api = WXAPIFactory.createWXAPI(this, Constants.APP_ID);
        api.handleIntent(getIntent(), this);
    }

	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		setIntent(intent);
        api.handleIntent(intent, this);
	}

	@Override
	public void onReq(BaseReq req) {
		System.out.println("onReq");

	}

	@Override
	public void onResp(BaseResp resp) {
		 
		Log.d(TAG, "onPayFinish, errCode = " + resp.errCode);

	        String msg = "";
	        
	        if(resp.errCode == 0)
	        {
	            msg = "支付成功";
	        }
	        else if(resp.errCode == -1)
	        {
	            msg = "支付失败";
	        }
	        else if(resp.errCode == -2)
	        {
	            msg = "已取消支付";
	        }
		if (resp.getType() == ConstantsAPI.COMMAND_PAY_BY_WX) {
			System.out.println("微信支付结果 : " + msg);
			 AlertDialog.Builder builder = new AlertDialog.Builder(this);
	         builder.setTitle("提示");
	         builder.setMessage("微信支付结果 : " + String.valueOf(resp.errCode));
	         builder.show();
	         
	         buyRoomCardCallBack(String.valueOf(resp.errCode),msg);
		}
	}
}