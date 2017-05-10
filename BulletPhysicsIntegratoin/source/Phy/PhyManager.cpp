#include "PhyManager.h"
#include <stdio.h>
#include "Cam.h"
#include "Util/GLUtil.h"

PhyManager* PhyManager::_ref = NULL;

PhyManager* PhyManager::GetInstance()
{
	if(_ref == NULL)
		_ref = new PhyManager();
	return _ref;
}

PhyManager::PhyManager()
{
	m_pickConstraint = 0;

	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new	btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0,-20,0));
}

btDiscreteDynamicsWorld* PhyManager::GetDynamicWorld()
{
	return dynamicsWorld;
}

void PhyManager::Update(float deltaTime)
{
	dynamicsWorld->stepSimulation(deltaTime,10);
}

void PhyManager::Draw()
{	
}

btVector3 PhyManager::getRayTo(int x,int y)
{
	float SW = (float)GLUtil::GetWindowWidth();
	float SH = (float)GLUtil::GetWindowHeight();

	float top = 0;			
	float bottom = 0;		
	float nearPlane = 0;	
	float farPlane = 0;
	float left = 0;
	float right = 0;

	GLUtil::GetProjectionValues(&left, &right, &bottom, &top, &nearPlane, &farPlane);

	float modelViewMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);

	float params[9];
	GLMat::GetGluLookAtParameters(modelViewMatrix, params);

	btVector3 camPos(params[0], params[1], params[2]);
	btVector3 camTarget(params[3], params[4], params[5]);
	btVector3 upVector(params[6], params[7], params[8]);

	float tanFov = (top-bottom)*0.5f / nearPlane;
	float fov = 2.0f * atanf (tanFov);

	btVector3 rayFrom = camPos;
	btVector3 forwardVector = camTarget - camPos;
	//btVector3 forwardVector = cam->getForwardVector();
	forwardVector.normalize();
	forwardVector*= farPlane;

	btVector3 rightOffset;

	btVector3 sideVector = forwardVector.cross(upVector);
	sideVector.normalize();
	upVector = sideVector.cross(forwardVector);
	upVector.normalize();

	//btVector3 sideVector = cam->getSideVector();

	float tanfov = tanf(0.5f*fov);

	btScalar aspect = SH / SW;

	sideVector *= 2.f * farPlane * tanfov;
	upVector *= 2.f * farPlane * tanfov;

	if(aspect < 1)
		sideVector /= aspect;
	else
		upVector *= aspect;

	btVector3 rayToCenter = rayFrom + forwardVector;
	btVector3 dHor = sideVector * 1.f/SW;
	btVector3 dVert = upVector * 1.f/SH;

	btVector3 rayTo = rayToCenter - 0.5f * sideVector + 0.5f * upVector;
	rayTo += (float)x * dHor;
	rayTo -= (float)y * dVert;

	return rayTo;
}

int gPickingConstraintId = 0;
btVector3 gOldPickingPos;
float gOldPickingDist  = 0.f;
btRigidBody* pickedBody = 0;//for deactivation state

btScalar mousePickClamping = 30.f;

void PhyManager::OnClick(int x, int y)
{
	float modelViewMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);

	float params[9];
	GLMat::GetGluLookAtParameters(modelViewMatrix, params);

	btVector3 m_cameraPosition(params[0], params[1], params[2]);

	btVector3 rayTo =getRayTo(x,y);

	//add a point to point constraint for picking
	if(dynamicsWorld)
	{
		btCollisionWorld::ClosestRayResultCallback rayCallback(m_cameraPosition,rayTo);
		dynamicsWorld->rayTest(m_cameraPosition,rayTo,rayCallback);
		
		if (rayCallback.hasHit())
		{
			btRigidBody* body = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
			if (body)
			{
				//other exclusions?
				if (!(body->isStaticObject() || body->isKinematicObject()))
				{
					pickedBody = body;
					pickedBody->setActivationState(DISABLE_DEACTIVATION);

					btVector3 pickPos = rayCallback.m_hitPointWorld;

					btVector3 localPivot = body->getCenterOfMassTransform().inverse() * pickPos;

					btPoint2PointConstraint* p2p = new btPoint2PointConstraint(*body,localPivot);
					p2p->m_setting.m_impulseClamp = mousePickClamping;

					dynamicsWorld->addConstraint(p2p);
					m_pickConstraint = p2p;

					//save mouse position for dragging
					gOldPickingPos = rayTo;

					btVector3 eyePos(m_cameraPosition[0],m_cameraPosition[1],m_cameraPosition[2]);

					gOldPickingDist  = (pickPos-eyePos).length();

					//very weak constraint for picking
					p2p->m_setting.m_tau = 1.1f;
				}
			}
		}
	}
}

void PhyManager::OnDrag(int x, int y)
{
	if(m_pickConstraint)
	{
		float modelViewMatrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);

		float params[9];
		GLMat::GetGluLookAtParameters(modelViewMatrix, params);

		btVector3 m_cameraPosition(params[0], params[1], params[2]);

		//Cam* cam = Cam::GetInstance();
		//btVector3 m_cameraPosition = cam->getCameraPosition();

		//move the constraint pivot
		btPoint2PointConstraint* p2p = static_cast<btPoint2PointConstraint*>(m_pickConstraint);
		if (p2p)
		{
			//keep it at the same picking distance
			btVector3 newRayTo = getRayTo(x,y);
			btVector3 eyePos(m_cameraPosition[0],m_cameraPosition[1],m_cameraPosition[2]);
			btVector3 dir = newRayTo-eyePos;
			dir.normalize();
			dir *= gOldPickingDist;

			btVector3 newPos = eyePos + dir;
			p2p->setPivotB(newPos);
		}
	}
}

void PhyManager::OnRelease(int x, int y)
{
	if(m_pickConstraint)
	{
		dynamicsWorld->removeConstraint(m_pickConstraint);
		delete m_pickConstraint;
		//printf("removed constraint %i",gPickingConstraintId);
		m_pickConstraint = 0;
		pickedBody->forceActivationState(ACTIVE_TAG);
		pickedBody->setDeactivationTime( 0.f );
		pickedBody = 0;
	}
}

PhyManager::~PhyManager()
{
    //cleanup in the reverse order of creation/initialization

    //remove the rigid bodies from the dynamics world and delete them
    //for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
    //    btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
    //    btRigidBody* body = btRigidBody::upcast(obj);
    //    if (body && body->getMotionState()) {
    //        delete body->getMotionState();
    //    }
    //    dynamicsWorld->removeCollisionObject(obj);
    //    delete obj;
    //}

	if(dynamicsWorld)
	{
		delete dynamicsWorld;
		dynamicsWorld = NULL;
	}

	if(solver)
	{
		delete solver;
		solver = NULL;
	}

	if(overlappingPairCache)
	{
		delete overlappingPairCache;	//delete broadphase
		overlappingPairCache = NULL;
	}

	if(dispatcher)
	{
		delete dispatcher;
		dispatcher = NULL;
	}

	if(collisionConfiguration)
	{
		delete collisionConfiguration;
		collisionConfiguration = NULL;
	}
}

void PhyManager::DeleteInstance()
{
	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}