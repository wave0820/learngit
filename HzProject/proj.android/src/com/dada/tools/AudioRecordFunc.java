package com.dada.tools;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.channels.FileChannel;


import android.media.AudioFormat;
import android.media.AudioRecord;
import android.util.Log;

public class AudioRecordFunc {
	public native static void nativeRecorderEnd();
	
	public static final String LOG_TAG = "AudioRecord";
	// 缓冲区字节大小
	private static int bufferSizeInBytes = 0;

	// AudioName裸音频数据文件 ，麦克风
	private static String AudioName = "";

	// NewAudioName可播放的音频文件
	private static String NewAudioName = "";

	private static AudioRecord audioRecord;
	private static boolean isRecord = false;// 设置正在录制的状态
	private static Thread recThread = null;// 录音线程

	public static void startRecordAndFile(String _filePath, String _fileName) {
		Log.e(LOG_TAG, "startRecordAndFile...");

		if (isRecord) {
			Log.e(LOG_TAG, "is recording...");
			return;
		}

		//"/mnt/sdcard/"
		AudioName = _filePath + _fileName + ".pcm";
		NewAudioName = _filePath + _fileName + ".wav";

		Log.d(LOG_TAG, " inFilename :" + AudioName);
		Log.d(LOG_TAG, " outFilename :" + NewAudioName);
		
		File file1 = new File(AudioName);
		File file2 = new File(NewAudioName);
		if (!file1.exists()) {
			Log.e(LOG_TAG, "file1 not exit");
			try {
				file1.createNewFile();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
		if (!file2.exists()) {
			Log.e(LOG_TAG, "file2 not exit");

			try {
				file2.createNewFile();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
		Log.e(LOG_TAG, "创建后  file2 exit ：" + file2.exists());
		
		if (audioRecord == null)
			creatAudioRecord();

		audioRecord.startRecording();
		// 让录制状态为true
		isRecord = true;
		// 开启音频文件写入线程
		// new Thread(new AudioRecordThread()).start();
		
		if (recThread == null) {
			recThread = new Thread(new Runnable() {
				public void run() {
					if (isRecord) {
						writeDateTOFile();// 往文件中写入裸数据
						copyWaveFile(AudioName, NewAudioName);// 给裸数据加上头文件
					}
				}
			}, "AudioRecorder Thread");
			recThread.start();
		}
	}
	

	public static void stopRecordAndFile() {
		Log.e(LOG_TAG, "stopRecordAndFile...");
		close();
	}

	public long getRecordFileSize() {
		return AudioFileFunc.getFileSize(NewAudioName);
	}

	private static void close() {
		if (audioRecord != null) {
			Log.d(LOG_TAG,"录完结束，删除资源...");
			audioRecord.stop();
			audioRecord.release();// 释放资源
			audioRecord = null;
		}
	
		isRecord = false;// 停止文件写入
		
		File file = new File(NewAudioName);
		if (file.exists()) {
			Log.d(LOG_TAG, NewAudioName + "目录存在..");
			FileInputStream fis;
			try {
				fis = new FileInputStream(file);
				FileChannel fc= fis.getChannel();  
				Log.d(LOG_TAG, NewAudioName + "文件大小 : "+ fc.size());
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}  
			

		}else{
			Log.d(LOG_TAG, NewAudioName + "目录不存在..");
		}
	}

	private static void creatAudioRecord() {
		Log.e(LOG_TAG, "creatAudioRecord...");

		// 获得缓冲区字节大小
		bufferSizeInBytes = AudioRecord.getMinBufferSize(AudioFileFunc.AUDIO_SAMPLE_RATE, AudioFormat.CHANNEL_IN_STEREO,
				AudioFormat.ENCODING_PCM_16BIT);

		// 创建AudioRecord对象
		audioRecord = new AudioRecord(AudioFileFunc.AUDIO_INPUT, AudioFileFunc.AUDIO_SAMPLE_RATE,
				AudioFormat.CHANNEL_IN_STEREO, AudioFormat.ENCODING_PCM_16BIT, bufferSizeInBytes);
	}

	/**
	 * 这里将数据写入文件，但是并不能播放，因为AudioRecord获得的音频是原始的裸音频，
	 * 如果需要播放就必须加入一些格式或者编码的头信息。但是这样的好处就是你可以对音频的 裸数据进行处理，比如你要做一个爱说话的TOM
	 * 猫在这里就进行音频的处理，然后重新封装 所以说这样得到的音频比较容易做一些音频的处理。
	 */
	private static void writeDateTOFile() {
		// new一个byte数组用来存一些字节数据，大小为缓冲区大小
		byte[] audiodata = new byte[bufferSizeInBytes];
		FileOutputStream fos = null;
		int readsize = 0;
		try {
			File file = new File(AudioName);
			if (file.exists()) {
				file.delete();
			}
			fos = new FileOutputStream(file);// 建立一个可存取字节的文件

		} catch (Exception e) {
			e.printStackTrace();
		}
		while (isRecord == true) {
			readsize = audioRecord.read(audiodata, 0, bufferSizeInBytes);
			if (AudioRecord.ERROR_INVALID_OPERATION != readsize && fos != null) {
				try {
					Log.d(LOG_TAG, "file write : " + audiodata);
					fos.write(audiodata);
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		try {
			if (fos != null)
				fos.close();// 关闭写入流
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	// 这里得到可播放的音频文件
	private static void copyWaveFile(String inFilename, String outFilename) {
		Log.d(LOG_TAG, " inFilename :" + inFilename);
		Log.d(LOG_TAG, " outFilename :" + outFilename);
	
		if (!isRecord) {
			Log.e(LOG_TAG, "is not Record");
			return;
		}
		
		FileInputStream in = null;
		FileOutputStream out = null;
		long totalAudioLen = 0;
		long totalDataLen = totalAudioLen + 36;
		long longSampleRate = AudioFileFunc.AUDIO_SAMPLE_RATE;
		int channels = 2;
		long byteRate = 16 * AudioFileFunc.AUDIO_SAMPLE_RATE * channels / 8;
		byte[] data = new byte[bufferSizeInBytes];
		try {
			in = new FileInputStream(inFilename);
			out = new FileOutputStream(outFilename);
			totalAudioLen = in.getChannel().size();
			totalDataLen = totalAudioLen + 36;

			WriteWaveFileHeader(out, totalAudioLen, totalDataLen, longSampleRate, channels, byteRate);
			while (in.read(data) != -1) {
				out.write(data);
			}
			
			out.flush();
			
			File testF = new File(outFilename);
			FileInputStream tfis = new FileInputStream(testF);
			FileChannel fc= tfis.getChannel();
			Log.d("FileSize", NewAudioName + "文件大小 : "+ fc.size());
			
			in.close();
			out.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * 这里提供一个头信息。插入这些信息就可以得到可以播放的文件。 为我为啥插入这44个字节，这个还真没深入研究，不过你随便打开一个wav
	 * 音频的文件，可以发现前面的头文件可以说基本一样哦。每种格式的文件都有 自己特有的头文件。
	 */
	private static void WriteWaveFileHeader(FileOutputStream out, long totalAudioLen, long totalDataLen,
			long longSampleRate, int channels, long byteRate) throws IOException {
		byte[] header = new byte[44];
		header[0] = 'R'; // RIFF/WAVE header
		header[1] = 'I';
		header[2] = 'F';
		header[3] = 'F';
		header[4] = (byte) (totalDataLen & 0xff);
		header[5] = (byte) ((totalDataLen >> 8) & 0xff);
		header[6] = (byte) ((totalDataLen >> 16) & 0xff);
		header[7] = (byte) ((totalDataLen >> 24) & 0xff);
		header[8] = 'W';
		header[9] = 'A';
		header[10] = 'V';
		header[11] = 'E';
		header[12] = 'f'; // 'fmt ' chunk
		header[13] = 'm';
		header[14] = 't';
		header[15] = ' ';
		header[16] = 16; // 4 bytes: size of 'fmt ' chunk
		header[17] = 0;
		header[18] = 0;
		header[19] = 0;
		header[20] = 1; // format = 1
		header[21] = 0;
		header[22] = (byte) channels;
		header[23] = 0;
		header[24] = (byte) (longSampleRate & 0xff);
		header[25] = (byte) ((longSampleRate >> 8) & 0xff);
		header[26] = (byte) ((longSampleRate >> 16) & 0xff);
		header[27] = (byte) ((longSampleRate >> 24) & 0xff);
		header[28] = (byte) (byteRate & 0xff);
		header[29] = (byte) ((byteRate >> 8) & 0xff);
		header[30] = (byte) ((byteRate >> 16) & 0xff);
		header[31] = (byte) ((byteRate >> 24) & 0xff);
		header[32] = (byte) (2 * 16 / 8); // block align
		header[33] = 0;
		header[34] = 16; // bits per sample
		header[35] = 0;
		header[36] = 'd';
		header[37] = 'a';
		header[38] = 't';
		header[39] = 'a';
		header[40] = (byte) (totalAudioLen & 0xff);
		header[41] = (byte) ((totalAudioLen >> 8) & 0xff);
		header[42] = (byte) ((totalAudioLen >> 16) & 0xff);
		header[43] = (byte) ((totalAudioLen >> 24) & 0xff);
		out.write(header, 0, 44);
		out.flush();
	}
}
