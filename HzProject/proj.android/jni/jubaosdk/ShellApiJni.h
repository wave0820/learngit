#ifndef __SHELLAPI_JNI__
#define __SHELLAPI_JNI__
#include <string>
/**
 * @author 石头
 */
class ShellApiJni {
public:
	struct PayOrder {
	public:
		std::string M_payId_;
		std::string M_amount_;
		std::string M_goodsName_;
		std::string M_remark_;
		std::string M_appId_;
		std::string M_playerId_;
		// std::string M_channelId_;
	};

	class Callback {
	protected:
		virtual ~Callback() {}
	public:
		virtual void call(int code, std::string mesasge) = 0;
	};

	static void pay(PayOrder order);

	static std::string srand();

	static void regCallback(Callback* callback);
};

#endif//__SHELL_API_JNI__