#ifndef __libCommon_LoadingLayer2_h__
#define __libCommon_LoadingLayer2_h__

#include "Base/YZLayer.h"
#include <string>

namespace YZ
{

	class LoadingLayer2 : public YZLayer
	{
	private:
		int		_opacity;
		Label*	_label;

	public:
		static LoadingLayer2* createLoading(Node* parent, const std::string& text, const std::string& image);

		static void removeLoading(Node* parent);

	private:
		LoadingLayer2();
		virtual ~LoadingLayer2();

	public:
		virtual bool init() override;

		// 功能接口
	public:
		void setText(const std::string& text);

	private:
		void createDialog(const std::string& text, const std::string& image);
	
		void autoSetOpacity(float dt);

	public:
		CREATE_FUNC(LoadingLayer2);
	};

}
#endif // __libCommon_LoadingLayer2_h__
