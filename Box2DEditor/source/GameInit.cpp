#include "GameInit.h"
#include "CommonLib/Ref.h"
#include "CommonLib/UIFont.h"
#include "CommonLib/DialogBox.h"
#include "CommonLib/AddData.h"
#include "PhysicsLib/PhysicsManager.h"
#include "Platform/Platform.h"
#include "CommonLib/Sprite.h"

GameInit::GameInit()
{
	InitGLSettings();
	Ref::GetInstance()->Init();
	AddData::GetInstance()->Init();
	UIFont::GetInstance()->Init();
	DialogBox::Init();
	PhysicsManager::GetInstance()->Init();
}

GameInit::~GameInit()
{
	PhysicsManager::GetInstance()->Terminate();
	AddData::GetInstance()->Terminate();
	DialogBox::Terminate();
	UIFont::GetInstance()->Terminate();
	Ref::GetInstance()->Terminate();
}

void GameInit::InitGLSettings()
{
	glShadeModel( GL_SMOOTH		);
	glCullFace	( GL_BACK		);
	glFrontFace	( GL_CCW		);

	glEnable( GL_BLEND );
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glBlendFunc	( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glDisable	( GL_FOG		);
	glDisable	( GL_LIGHTING	);
	glDisable	( GL_CULL_FACE	);
	glDisable	( GL_ALPHA_TEST	);
	glDisable	( GL_DEPTH_TEST	);

	glViewport(0,0, Platform::GetWindowWidth(), Platform::GetWindowHeight());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f,Platform::GetWindowWidth(),Platform::GetWindowHeight(),0.0f,0.0f,1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0,0,0,255);
}
