#include "TextureManager.h"
#include "UIFont.h"
#include "AddData.h"
#include "Platform.h"
#include "SoundManager.h"
#include "GameInit.h"
#include "GameInfo.h"
#include "SaveManager.h"
#include "ModuleManager.h"
#include "Flags.h"

GameInit::GameInit()
{
	InitGLSettings();

	GameInfo::GetInstance();
	AddData::GetInstance();
	TextureManager::GetInstance();
	UIFont::GetInstance();
	SaveManager::GetInstance();
	SoundManager::GetInstance();
	Flags::GetInstance();
	ModuleManager::GetInstance();
}

GameInit::~GameInit()
{
	ModuleManager::DeleteInstance();
	Flags::DeleteInstance();
	SoundManager::DeleteInstance();
	SaveManager::DeleteInstance();
	UIFont::DeleteInstance();
	TextureManager::DeleteInstance();
	AddData::DeleteInstance();
	GameInfo::DeleteInstance();
}

void GameInit::InitGLSettings()
{
	glShadeModel( GL_SMOOTH		);
	glCullFace	( GL_BACK		);
	glFrontFace	( GL_CCW		);

	glEnable( GL_BLEND );
	glBlendFunc	( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

	glDisable	( GL_FOG		);
	glDisable	( GL_LIGHTING	);
	glDisable	( GL_CULL_FACE	);
	glDisable	( GL_ALPHA_TEST	);
	glDisable	( GL_DEPTH_TEST	);
}


/*
unsigned int GameLoop::GenerateGLTextureID(unsigned char* buffer, int width, int height, int bytesPerPixel)
{
	unsigned int textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(bytesPerPixel == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	else if(bytesPerPixel == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return textureID;
}

void GameLoop::DrawTexture(int textureID, int x, int y, int w, int h)
{
	float u = 0;
	float v = 0;
	float cw = 1;
	float ch = 1;

	GLfloat ver[8];
	GLfloat uvs[8];

	ver[0] = x;		ver[1] = y;
	ver[2] = x+w;	ver[3] = y;
	ver[4] = x;		ver[5] = y+h;
	ver[6] = x+w;	ver[7] = y+h;

	uvs[0] = u;		uvs[1] = v;
	uvs[2] = u+cw;	uvs[3] = v;
	uvs[4] = u;		uvs[5] = v+ch;
	uvs[6] = u+cw;	uvs[7] = v+ch;

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glVertexPointer(2, GL_FLOAT, 0, ver);
	glTexCoordPointer (2, GL_FLOAT, 0, uvs);

	glDrawArrays(GL_TRIANGLE_STRIP,0,4);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

 */
