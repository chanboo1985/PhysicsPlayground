#include "MenuLayer.h"
#include "MainScene.h"

bool MenuLayer::init()
{
	if(!Layer::init())
		return false;

	addUI();

	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(MenuLayer::toggleMenu), "toggleMenu", nullptr);

	return true;
}


void MenuLayer::addUI()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	_popCurrent = nullptr;
	int margin = 10;
	_label = LabelTTF::create("Hello World", "fonts/Marker Felt.ttf", 24);
    _label->setString("   Mode:Move");
	_label->setPosition(origin.x + _label->getContentSize().width, origin.y + visibleSize.height - _label->getContentSize().height);
	this->addChild(_label);
//	_label->setVisible(false);
//	addUI();

	PopMenu* popSlider = PopMenu::create();

	popSlider->addSlider("Size");
	popSlider->addSlider("Density");
	popSlider->addSlider("Friction");
	popSlider->addSlider("Restitution");
	popSlider->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height);
	this->addChild(popSlider);

	auto pop = PopMenu::create();
	pop->addButton("Move",[](){log("Test1 Touched!");});
	pop->addButton("Add Regular",[](){log("Test2 Touched!");});
	pop->addButton("Add Custom",[](){log("Test3 Touched!");});
	pop->addButton("Add Joint",[](){log("Test4 Touched!");});
	pop->setPosition(origin.x + visibleSize.width - pop->getListViewContentSize().width, origin.y + visibleSize.height);
	pop->setName("popMain");
	this->addChild(pop);
	pop->setLocalZOrder(100);
	pop->popEnter();

	pop->setCallback("Move",[&](){
		PhysicsManager::getInstance()->setTouchType(PhysicsManager::MOVE_TYPE);
		_label->setString("Mode:Move");
		if(_popCurrent)
			_popCurrent->popExit();
		_popCurrent = nullptr;
	});

	int maxButtonShown = (int)((visibleSize.height - pop->getListViewContentSize().height - margin) / 
						(pop->getListViewContentSize().height /pop->getButtonCount()));


	PopMenu* popRegular = PopMenu::create();
	popRegular->setMaxButtonShown(maxButtonShown);
	popRegular->addButton("Circle",[](){PhysicsManager::getInstance()->setSideNum(0);});
	popRegular->addButton("Triangle",[](){PhysicsManager::getInstance()->setSideNum(3);});
	popRegular->addButton("Square",[](){PhysicsManager::getInstance()->setSideNum(4);});
	popRegular->addButton("Pentagon",[](){PhysicsManager::getInstance()->setSideNum(5);});
	popRegular->addButton("Hexgon",[](){PhysicsManager::getInstance()->setSideNum(6);});
	popRegular->addButton("Heptagon",[](){PhysicsManager::getInstance()->setSideNum(7);});
	popRegular->addButton("Octagon",[](){PhysicsManager::getInstance()->setSideNum(8);});
	popRegular->addButton("Enneagon",[](){PhysicsManager::getInstance()->setSideNum(9);});
	popRegular->addButton("Decagon",[](){PhysicsManager::getInstance()->setSideNum(10);});	
	popRegular->setPosition(pop->getPosition() - Vec2(0, pop->getListViewContentSize().height));
	popRegular->setMargin(margin);

	this->addChild(popRegular);
//	popRegular->setLocalZOrder(100);
	pop->setCallback("Add Regular",[&,popRegular,popSlider](){
		if(PhysicsManager::getInstance()->getTouchType()!=PhysicsManager::ADD_TYPE)
		{
			PhysicsManager::getInstance()->setTouchType(PhysicsManager::ADD_TYPE);
			_label->setString("Mode:Add Regular");
			popRegular->popEnter();
			if(_popCurrent)
				_popCurrent->popExit();
			_popCurrent = popRegular;
			popSlider->popEnter();
		}
	});

	PopMenu* popCustom = PopMenu::create();
	popCustom->addButton("Add",[&](){_main->setIsDelete(false);});
	popCustom->addButton("Delete",[&](){_main->setIsDelete(true);});
	popCustom->addButton("Clear",[&](){_main->clearMarks();});
	popCustom->addButton("Confirm",[&](){_main->addCustomPolygon();});
	popCustom->setPosition(pop->getPosition() - Vec2(0, pop->getListViewContentSize().height));
	popCustom->setMargin(margin);
	popCustom->setMaxButtonShown(maxButtonShown);
	this->addChild(popCustom);

	pop->setCallback("Add Custom",[&,popCustom](){
		if(PhysicsManager::getInstance()->getTouchType()!=PhysicsManager::ADD_CUSTOM_TYPE)
		{
			PhysicsManager::getInstance()->setTouchType(PhysicsManager::ADD_CUSTOM_TYPE);
			_label->setString("Mode:Add Custom");
			_main->setIsDelete(false);
			popCustom->popEnter();
			if(_popCurrent)
				_popCurrent->popExit();
			_popCurrent = popCustom;
		}
	});

	PopMenu* popJoint = PopMenu::create();
	popJoint->addButton("Mark",[](){});
	popJoint->addButton("Create",[](){});
	popJoint->addButton("Add Mark",[](){});
	popJoint->setPosition(pop->getPosition() - Vec2(0, pop->getListViewContentSize().height));
	popJoint->setMargin(margin);
	popJoint->setMaxButtonShown(maxButtonShown);
	this->addChild(popJoint);

	pop->setCallback("Add Joint",[&,popJoint](){
		if(PhysicsManager::getInstance()->getTouchType()!=PhysicsManager::ADD_JOINT_TYPE)
		{
			PhysicsManager::getInstance()->setTouchType(PhysicsManager::ADD_JOINT_TYPE);
			_label->setString("Mode:Add Joint");
			popJoint->popEnter();
			if(_popCurrent)
				_popCurrent->popExit();
			_popCurrent = popJoint;
		}
	});

	PopMenu* popPause = PopMenu::create();
	popPause->addButton("Pause", nullptr);
	popPause->addButton("Gravity", nullptr);
	popPause->addButton("Delete", nullptr);
	popPause->setPosition(origin.x, origin.y + visibleSize.height);
	popPause->setMargin(50);
	popPause->setName("popPause");
	this->addChild(popPause);

	popPause->setCallback("Pause", [&,popPause](){
		PhysicsManager::getInstance()->pause();
		popPause->reName("Pause","Resume");
	});

	popPause->setCallback("Resume",[&,popPause](){
		PhysicsManager::getInstance()->resume();
		popPause->reName("Resume","Pause");
	});

	popPause->setCallback("Gravity",[&](){
		PhysicsManager::getInstance()->setTouchType(PhysicsManager::SET_GRAVITY_TYPE);
		_label->setString("Mode:Set Gravity");
	});

	popPause->setCallback("Delete",[&](){
		PhysicsManager::getInstance()->setTouchType(PhysicsManager::DELETE_TYPE);
		_label->setString("Mode:Delete body");
	});

	popPause->popEnter();

    //_valueLabel = Text::create("Move the slider thumb","Move the slider thumb",24);
    //_valueLabel->setAnchorPoint(Vec2(0.5f, -1));
    //_valueLabel->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    //this->addChild(_valueLabel);

    //// Create the slider
    //Slider* slider = Slider::create();
    //slider->loadBarTexture("cocosui/sliderTrack.png");
    //slider->loadSlidBallTextures("cocosui/sliderThumb.png", "cocosui/sliderThumb.png", "");
    //slider->loadProgressBarTexture("cocosui/sliderProgress.png");
    //slider->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
    //slider->addEventListener(CC_CALLBACK_2(MenuLayer::sliderEvent, this));
    //this->addChild(slider);


}

void MenuLayer::sliderEvent(Ref *pSender, Slider::EventType type)
{
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        Slider* slider = dynamic_cast<Slider*>(pSender);
        int percent = slider->getPercent();
        _valueLabel->setString(String::createWithFormat("Percent %d", percent)->getCString());
    }
}

void MenuLayer::setMainScene(MainScene* mainScene)
{
	_main = mainScene;
}

void MenuLayer::toggleMenu(Ref* sender)
{
	PopMenu* pop = dynamic_cast<PopMenu*>(this->getChildByName("popMain"));
	pop->popToggle();
	pop = dynamic_cast<PopMenu*>(this->getChildByName("popPause"));
	pop->popToggle();
}