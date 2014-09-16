#ifndef __MenuLayer__
#define __MenuLayer__
#include "cocos2d.h"
#include "PopMenu.h"
#include "PhysicsManager.h"
USING_NS_CC;

class MainScene;

class MenuLayer: public Layer
{
public:
	bool init();
	CREATE_FUNC(MenuLayer);
	void addUI();
	void setMainScene(MainScene* mainScene);
private:
	PopMenu* _popCurrent;
	LabelTTF* _label;
	MainScene* _main;
};

#endif