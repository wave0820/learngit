package com.dada.tools;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import android.media.MediaPlayer;
import android.media.MediaRecorder;
import android.util.Log;

public class MediaTools {

	public native static void nativeRecorderEnd();
	
	public static final String LOG_TAG = "AudioRecord";

	// 语音文件保存路径
	public static String filePath = null;
	
	public static String fileName = null;
	
	// 上传路径
	public static String UploadPath = null;
	// 下载路径
	public static String DownloadPath = null;

	// 录音时长
	public static long recordTimer;

	// 语音操作对象
	private static MediaPlayer mPlayer = null;
	private static MediaRecorder mRecorder = null;
	//private static File file;

	public static void init(String _filePath) {
		filePath = _filePath;
		//file = new File(filePath);
	}

	// 提示权限
	private static void promptAuthority(String filePath) {
		mRecorder = new MediaRecorder();
		mRecorder.reset();
		mRecorder.setOutputFile(filePath);
		mRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
		mRecorder.setOutputFormat(MediaRecorder.OutputFormat.DEFAULT);
		mRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.DEFAULT);
		mRecorder.setAudioEncodingBitRate(4750);
		try {
			mRecorder.prepare();
		} catch (IOException e) {
			Log.e(LOG_TAG, "prepare() failed.promptAuthority:" + e.getMessage());
		}
		// mRecorder.start();
		// mRecorder.stop();
		mRecorder.release();
		mRecorder = null;
	}

	// 开始录音
	public static void startRecorder(String _filePath , String _fileName) {
		filePath = _filePath;
		fileName = _fileName;

		String file = filePath + fileName + ".amr";

		File m_oFile = new File(file);
		if (m_oFile.exists()) {
			Log.e(LOG_TAG, "文件已存在..." + file);
		}else{
			Log.e(LOG_TAG, "文件不存在..." + file);
			m_oFile.mkdirs();
		}
		
		deleteFile();
		Log.e(LOG_TAG, "startRecorder: " + file);
		mRecorder = new MediaRecorder();
		mRecorder.setOutputFile(file);
		mRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
		mRecorder.setOutputFormat(MediaRecorder.OutputFormat.DEFAULT);
		mRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.DEFAULT);
		mRecorder.setAudioEncodingBitRate(4750);
		try {
			mRecorder.prepare();
		} catch (IOException e) {
			Log.e(LOG_TAG, "prepare() failed.startRecorder:" + file);
		}
		mRecorder.start();
		recordTimer = System.currentTimeMillis();
	}

	// 停止录音
	public static void stopRecorder() {
		Log.e(LOG_TAG, "stopRecorder:");
		if (mRecorder != null) {
			mRecorder.stop();
			mRecorder.release();
		}
		long temp = System.currentTimeMillis() - recordTimer;
		recordTimer = (long) Math.ceil(temp / 1000);

		Log.e(LOG_TAG, "录音时长:" + recordTimer + "秒");

		try {
			String file = filePath + fileName + ".amr";
			FileInputStream fStream = new FileInputStream(file);
			if (fStream.available() <= 0) {
				Log.e(LOG_TAG, "录音失败，请尝试开启录音权限...");
			} else {
				Log.e(LOG_TAG, "录音成功，尝试上传到服务器...:");
				// NetWorkUtils.upload();
				nativeRecorderEnd();
			}
			fStream.close();
		} catch (Exception e) {

		}
	}

	// 开始播放
	public static void startPlaying(String downloadPath) {
		DownloadPath = downloadPath;
		// NetWorkUtils.download();
	}

	// 准备播放
	public static void readyPlaying() {
		try {
			String file = filePath + "/" + fileName;

			mPlayer = new MediaPlayer();
			mPlayer.setDataSource(file);
			mPlayer.prepare();
			mPlayer.start();
		} catch (IOException e) {
			Log.e(LOG_TAG, "播放失败");
		}
	}

	// 停止播放
	public static void stopPlaying() {
		if (mPlayer != null) {
			mPlayer.stop();
			mPlayer.release();
			mPlayer = null;
		}
	}

	// 删除文件
	private static void deleteFile() {
		/*
		if (file.exists() == true)// 判断文件是否存在
		{
			if (file.isFile() == true)// 判断是否是文件
			{
				file.delete();
			}
		}*/
	}
}
