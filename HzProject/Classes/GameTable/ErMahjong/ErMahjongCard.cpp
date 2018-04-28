#include "ErMahjongCard.h"
#include "Util/JzLog.h"
namespace ErMahjong
{
	#define ErTag_Color_Layer 100
	#define ErFile_Card_Mark "erMaJiang/DisplayPicture/card_mark.png"

	MahjongCard::MahjongCard()
	{
		_ignoreAnchorPointForPosition = false;
        caiShen = nullptr;
	}

	MahjongCard::~MahjongCard()
	{
	}

	 MahjongCard* MahjongCard::create(BYTE cardValue, MJPostion pos){
		MahjongCard* card = new MahjongCard();
		if (card && card->initWithCardValue(cardValue,pos))
		{
			card->autorelease();
			return card;
		}
		CC_SAFE_DELETE(card);
		return nullptr;
	}

	bool MahjongCard::initWithCardValue(BYTE cardValue,MJPostion pos)
	{
		if (!Sprite::initWithFile(getCardTextureFileName(cardValue,pos,false)))
		{
			return false;
		}

        _dir = pos;
		_Value = cardValue;
        _bResetCardSize = true;
        _caiShenCardValue = 0x00;
        
        switch (_dir) {
            case MJPostion::Bottom:
            {
                this->setCardSize(MJ_BOTTOM_W, MJ_BOTTOM_H);
                _bResetCardSize = true;
            }
                break;
                
            case MJPostion::Right:
            {
                this->setCardSize(MJ_RIGHT_W, MJ_RIGHT_H);
                _bResetCardSize = false;
            }
                break;
            case MJPostion::Top:
            {
                this->setCardSize(MJ_TOP_W, MJ_TOP_H);
                _bResetCardSize = true;
            }
                break;
            case MJPostion::Left:
            {
                this->setCardSize(MJ_LEFT_W, MJ_LEFT_H);
                _bResetCardSize = false;
            }
                break;
        }
        
		return true;
	}
    
    void MahjongCard::showCardWinTexture(){
        this->setTexture(getCardTextureFileName(_Value,_dir,true));
        this->resetCardSize();
    }
    
    void MahjongCard::updateCardTexture(){
        this->setTexture(getCardTextureFileName(_Value,_dir,false));
        this->resetCardSize();
    }

    void MahjongCard::setCaiShenCardValue(BYTE cardValue){
        _caiShenCardValue = cardValue;
    }
    
	void MahjongCard::setCardValue(BYTE cardValue)
	{
		//this->setTexture(getCardTextureFileName(cardValue,false));
		_Value = cardValue;
	}
    
    
    void MahjongCard::setCardAnGuangTexture(){
            
        string fileName = "";
        
        Size oldSize = this->getContentSize();
        
        switch (_dir)
        {
            case ErMahjong::Bottom:
                fileName=   "erMaJiang/positonMark/ownMark.png";
                break;
            case ErMahjong::Right:
                fileName=   "erMaJiang/positonMark/an_leftRight.png";
                break;
            case ErMahjong::Top:
                fileName=   "erMaJiang/positonMark/ownMark.png";
                break;
            case ErMahjong::Left:
                fileName=   "erMaJiang/positonMark/an_leftRight.png";
                break;
        }
        
        this->setTexture(fileName);
        this->resetCardSize();
    }
    
    void MahjongCard::setCardSize(float width, float heght){
        _cardWidth = width;
        _cardHeight = heght;
        resetCardSize();
    }
    
    void MahjongCard::resetCardSize(){
        
        if (!_bResetCardSize) {
            return;
        }
        
        this->setScale(_cardWidth / this->getContentSize().width, _cardHeight / this->getContentSize().height);
    }
    
	BYTE MahjongCard::getCardValue()
	{
		return _Value;
	}

	void MahjongCard::addTouchedLayer()
	{
		if (this->getChildByTag(ErTag_Color_Layer) == nullptr)
		{
			auto mark = Sprite::create(ErFile_Card_Mark);
			mark->setAnchorPoint(Vec2::ZERO);
			mark->setPosition(Vec2::ZERO);
			this->addChild(mark, 1, ErTag_Color_Layer);

		}
	}

	void MahjongCard::removeTouchedLayer()
	{
		Node* pNode = this->getChildByTag(ErTag_Color_Layer);
		if(pNode != nullptr)
		{
			pNode->removeFromParent();
		}
	}
    
    BYTE MahjongCard::getMJCardType(){
        return (_Value&MASK_COLOR)>>4;
    }
    
    BYTE MahjongCard::getMJCardValue(){
        return (_Value&MASK_VALUE);
    }

    bool MahjongCard::afterMJCard(ErMahjong::MahjongCard *card){
        BYTE cbColor= this->getMJCardType();
        BYTE tmpCardType = card->getMJCardType();
        
        if (card->getCardValue() == _caiShenCardValue) {
            return false;
        }
        
        //如果花色相同
        if(cbColor == tmpCardType){
            BYTE cbValue= this->getMJCardValue();
            BYTE tmpCardValue = card->getMJCardValue();
            
            //CCLOG("Last Value : %d   Tmp Value : %d" , cbValue , tmpCardValue);
            //花色相同，如果
            if(tmpCardValue < cbValue){
                return false;
            }else{
                return true;
            }
        }else if (tmpCardType > cbColor){
            return true;
        }else{
            return false;
        }
    }
    
    void MahjongCard::showCaiShenSprite(bool isVisable){
        if (caiShen == nullptr) {
            caiShen = Sprite::create("erMaJiang/long.png");
            caiShen->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            caiShen->setPosition(Vec2(-15, -2));
            caiShen->setVisible(false);
            this->addChild(caiShen);
        }
        
        if (_dir == MJPostion::Left || _dir == MJPostion::Right) {
            caiShen->setRotation(90);
        }
        
        caiShen->setVisible(isVisable);
        
    }
    
	string MahjongCard::getCardTextureFileName(BYTE cardValue,MJPostion pos,bool iWin)
	{
		BYTE cbColor=(cardValue&MASK_COLOR)>>4;
		BYTE cbValue=(cardValue&MASK_VALUE);
        
        char filename[60];
        
		if (iWin)
		{
            if (cbColor < 0 || cbColor > 3 || cbValue < 1 || cbValue > 9) {
                JzLog::shareLog()->log(kLogLevelError, "获取牌的纹理出错。 牌值为 : %d" , cardValue);
                sprintf(filename,"erMaJiang/winCard/w_%d.png",1);
                return filename;
            }
            
            switch (pos)
            {
                case ErMahjong::Bottom:
                {
                    switch (cbColor)
                    {
                        case MJColor::Wan:
                            sprintf(filename,"erMaJiang/outCard/buttom_top/w_%d.png",cbValue);
                            break;
                        case MJColor::Tiao:
                            sprintf(filename,"erMaJiang/outCard/buttom_top/tiao_%d.png",cbValue);
                            break;
                        case MJColor::Bing:
                            sprintf(filename,"erMaJiang/outCard/buttom_top/tong_%d.png",cbValue);
                            break;
                        case MJColor::Feng:
                            sprintf(filename,"erMaJiang/outCard/buttom_top/feng_%d.png",cbValue);
                            break;
                    }
                }
                    break;
                case ErMahjong::Right:
                    switch (cbColor)
                {
                    case MJColor::Wan:
                        sprintf(filename,"erMaJiang/outCard/right/w_%d.png",cbValue);
                        break;
                    case MJColor::Tiao:
                        sprintf(filename,"erMaJiang/outCard/right/tiao_%d.png",cbValue);
                        break;
                    case MJColor::Bing:
                        sprintf(filename,"erMaJiang/outCard/right/tong_%d.png",cbValue);
                        break;
                    case MJColor::Feng:
                        sprintf(filename,"erMaJiang/outCard/right/feng_%d.png",cbValue);
                        break;
                }

                    break;
                case ErMahjong::Top:
                {
                    switch (cbColor)
                    {
                        case MJColor::Wan:
                            sprintf(filename,"erMaJiang/winCard/w_%d.png",cbValue);
                            break;
                        case MJColor::Tiao:
                            sprintf(filename,"erMaJiang/winCard/tiao_%d.png",cbValue);
                            break;
                        case MJColor::Bing:
                            sprintf(filename,"erMaJiang/winCard/tong_%d.png",cbValue);
                            break;
                        case MJColor::Feng:
                            sprintf(filename,"erMaJiang/winCard/feng_%d.png",cbValue);
                            break;
                    }

                }
                    break;
                case ErMahjong::Left:
                    switch (cbColor)
                {
                    case MJColor::Wan:
                        sprintf(filename,"erMaJiang/outCard/left/w_%d.png",cbValue);
                        break;
                    case MJColor::Tiao:
                        sprintf(filename,"erMaJiang/outCard/left/tiao_%d.png",cbValue);
                        break;
                    case MJColor::Bing:
                        sprintf(filename,"erMaJiang/outCard/left/tong_%d.png",cbValue);
                        break;
                    case MJColor::Feng:
                        sprintf(filename,"erMaJiang/outCard/left/feng_%d.png",cbValue);
                        break;
                }

                    break;
            }
        }
        else
        {
            switch (pos)
			{
				case ErMahjong::Bottom:
				{
                    if (cbColor < 0 || cbColor > 3 || cbValue < 1 || cbValue > 9) {
                        JzLog::shareLog()->log(kLogLevelError, "获取牌的纹理出错。 牌值为 : %d" , cardValue);
                        sprintf(filename,"erMaJiang/winCard/w_%d.png",1);
                        return filename;
                    }
                    
					switch (cbColor)
					{
						case MJColor::Wan:
							sprintf(filename,"erMaJiang/positonMark/w_%d.png",cbValue);
							break;
						case MJColor::Tiao:
							sprintf(filename,"erMaJiang/positonMark/tiao_%d.png",cbValue);
							break;
						case MJColor::Bing:
							sprintf(filename,"erMaJiang/positonMark/tong_%d.png",cbValue);
							break;
						case MJColor::Feng:
							sprintf(filename,"erMaJiang/positonMark/feng_%d.png",cbValue);
							break;
					}
				}
				break;
			case ErMahjong::Right:
				sprintf(filename,"erMaJiang/positonMark/right_card.png");
				break;
			case ErMahjong::Top:
				sprintf(filename,"erMaJiang/positonMark/top_card.png");
				break;
			case ErMahjong::Left:
				sprintf(filename,"erMaJiang/positonMark/left_card.png");
				break;
			}
		}
		return filename;
	}
}

