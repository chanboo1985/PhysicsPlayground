#ifndef __PhysicsManager__
#define __PhysicsManager__
#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "GLES-Render.h"

USING_NS_CC;

#define PTM_RATIO 32.0
class PhysicsManager : public Ref
{
public:

	enum TouchType
	{
		MOVE_TYPE,
		ADD_TYPE,
		ADD_CUSTOM_TYPE,
		ADD_JOINT_TYPE,
		SET_GRAVITY_TYPE,
		DELETE_TYPE,
		NO_COLLIDE_TYPE,
		COLLIDE_TYPE,
		ADD_GADGET
	};

	enum GadgetType{
		GADGET_INVALID = 0,
		GADGET_THRUSTER,
		GADGET_HYDRAULIC
	};

	static PhysicsManager* getInstance();
	static void purgeInstance();
	bool init();
	void addRegularPolygon(Point pos);
	bool isMovingBody();
	bool MouseDown(const Vec2& pos);
	void MouseUp(const Vec2& pos);
	void MouseMove(const Vec2& pos);

	bool pauseMouseDown(const Vec2& pos);

	void addCustomPolygon(const std::vector<Vec2>& points);
	void addCircle(Point pos, float radias);
	void addJoint(const Vec2& pos1, const Vec2& pos2, const Vec2& pos3 = Vec2(), const Vec2& pos4 = Vec2());
	void addNoCollide(const Vec2& pos1, const Vec2& pos2);

	b2Body* getBodyAt(const Vec2& pos);
	void deleteBodyAt(const Vec2& pos);
	void addWheelJoint(const Vec2& pos);
	void update(float dt);
	void pause();
	void resume();
	void togglePause();
	void setGravity(const Vec2& gravity);
	float getPropertyByName(const std::string& name);
	bool getPropertyByNameBool(const std::string& name);
	Vec2 getRangeByName(const std::string& name);
	void setPropertyByName(const std::string& name, float fval);
	void setPropertyByNameBool(const std::string& name, bool bval);
	b2BodyType getBodyType(int type);
	void doDelete();
	b2Joint* getJointForGear(b2Body* body);

	void addGadgetAt(const Vec2& pos);
public:
	CC_SYNTHESIZE(TouchType, _touchType, TouchType);
	CC_SYNTHESIZE(b2JointType, _jointType, JointType);
	CC_SYNTHESIZE(b2World*, _world, World);
	CC_SYNTHESIZE(int, _sideNum, SideNum);
	CC_SYNTHESIZE(GadgetType, _gadgetType, GadgetType);

private:
	class BodyInfo :public Ref
	{
	public:
		bool init();
		CREATE_FUNC(BodyInfo);
		void deleteCollide(b2Body* body);
		std::vector<b2Body*> collides;
	};

	class CustomFilter :public b2ContactFilter
	{
	public:
		bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
	};

private:
	static PhysicsManager* _physicsManager;
	PhysicsManager();
	~PhysicsManager();
	PhysicsManager(const PhysicsManager& other);
	PhysicsManager& operator=(const PhysicsManager& other);

	GLESDebugDraw* _debugDraw;

//	TouchType _touchType;
	b2Vec2 _mouseWorld;
	b2MouseJoint* _mouseJoint;
	b2Body* _groundBody;
	b2Body* _car;
	b2Body* _wheel;
	bool _isPaused;
	b2Body* _movingBody;
	float _size;
	b2FixtureDef _fixtureDef;
	b2WheelJointDef _wheelJointDef;
	bool _enableMotor;
	bool _collideConnected;
	float _motorSpeed;
	float _maxMotorTorque;
	float _frequencyHz;
	float _dampingRatio;
	float _lowerAngle;
	float _upperAngle;
	bool _enableLimit;
	float _lowerTranslation;
	float _upperTranslation;
	float _maxMotorForce;
	float _bodyType;
	bool _toGround;
	float _pulleyRatio;
	float _maxLength;
	float _maxForce;
	float _maxTorque;
	std::vector<b2Joint*> _jointDelete;
	std::vector<b2Body*> _bodyDelete;
	float _gearRatio;
	CustomFilter* _filter;
	std::vector<b2Body*> _thrusters;
	int _maxControllerNum;
	std::vector<float> _controller;
};


#endif