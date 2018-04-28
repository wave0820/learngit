package com.dada.audios;

import java.io.File;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;

import com.dada.AppActivity;

import android.app.Activity;
import android.content.Context;
import android.media.MediaPlayer;
import android.media.MediaRecorder;
import android.net.Uri;
import android.os.Handler;
import android.util.Log;
import android.os.Build.VERSION;
import android.os.Build.VERSION_CODES;

public class AudioRecordTools{

	private static File mRecAudioFile; // 录制的音频文件
	private static File mRecAudioPath; // 录制的音频文件路徑
	private static MediaRecorder mMediaRecorder;// MediaRecorder对象
	private static String mFileName;
	private static String mFilePath;
	private static Context context;
	
	public native static void nativeRecorderStart();

	
	public static void init(Context _context){
		System.out.println("初始化 AudioRecordTools");
		context = _context;
	}
	
	 //提示权限
    public static void promptAuthority(String filePath)
    {
    	System.out.println("promptAuthority : " + filePath);
    	
		mRecAudioFile = new File(filePath +"Voice1231231242.amr");
    	mMediaRecorder = new MediaRecorder();
    	mMediaRecorder.reset();
		mMediaRecorder.setOutputFile(mRecAudioFile.getAbsolutePath());
    	mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
    	mMediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.RAW_AMR);
    	mMediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
    	mMediaRecorder.setAudioEncodingBitRate(4750);
        try
        {
        	mMediaRecorder.prepare();
        }
        catch (IOException e)
        {
            Log.e("Record", "prepare() failed.promptAuthority:" + e.getMessage());
        }

        mMediaRecorder.release();
        mMediaRecorder = null;
    }
	
	public static void startRecord(String filePath , String fileName) {
		
		if (mMediaRecorder != null) {
			/* ⑤停止录音 */
			mMediaRecorder.stop();
			/* ⑥释放MediaRecorder */
			mMediaRecorder.release();
			mMediaRecorder = null;
		}
		
		mFileName = fileName;
		mFilePath = filePath;
		
		System.out.println("mFileName : " + mFileName);
		System.out.println("mFilePath : " + mFilePath);
		
		try {
			/* ①Initial：实例化MediaRecorder对象 */
			mMediaRecorder = new MediaRecorder();
			/* ②setAudioSource/setVedioSource */
			mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);// 设置麦克风
			/*
			 * ②设置输出文件的格式：THREE_GPP/MPEG-4/RAW_AMR/Default
			 * THREE_GPP(3gp格式，H263视频/ARM音频编码)、MPEG-4、RAW_AMR(
			 * 只支持音频且音频编码要求为AMR_NB)
			 */
			mMediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.RAW_AMR);
			/* ②设置音频文件的编码：AAC/AMR_NB/AMR_MB/Default */
			mMediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
			/* ②设置输出文件的路径 */
			try {
				//mRecAudioFile = File.createTempFile(mFileName, ".amr", mRecAudioPath);
				
				mRecAudioFile = new File(mFilePath + mFileName + ".amr");
				System.out.println(mRecAudioFile.getName() + " 文件不存在...");
				if (!mRecAudioFile.exists()) {
					mRecAudioFile.createNewFile();
				}else{
					mRecAudioFile.delete();
					mRecAudioFile = new File(mFilePath + mFileName + ".amr");
				}
				System.out.println("Start File Name ：" + mRecAudioFile.getName());
			} catch (Exception e) {
				e.printStackTrace();
			}
			mMediaRecorder.setOutputFile(mRecAudioFile.getAbsolutePath());
			/* ③准备 */
			mMediaRecorder.prepare();
			/* ④开始 */
			mMediaRecorder.start();
			
			nativeRecorderStart();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void stopRecord() {
		
		System.out.println("stopRecord...");
		System.out.println(mRecAudioFile.exists());
		if (mRecAudioFile != null) {
			/* ⑤停止录音 */
			mMediaRecorder.stop();
			/* ⑥释放MediaRecorder */
			mMediaRecorder.release();
			mMediaRecorder = null;
			/* 按钮状态 */
			playRecord(mRecAudioFile);
		}
	}

	public static void playChatRecord(String filePath , String fileName){
		System.out.println("播放聊天声音..." + filePath + fileName + ".amr");

		/*
		MediaPlayer mediaPlayer = new MediaPlayer();
		try {
			 mediaPlayer.reset();  
	         mediaPlayer.setDataSource(filePath + fileName + ".amr");  
	         mediaPlayer.prepare();//prepare之后自动播放 
		} catch (Exception e) {
			e.printStackTrace();
		}*/
		
		mRecAudioFile = new File(filePath + fileName + ".amr");
		if(mRecAudioFile.exists()){
			playRecord(mRecAudioFile);
		}else{
			System.out.println("文件不存在 : " + mRecAudioFile.getName());
		}
	}
	
	static MediaPlayer getMediaPlayer(Context context){

	    MediaPlayer mediaplayer = new MediaPlayer();

	    if (android.os.Build.VERSION.SDK_INT < android.os.Build.VERSION_CODES.KITKAT) {
	        return mediaplayer;
	    }

	    try {
	        Class<?> cMediaTimeProvider = Class.forName( "android.media.MediaTimeProvider" );
	        Class<?> cSubtitleController = Class.forName( "android.media.SubtitleController" );
	        Class<?> iSubtitleControllerAnchor = Class.forName( "android.media.SubtitleController$Anchor" );
	        Class<?> iSubtitleControllerListener = Class.forName( "android.media.SubtitleController$Listener" );

	        Constructor constructor = cSubtitleController.getConstructor(new Class[]{Context.class, cMediaTimeProvider, iSubtitleControllerListener});

	        Object subtitleInstance = constructor.newInstance(context, null, null);

	        Field f = cSubtitleController.getDeclaredField("mHandler");

	        f.setAccessible(true);
	        try {
	            f.set(subtitleInstance, new Handler());
	        }
	        catch (IllegalAccessException e) {return mediaplayer;}
	        finally {
	            f.setAccessible(false);
	        }

	        Method setsubtitleanchor = mediaplayer.getClass().getMethod("setSubtitleAnchor", cSubtitleController, iSubtitleControllerAnchor);

	        setsubtitleanchor.invoke(mediaplayer, subtitleInstance, null);
	        //Log.e("", "subtitle is setted :p");
	    } catch (Exception e) {}

	    return mediaplayer;
	}
	
	private static void playRecord(File file) {
	
		MediaPlayer mMediaPlayer = getMediaPlayer(context);

		try {
			FileInputStream fis = new FileInputStream(file);   
			mMediaPlayer.reset();
			mMediaPlayer.setDataSource(fis.getFD());
			mMediaPlayer.prepare();
			mMediaPlayer.start();
			mMediaPlayer.setLooping(false);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private void clearAllFile(String filePath){
		  // 取得指定位置的文件设置显示到播放列表
        File home = new File(filePath);
        if (home.listFiles(new MusicFilter()).length > 0)
        {
            for (File file : home.listFiles(new MusicFilter()))
            {
            	file.delete();
            }
        }
	}
	
}

/* 过滤文件类型 */
class MusicFilter implements FilenameFilter
{
    public boolean accept(File dir, String name)
    {
        return (name.endsWith(".amr"));
    }
}