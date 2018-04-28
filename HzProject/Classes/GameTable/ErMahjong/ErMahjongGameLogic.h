#ifndef __ErMahjong_GameLogic_h__
#define __ErMahjong_GameLogic_h__

#include "YZBaseType.h"
#include "ErMahjongDefine.h"
#include <vector>

namespace ErMahjong
{
	class GameLogic
	{
	public:
		//���캯��		
		GameLogic(void);
		//��������
		virtual ~GameLogic(void);
	public:
		//ͨ����ֵ�����Ƶ�����
		BYTE GetKind(BYTE byPs);
		//�ж�һ�����Ƿ���Ч
		bool Verify(BYTE byPs);
		//ͨ����ֵ�����Ƶĵ���
		BYTE GetDian(BYTE byPs);
		//��ȡ�����������ֵ��ɫ��
	    BYTE CastDice(BYTE& byDice0,BYTE& byDice1);
		//ϴ���㷨
	    void RandCards(BYTE* pCards,int iNums);
		//ɾ��һ����
	    bool RemoveOneCard(BYTE byCard,BYTE bySrc[],int iSrcNums);

		//����
	    void sort(BYTE byCards[], int iNums,bool bFirstBig);

		bool WzFengZiLocked(int nDeskStation,BYTE byHandCard[MJ_MAX_HANDCARD], BYTE iCardCount,int iCaiShen);
	};
}

#endif // GameLogic_h__
