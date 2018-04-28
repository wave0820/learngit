package com.dada.tools;

import java.io.File;

import android.media.MediaRecorder;
import android.os.Environment;

public class AudioFileFunc {
	 //音频输入-麦克风
    public final static int AUDIO_INPUT = MediaRecorder.AudioSource.MIC;
     
    //采用频率
    //44100是目前的标准，但是某些设备仍然支持22050，16000，11025
    public final static int AUDIO_SAMPLE_RATE = 44100;  //44.1KHz,普遍使用的频率   
   
    //录音输出文件
    private final static String AUDIO_RAW_FILENAME = "RawAudio.pcm";
    private final static String AUDIO_WAV_FILENAME = "FinalAudio.wav";
    public final static String AUDIO_AMR_FILENAME = "FinalAudio.amr";
    public final static String AUDIO_AAC_FILENAME = "FinalAudio.aac";
     
    /**
     * 判断是否有外部存储设备sdcard
     * @return true | false
     */
    public static boolean isSdcardExit(){       
        if (Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED))
            return true;
        else
            return false;
    }
         
    /**
     * 获取麦克风输入的原始音频流文件路径
     * @return
     */
    public static String getRawFilePath(){
        String mAudioRawPath = "";
        if(isSdcardExit()){
//            String fileBasePath =;
            mAudioRawPath =  Environment.getExternalStorageDirectory()
            		+"/sabine/"+AUDIO_RAW_FILENAME;
        }   
         
        return mAudioRawPath;
    }
     
    /**
     * 获取编码后的WAV格式音频文件路径
     * @return
     */
    public static String getWavFilePath(){
        String mAudioWavPath = "";
        if(isSdcardExit()){
        	mAudioWavPath =  Environment.getExternalStorageDirectory()+"/sabine/"+AUDIO_WAV_FILENAME;
        }
        return mAudioWavPath;
    }
     
     
    /**
     * 获取编码后的AMR格式音频文件路径
     * @return
     */
    public static String getAMRFilePath(){
        String mAudioAMRPath = "";
        if(isSdcardExit()){
//            String fileBasePath = Environment.getExternalStorageDirectory().getAbsolutePath();
            mAudioAMRPath =  Environment.getExternalStorageDirectory()+"/sabine/"+AUDIO_AMR_FILENAME;
        }
        return mAudioAMRPath;
    }   
    /**
     * 获取编码后的AMR格式音频文件路径
     * @return
     */
    public static String getAACFilePath(){
        String mAudioAACPath = "";
        if(isSdcardExit()){
//            String fileBasePath = Environment.getExternalStorageDirectory().getAbsolutePath();
            mAudioAACPath =  Environment.getExternalStorageDirectory()+"/sabine/"+AUDIO_AAC_FILENAME;
        }
        return mAudioAACPath;
    }   
     
     
    /**
     * 获取文件大小
     * @param path,文件的绝对路径
     * @return
     */
    public static long getFileSize(String path){
        File mFile = new File(path);
        if(!mFile.exists())
            return -1;
        return mFile.length();
    }
 

}
