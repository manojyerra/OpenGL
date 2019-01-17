#ifndef PHY_MANAGER_H
#define PHY_MANAGER_H
#include "btBulletDynamicsCommon.h"

class PhyManager
{
private:
	static PhyManager* _ref;
	btTypedConstraint* m_pickConstraint;

	PhyManager();
	~PhyManager();

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;

public:
	static PhyManager* GetInstance();
	static void DeleteInstance();

	btDiscreteDynamicsWorld* GetDynamicWorld();
	void Update(float deltaTime);
	void Draw();

	btVector3 getRayTo(int x, int y);

	void OnClick(int x, int y);
	void OnRelease(int x, int y);
	void OnDrag(int x, int y);
};

#endif