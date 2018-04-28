package com.mj958.chwh.wxapi;


import org.apache.http.Header;
import org.json.JSONObject;

import cn.magicwindow.Session;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;
import com.tencent.mm.sdk.constants.ConstantsAPI;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.modelmsg.SendAuth;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.widget.FrameLayout;
import android.widget.Toast;
import net.sourceforge.simcpux.Constants;

public class WXEntryActivity extends Activity implements IWXAPIEventHandler{
	FrameLayout m_webLayout;
	private static IWXAPI api;
	
	public native static void nativeInfoWxCode(String headUrl,String unionid,String nickname,String sex);
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        System.out.println("WXEntryActivity onCreate...");
        
        m_webLayout = new FrameLayout(this);
        FrameLayout.LayoutParams lytp = new FrameLayout.LayoutParams(800,640);
        lytp .gravity = Gravity.CENTER;
        addContentView(m_webLayout, lytp);
        
        api = WXAPIFactory.createWXAPI(this, Constants.APP_ID);
        api.registerApp(Constants.APP_ID);
        api.handleIntent(getIntent(), this);
    }
    
	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		setIntent(intent);
        api.handleIntent(intent, this);
	}

	// 瀵邦喕淇婇崣鎴︼拷鐠囬攱鐪伴崚鎵儑娑撳鏌熸惔鏃傛暏閺冭绱濇导姘礀鐠嬪啫鍩岀拠銉︽煙濞夛拷	
	@Override
	public void onReq(BaseReq req) {
		switch (req.getType()) {
		case ConstantsAPI.COMMAND_GETMESSAGE_FROM_WX:
			break;
		case ConstantsAPI.COMMAND_SHOWMESSAGE_FROM_WX:
			break;
		case ConstantsAPI.COMMAND_PAY_BY_WX:
			System.out.println("微信支付成功...");
			break;
		default:
		    break;
	}
	}

	@Override
	public void onResp(BaseResp resp) {
		System.out.println("收到结果！！！！！ : " + resp.errCode);
		System.out.println("WX Resp....");

		switch (resp.errCode) {
		case BaseResp.ErrCode.ERR_OK:
		{
			System.out.println("成功！！！！！");
			
			if (ConstantsAPI.COMMAND_SENDMESSAGE_TO_WX == resp.getType()) {
				Toast.makeText(this, "分享成功", Toast.LENGTH_LONG).show();
				break;
			}
			String code = ((SendAuth.Resp)resp).code;
			String url = String.format("https://api.weixin.qq.com/sns/oauth2/access_token?appid=%s&secret=%s&code=%s&grant_type=authorization_code", Constants.APP_ID,Constants.APP_SECRET,code) ;
			AsyncHttpClient client = new AsyncHttpClient();
			System.out.println("发送登录请求！！！！！");
			client.get(url, new AsyncHttpResponseHandler()
			{
				
				@Override
				public void onFailure(int arg0, Header[] arg1, byte[] arg2,
						Throwable arg3) {
					System.out.println("请求失败！！！！！");
				}

				@Override
				public void onSuccess(int arg0, Header[] arg1, byte[] arg2) {
					
					System.out.println("请求成功！！！！！");
					String ret = new String(arg2);
					try
					{
						System.out.println("解析数据 : " + ret);
						JSONObject jo = new JSONObject(ret);
						String url2 = String.format("https://api.weixin.qq.com/sns/oauth2/refresh_token?appid=%s&grant_type=refresh_token&refresh_token=%s", Constants.APP_ID,jo.get("refresh_token"));
						
						AsyncHttpClient client2 = new AsyncHttpClient();
						client2.get(url2, new AsyncHttpResponseHandler()
						{

							@Override
							public void onFailure(int arg0, Header[] arg1,
									byte[] arg2, Throwable arg3) {
								System.out.println("解析数据失败！！！！！");
							}
								
							@Override
							public void onSuccess(int arg0, Header[] arg1,
									byte[] arg2) {
								
								System.out.println("解析数据成功！！！！！");
								String ret3 = new String(arg2);
								System.out.println("MSG : " + ret3);
								try
								{
									System.out.println("client3 请求！！！！！");
									JSONObject jo3 = new JSONObject(ret3);
									String url3 = String.format("https://api.weixin.qq.com/sns/userinfo?access_token=%s&openid=%s", jo3.get("access_token"),jo3.get("openid"));
									AsyncHttpClient client3 = new AsyncHttpClient();
									client3.get(url3, new AsyncHttpResponseHandler(){

										@Override
										public void onFailure(int arg0,
												Header[] arg1, byte[] arg2,
												Throwable arg3) { 
											
											System.out.println("client3 请求失败！！！！！");
										}

										@Override
										public void onSuccess(int arg0,
												Header[] arg1, byte[] arg2) 
										{
											String retFinal = new String(arg2);
											System.out.println("收到登录数据 : " + retFinal);

											try
											{
												JSONObject joFinal = new JSONObject(retFinal);
												String headUrl = joFinal.getString("headimgurl");
												String nickname = joFinal.getString("nickname");
												String unionid = joFinal.getString("unionid");
												String sex = joFinal.getString("sex");

												nativeInfoWxCode(headUrl,unionid,nickname,sex);
											}
											catch(Exception e)
											{
												e.printStackTrace();
											}
										}
									});
								}
								catch(Exception e)
								{
									e.printStackTrace();
								}
							}
						});
					}
					catch(Exception e)
					{
						e.printStackTrace();
					}
				}
			});
			
		}	
			break;
		case BaseResp.ErrCode.ERR_USER_CANCEL:
			break;
		case BaseResp.ErrCode.ERR_AUTH_DENIED:
			break;
		default:
			break;
		}
		finish();
	}
	private void getRefreshToken(final String code)
	{
		String url = String.format("https://api.weixin.qq.com/sns/oauth2/access_token?appid=%s&secret=%s&code=%s&grant_type=authorization_code", "wxf936efe26bd466fe","7aeea3dd8335c93a63bf21174ddcf586",code) ;
		AsyncHttpClient client = new AsyncHttpClient();
		client.get(url, new AsyncHttpResponseHandler()
		{

			@Override
			public void onFailure(int arg0, Header[] arg1, byte[] arg2,
					Throwable arg3) {
				
			}

			@Override
			public void onSuccess(int arg0, Header[] arg1, byte[] arg2) {
				
				String ret = new String(arg2);
			}
		});
	}
	
	private void getWXInfo(final String freshToken)
	{
		String url = String.format("https://api.weixin.qq.com/sns/oauth2/refresh_token?appid=%s&grant_type=refresh_token&refresh_token=%s", "wxf936efe26bd466fe",freshToken);
		
		AsyncHttpClient client2 = new AsyncHttpClient();
		client2.get(url, new AsyncHttpResponseHandler()
		{

			@Override
			public void onFailure(int arg0, Header[] arg1,
					byte[] arg2, Throwable arg3) {
				
			}
				
			@Override
			public void onSuccess(int arg0, Header[] arg1,
					byte[] arg2) {
				String ret3 = new String(arg2);
				try
				{
					JSONObject jo3 = new JSONObject(ret3);
					String url3 = String.format("https://api.weixin.qq.com/sns/userinfo?access_token=%s&openid=%s", jo3.get("access_token"),jo3.get("openid"));
					AsyncHttpClient client3 = new AsyncHttpClient();
					client3.get(url3, new AsyncHttpResponseHandler(){

						@Override
						public void onFailure(int arg0,
								Header[] arg1, byte[] arg2,
								Throwable arg3) { }

						@Override
						public void onSuccess(int arg0,
								Header[] arg1, byte[] arg2) 
						{
							String retFinal = new String(arg2);
							Log.v("LALALA", retFinal);
							try
							{
								JSONObject joFinal = new JSONObject(retFinal);
							}
							catch(Exception e)
							{
							
							}
						}
					});
				}
				catch(Exception e)
				{
				}
			}
			
		});
		finish();
	}
	
	
	
}
