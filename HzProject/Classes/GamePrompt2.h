#ifndef GamePrompt2_h__
#define GamePrompt2_h__

#include "YZUIExport.h"

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
//#include "SDK1.6.2/CDShareInstance.h"
using namespace ui;
using namespace cocostudio;

class GamePromptLayer2 : public YZLayer
{
	CCRect _userRect;

private:
	Text*			_Label_Prompt;
	Button*         _Button_OK;
	Layout*			_layout;

public:
	CREATE_FUNC(GamePromptLayer2);

	GamePromptLayer2();
	virtual ~GamePromptLayer2();

public:
	virtual bool init() override;  

	void showPrompt(const std::string& prompt);

	void closePrompt();

	void setCallBack(std::function<void ()> sure);

private:
	void menuEventCallBack(Ref* pSender, Widget::TouchEventType type);

	void setPrompt(const std::string& prompt);

	void setPrompt(const std::string& prompt, const std::string& font, int size);

	void openUrlClicked(Ref * pSender, Widget::TouchEventType type);

private:
	std::function<void ()> _sure;

};

#endif // GamePrompt2_h__
