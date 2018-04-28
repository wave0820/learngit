#include "YZAudioEngine.h"
#include <SimpleAudioEngine.h>

namespace YZ
{
	static YZAudioEngine* instant = nullptr;

	YZAudioEngine::YZAudioEngine()
	{
        _currentBackgroundName = "";
        _lastBackgroundName ="";
        _switchOfMusic = UserDefault::getInstance()->getBoolForKey("MusicSwitch", true);
        _switchOfEffect = UserDefault::getInstance()->getBoolForKey("EffectSwitch", true);

        _musicVolume = UserDefault::getInstance()->getFloatForKey("MusicVolume" , 0.5f);
        _effectVolume = UserDefault::getInstance()->getFloatForKey("EffectVolume" , 0.5f);
        
        _soundLanguage = UserDefault::getInstance()->getIntegerForKey("SoundLanguage", 1);
        
        setBackgroundMusicVolume(_musicVolume);
        setEffectsVolume(_effectVolume);
	}

	YZAudioEngine* YZAudioEngine::getInstance()
	{
		if (nullptr == instant)
		{
			instant = new YZAudioEngine();
		}
		return instant;
	}

	void YZAudioEngine::setSwitchOfMusic(bool isOpen)
	{
		_switchOfMusic = isOpen;
        UserDefault::getInstance()->setBoolForKey("MusicSwitch", isOpen);
        if (_switchOfMusic) {
            if (0 != _lastBackgroundName.compare(""))
            {
                playBackgroundMusic(_lastBackgroundName.c_str() , true);
            }
        }else{
            this->stopBackgroundMusic();
        }
	}

	bool YZAudioEngine::getSwitchOfMusic() const
	{
		return _switchOfMusic;
	}

	void YZAudioEngine::setSwitcjofEffect(bool isOpen)
	{
		_switchOfEffect = isOpen;
        UserDefault::getInstance()->setBoolForKey("EffectSwitch", isOpen);
	}

	bool YZAudioEngine::getSwitcjofEffect() const
	{
		return _switchOfEffect;
	}

	void YZAudioEngine::playBackgroundMusic(const char* pszFilePath, bool bLoop)
	{
       // _switchOfMusic = false;
        
        if (_switchOfMusic)
		{
			if (nullptr != pszFilePath)
			{
				if (0 != _currentBackgroundName.compare(pszFilePath))
				{
					_currentBackgroundName = pszFilePath;
                    _lastBackgroundName = pszFilePath;
					CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(pszFilePath, bLoop);
				}
			}
        }else{
            _lastBackgroundName = pszFilePath;
        }
	}

	void YZAudioEngine::stopBackgroundMusic(bool bReleaseData)
	{
        _lastBackgroundName = _currentBackgroundName;
        _currentBackgroundName = "";
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(bReleaseData);
	}

	void YZAudioEngine::pauseBackgroundMusic()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}

	void YZAudioEngine::resumeBackgroundMusic()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}

	unsigned int YZAudioEngine::playEffect(const char* pszFilePath, bool bLoop, float pitch, float pan, float gain)
	{
       // _switchOfEffect = false;
        
		if (_switchOfEffect)
		{
			//return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(pszFilePath, bLoop, pitch, pan, gain);
            return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(pszFilePath);
		}
		return 0;
	}

	void YZAudioEngine::setEffectsVolume(float volume)
	{
        _effectVolume = volume;
        UserDefault::getInstance()->setFloatForKey("EffectVolume", _effectVolume);
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(_effectVolume);
	}

	float YZAudioEngine::getEffectsVolume() const
	{
		return _effectVolume;
	}

	void YZAudioEngine::setBackgroundMusicVolume(float volume)
	{
        _musicVolume = volume;
        UserDefault::getInstance()->setFloatForKey("MusicVolume", _musicVolume);
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(_musicVolume);
	}

	float YZAudioEngine::getBackgroundMusicVolume() const
	{
		return _musicVolume;
	}
    
    void YZAudioEngine::setSwitchSoundLanguage(int language){
        _soundLanguage = language;
        UserDefault::getInstance()->setIntegerForKey("SoundLanguage", _soundLanguage);
    }
    
    int YZAudioEngine::getSoundLanguage() const{
        return _soundLanguage;
    }
}
