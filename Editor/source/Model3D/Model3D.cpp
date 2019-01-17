#include "Model3D.h"
#include "Sui/Sui.h"
#include "Cam.h"
#include "btBulletDynamicsCommon.h"
#include "IO/BinaryObject/BinaryObjectReader.h"
#include "IO/FLM/FLMWriter.h"
#include "Util/UtilFuncs.h"
#include "Util/Meterial.h"

Model3D::Model3D(string filePath, int drawMode)
{
	_modelPath = filePath;
	_drawMode = drawMode;
	_scaleUnits = 1.0f;

	_textureID = 0;
	_vertexBufID = 0;
	_uvBufID = 0;
	_normalBufID = 0;
	_indicesBufID = 0;
	_displayListID = 0;
	_drawTexture = true;
	_drawBoundingShapes = true;
	_wireFrame = false;
	_meterialName = "";

	_r = _g = _b = _a = 255;

	_helper = new Model3D_Helper();

	_flm = new FLMStruct();
	FLMReader reader(filePath, _flm);
	SetOrientation(_flm->mat);

	if(_flm->texturePath)
		SetTexture(_flm->texturePath);

	_drawMode = IMMEDIATE_MODE;

	if(_drawMode == VBO_MODE)
		CreateBufferObects();
	else if(_drawMode == DISPLAY_LIST_MODE)
		CreateCallList();

	_compoundShape = NULL;
}

void Model3D::UniformScaleBuf(float units)
{
	for(int i=0; i<_flm->numVertex; i++)
	{
		_flm->vertexBuf[3*i + 0] *= units/_scaleUnits;
		_flm->vertexBuf[3*i + 1] *= units/_scaleUnits;
		_flm->vertexBuf[3*i + 2] *= units/_scaleUnits;
	}

	_scaleUnits = units;
}

float Model3D::GetScaleUnits()
{
	return _scaleUnits;
}

void Model3D::SetAsPhysicsObj(bool makePhysicsObj)
{
	if(makePhysicsObj)
	{
		if(_flm->numBShapes > 0)
		{
			if(_modelPath.find("gramphone") != -1)
			{
				_flm->shapeIDArr[0] = Shape::MESH;
				_flm->numBShapes = 1;
			}

			_compoundShape = new PhyCompound();

			for(int i=0;i<_flm->numBShapes; i++)
			{
				if(_flm->shapeIDArr[i] == Shape::BOX)
				{
					_compoundShape->AddBox(	_flm->bShapeMatArr[i],
											_flm->scaleArr[i][0]/2.0f,
											_flm->scaleArr[i][1]/2.0f,
											_flm->scaleArr[i][2]/2.0f);
				}
				else if(_flm->shapeIDArr[i] == Shape::CYLINDER)
				{
					_compoundShape->AddCylinder(_flm->bShapeMatArr[i],
												_flm->scaleArr[i][0],
												_flm->scaleArr[i][2]/2.0f);
				}
				else if(_flm->shapeIDArr[i] == Shape::SPHERE)
				{
					_compoundShape->AddSphere(_flm->bShapeMatArr[i], _flm->scaleArr[i][0]);
				}
				else if(_flm->shapeIDArr[i] == Shape::CONE)
				{
					_compoundShape->AddCone(_flm->bShapeMatArr[i], _flm->scaleArr[i][0], _flm->scaleArr[i][2]);
				}
				else if(_flm->shapeIDArr[i] == Shape::MESH)
				{
					btTriangleMesh* triMesh = new btTriangleMesh();

					for(int j=0; j<_flm->numIndices; j+=3)
					{
						int index1 = 0;
						int index2 = 0;
						int index3 = 0;

						if(_flm->indicesFormat == GL_UNSIGNED_BYTE)
						{
							index1 = _flm->indicesBuf[j+0];
							index2 = _flm->indicesBuf[j+1];
							index3 = _flm->indicesBuf[j+2];
						}
						else if(_flm->indicesFormat == GL_UNSIGNED_SHORT)
						{
							index1 = ((unsigned short*)(_flm->indicesBuf))[j+0];
							index2 = ((unsigned short*)(_flm->indicesBuf))[j+1];
							index3 = ((unsigned short*)(_flm->indicesBuf))[j+2];
						}
						else if(_flm->indicesFormat == GL_UNSIGNED_INT)
						{
							index1 = ((unsigned int*)(_flm->indicesBuf))[j+0];
							index2 = ((unsigned int*)(_flm->indicesBuf))[j+1];
							index3 = ((unsigned int*)(_flm->indicesBuf))[j+2];
						}

						btVector3 v0(_flm->vertexBuf[3*index1 + 0], _flm->vertexBuf[3*index1 + 1], _flm->vertexBuf[3*index1 + 2]);
						btVector3 v1(_flm->vertexBuf[3*index2 + 0], _flm->vertexBuf[3*index2 + 1], _flm->vertexBuf[3*index2 + 2]);
						btVector3 v2(_flm->vertexBuf[3*index3 + 0], _flm->vertexBuf[3*index3 + 1], _flm->vertexBuf[3*index3 + 2]);

						triMesh->addTriangle(v0,v1,v2);

						bVerVec.push_back(v0.getX());
						bVerVec.push_back(v0.getY());
						bVerVec.push_back(v0.getZ());

						bVerVec.push_back(v1.getX());
						bVerVec.push_back(v1.getY());
						bVerVec.push_back(v1.getZ());

						bVerVec.push_back(v2.getX());
						bVerVec.push_back(v2.getY());
						bVerVec.push_back(v2.getZ());
					}

					float tempMat[16];
					memset(tempMat, 0, 16*sizeof(float));
					tempMat[0] = tempMat[5] = tempMat[10] = tempMat[15] = 1.0f;
					_compoundShape->AddMesh(tempMat, triMesh);
				}
			}

			if(_modelPath.find("gramphone") != -1)
				_compoundShape->MakeCompoundShape(GetOrientation().Get(), 5);
			else
				_compoundShape->MakeCompoundShape(GetOrientation().Get(), GetMass());
		}
	}
	else
	{
		if(_compoundShape)
		{
			delete _compoundShape;
			_compoundShape = NULL;
		}
	}
}

const FLMStruct* Model3D::GetFLMStruct()
{
	return _flm;
}

string Model3D::GetModelPath()
{
	return _modelPath;
}

string Model3D::GetTexturePath()
{
	return _texturePath;
}

Model3D* Model3D::Clone()
{
	string tempModelPath = "";
	
	int count = 1;
	char replaceWith[32];

	while(true)
	{
		sprintf(replaceWith, "_d%d.flm", count);

		tempModelPath = _modelPath;
		tempModelPath.replace(_modelPath.find(".flm"), 4, replaceWith);

		FILE* fp = fopen(tempModelPath.c_str(), "rb");
		if(!fp)
			break;
		else
			fclose(fp);

		count++;
	}

	Write(tempModelPath);
	Model3D* model = new Model3D(tempModelPath, _drawMode);
	return model;
}

void Model3D::SetPos(float x, float y, float z)
{
	_pos.x = x;
	_pos.y = y;
	_pos.z = z;
}

Point Model3D::GetPos()
{
	return _pos;
}

Point Model3D::GetScale()
{
	return Point(1,1,1);
}

Point Model3D::GetRotXYZ()
{
	return MatrixUtil::GetEulerXYZRot_In_Degrees(_rot.Get());
}

void Model3D::SetRotXYZ(float x, float y, float z)
{
	MatrixUtil::GetXYZRotArray(x, y, z, _rot.Get());
}

GLMat Model3D::GetOrientation()
{
	GLMat mat;
	mat.glTranslatef(_pos.x, _pos.y, _pos.z);
	mat.glMultMatrixf(_rot.Get());

	return mat;
}

void Model3D::SetOrientation(float* inMat)
{
	_pos.x = inMat[12];
	_pos.y = inMat[13];
	_pos.z = inMat[14];

	Point rot = MatrixUtil::GetEulerXYZRot_In_Degrees(inMat);
	_rot = MatrixUtil::GetGLMat(0,0,0, rot.x, rot.y, rot.z);
}

//**************** World Tranformations *********************//

void Model3D::TransInWorld(float x, float y, float z)
{
	_pos.Add(x,y,z);
}

void Model3D::RotInWorld(char axis, float angle)
{
	GLMat newRot;
	
	if(axis == 'x' || axis == 'X')	newRot.glRotatef(angle, 1,0,0);
	if(axis == 'y' || axis == 'Y')	newRot.glRotatef(angle, 0,1,0);
	if(axis == 'z' || axis == 'Z')	newRot.glRotatef(angle, 0,0,1);

	newRot.glMultMatrixf( _rot.Get() );

	_rot = newRot;
}


//**************** Local Tranformations *********************//

void Model3D::TransInLocal(char axis, float move)
{
	CVector3 vec;

	float* rot = _rot.Get();

	if(axis == 'x')			vec = CVector3( rot[0], rot[1], rot[2] );
	else if(axis == 'y')	vec = CVector3( rot[4], rot[5], rot[6] );
	else if(axis == 'z')	vec = CVector3( rot[8], rot[9], rot[10] );
	
	vec.Normalize();
	vec *= move;

	_pos.x += vec.x;
	_pos.y += vec.y;
	_pos.z += vec.z;
}

void Model3D::RotInLocal(char axis, float angle)
{
	float* currMat = _rot.Get();

	float viewBackUp[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, viewBackUp);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(axis == 'X' || axis == 'x')			glRotatef(angle, currMat[0], currMat[1], currMat[2]);
	else if(axis == 'Y' || axis == 'y')		glRotatef(angle, currMat[4], currMat[5], currMat[6]);
	else if(axis == 'Z' || axis == 'z')		glRotatef(angle, currMat[8], currMat[9], currMat[10]);

	glMultMatrixf(currMat);
	float newMat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, newMat);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(viewBackUp);

	_rot.Copy(newMat);
}

void Model3D::SetTexture(string texturePath)
{
	_texturePath = texturePath;

	if(_texturePath.length() > 0)
	{
		ImageBuffer* imgBuf = new ImageBuffer(_texturePath);

		if(imgBuf->GetBuffer())
		{
			if(_textureID)
				glDeleteTextures(1, &_textureID);

			_textureID = UtilFuncs::GenerateGLTextureID(imgBuf);
		}

		delete imgBuf;
	}
}

void Model3D::SetDrawTexture(bool drawTexture)
{
	_drawTexture = drawTexture;
}

bool Model3D::IsTextureDrawing()
{
	return _drawTexture;
}

unsigned int Model3D::GetGLTextureID()
{
	return _textureID;
}

void Model3D::SetGLTextureID(unsigned int id)
{
	_textureID = id;
}

int Model3D::GetMeterialID()
{
	return _flm->meterialID;
}

float Model3D::GetMass()
{
	if(IsStaticObject())
		return 0;

	float totVol = 0;

	for(int i=0;i<_flm->numBShapes;i++)
	{
		if(_flm->shapeIDArr[i] == Shape::BOX)
		{
			float w = _flm->scaleArr[i][0];
			float h = _flm->scaleArr[i][1];
			float d = _flm->scaleArr[i][2];

			totVol += w*h*d;
		}
		else if(_flm->shapeIDArr[i] == Shape::CYLINDER)
		{
			float radius = _flm->scaleArr[i][0];
			float height = _flm->scaleArr[i][2];

			totVol += PI_VAL * radius * radius * height;
		}
		else if(_flm->shapeIDArr[i] == Shape::CONE)
		{
			float radius = _flm->scaleArr[i][0];
			float height = _flm->scaleArr[i][2];

			totVol += (1.0f/3.0f) * PI_VAL * radius * radius * height;
		}
		else if(_flm->shapeIDArr[i] == Shape::SPHERE)
		{
			float radius = _flm->scaleArr[i][0];

			totVol += (4.0f/3.0f) * PI_VAL * radius * radius * radius;
		}
	}

	if(_flm->meterialID < 0)
		return 1;

	return totVol*Meterial::GetInstance()->massArr[_flm->meterialID] / 500.0f;
}

void Model3D::SetMeterialID(int meterialID, string meterialName)
{
	_flm->meterialID = meterialID;
	_meterialName = meterialName;
}

string Model3D::GetMeterialName()
{
	if(IsStaticObject())
		return "Static Object";

	return _meterialName;
}

//void Model3D::SetAsStaticObject(bool val)
//{
//	_flm->SetAsStaticObject(val);
//}

bool Model3D::IsStaticObject()
{
	return (_flm->meterialID == 0);
}

void Model3D::SetDrawBoundingShapes(bool drawBoundingShapes)
{
	_drawBoundingShapes = drawBoundingShapes;
}

void Model3D::AddBoundingShape(int shapeID, float* mat, Point scale)
{
	float scaleArr[3];
	scaleArr[0] = scale.x;
	scaleArr[1] = scale.y;
	scaleArr[2] = scale.z;

	AddBoundingShape(shapeID, mat, scaleArr);
}

void Model3D::AddBoundingShape(int shapeID, float* mat, float* scale)
{
	_flm->AddBoundingShape(shapeID, mat, scale);
}

void Model3D::RemoveAllBoundingShapes()
{
	_flm->RemoveAllBoundingShapes();
}

void Model3D::DeleteBoundingBox(int index)
{
	_flm->RemoveBoundingShape(index);
}

void Model3D::Write()
{
	Write(_modelPath);
}

void Model3D::Write(string path)
{
	_flm->SetMatByCopy(GetOrientation().Get());
	_flm->SetTexturePath(_texturePath);

	FLMWriter FLMWriter(_flm);
	FLMWriter.Write(path);
}

void Model3D::SetColor(int r, int g, int b, int a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

void Model3D::EnableWireFrame(bool wireFrame)
{
	_wireFrame = wireFrame;
}

bool Model3D::IsInWireFrame()
{
	return _wireFrame;
}

void Model3D::DrawBoundingShape(int index, int r, int g, int b, int a)
{
	if((index >=0 && index < _flm->numBShapes) == false)
		return;

	if(_drawBoundingShapes && _flm->numBShapes > 0)
	{
		glPushMatrix();
		
		if(_compoundShape)
		{
			float phyMat[16];
			_compoundShape->GetOrientation(phyMat);
			glMultMatrixf(phyMat);
		}
		else
		{
			glTranslatef(_pos.x, _pos.y, _pos.z);
			glMultMatrixf(_rot.Get());
		}

		bool isLightOn = glIsEnabled(GL_LIGHTING);
		glDisable(GL_LIGHTING);

		int i = index;

		if(_flm->shapeIDArr[i] == Shape::BOX)
		{
			Box bBox(0,0,0, _flm->scaleArr[i][0], _flm->scaleArr[i][1], _flm->scaleArr[i][2]);
			bBox.SetOpenGLMatrix( _flm->bShapeMatArr[i] );
			bBox.SetColor(r, g, b, a);
			bBox.Draw(false);
		}
		else if(_flm->shapeIDArr[i] == Shape::CYLINDER)
		{
			Cylinder bCylinder(0,0,0, _flm->scaleArr[i][0], _flm->scaleArr[i][2]);
			bCylinder.SetOpenGLMatrix( _flm->bShapeMatArr[i] );
			bCylinder.SetColor(r, g, b, a);
			bCylinder.Draw(false);
		}
		else if(_flm->shapeIDArr[i] == Shape::SPHERE)
		{
			Sphere bSphere(0,0,0, _flm->scaleArr[i][0]);
			bSphere.SetOpenGLMatrix( _flm->bShapeMatArr[i] );
			bSphere.SetColor(r, g, b, a);
			bSphere.Draw(false);
		}
		else if(_flm->shapeIDArr[i] == Shape::CONE)
		{
			Cone bCone(0,0,0, _flm->scaleArr[i][0], _flm->scaleArr[i][2]);
			bCone.SetOpenGLMatrix( _flm->bShapeMatArr[i] );
			bCone.SetColor(r, g, b, a);
			bCone.Draw(false);
		}

		if(isLightOn)
			glEnable(GL_LIGHTING);
		glPopMatrix();
	}
}

void Model3D::DrawBoundingShapes()
{
	//glBegin(GL_TRIANGLES);

	//for(int i=0;i<bVerVec.size(); i+=3)
	//{
	//	glVertex3f(bVerVec[i], bVerVec[i+1], bVerVec[i+2]);
	//}

	//glEnd();

	//return;

	if(_drawBoundingShapes && _flm->numBShapes > 0)
	{
		glPushMatrix();
		
		if(_compoundShape)
		{
			float phyMat[16];
			_compoundShape->GetOrientation(phyMat);
			glMultMatrixf(phyMat);
		}
		else
		{
			glTranslatef(_pos.x, _pos.y, _pos.z);
			glMultMatrixf(_rot.Get());
		}

		bool isLightOn = glIsEnabled(GL_LIGHTING);
		glDisable(GL_LIGHTING);

		for(int i=0;i<_flm->numBShapes;i++)
		{
			if(_flm->shapeIDArr[i] == Shape::BOX)
			{
				Box bBox(0,0,0, _flm->scaleArr[i][0], _flm->scaleArr[i][1], _flm->scaleArr[i][2]);
				bBox.SetOpenGLMatrix( _flm->bShapeMatArr[i] );
				bBox.Draw();
			}
			else if(_flm->shapeIDArr[i] == Shape::CYLINDER)
			{
				Cylinder bCylinder(0,0,0, _flm->scaleArr[i][0], _flm->scaleArr[i][2]);
				bCylinder.SetOpenGLMatrix( _flm->bShapeMatArr[i] );
				bCylinder.Draw();
			}
			else if(_flm->shapeIDArr[i] == Shape::SPHERE)
			{
				//glEnable(GL_LIGHTING);
				Sphere bSphere(0,0,0, _flm->scaleArr[i][0]);
				bSphere.SetOpenGLMatrix( _flm->bShapeMatArr[i] );
				bSphere.Draw();
			}
			else if(_flm->shapeIDArr[i] == Shape::CONE)
			{
				Cone bCone(0,0,0, _flm->scaleArr[i][0], _flm->scaleArr[i][2]);
				bCone.SetOpenGLMatrix( _flm->bShapeMatArr[i] );
				bCone.Draw();
			}
		}

		if(isLightOn)
			glEnable(GL_LIGHTING);
		glPopMatrix();
	}
}

unsigned int Model3D::Draw()
{
	glColor4ub(_r, _g, _b, _a);
	glPushMatrix();

	if(_compoundShape)
	{
		float phyMat[16];
		_compoundShape->GetOrientation(phyMat);
		glMultMatrixf(phyMat);
	}
	else
	{
		glTranslatef(_pos.x, _pos.y, _pos.z);
		glMultMatrixf(_rot.Get());
	}

	if(_drawMode == VBO_MODE)
	{
		glDisable( GL_TEXTURE_2D );
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		if(_drawTexture && _textureID && _uvBufID)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _textureID);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, _uvBufID );
			glTexCoordPointer(2, GL_FLOAT, 0, (void*)0);
		}

		if(_normalBufID)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, _normalBufID );
			glNormalPointer(GL_FLOAT, 0, (void*)0);
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, _vertexBufID );
		glVertexPointer(3, GL_FLOAT, 0, (void*)0);

		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, _indicesBufID);
		glDrawElements(GL_TRIANGLES, _flm->numIndices, _flm->indicesFormat, (void*)0);
		
		if(_wireFrame)
		{
			glDisable(GL_LIGHTING);
			glColor4ub(255, 0, 0, 255);
			glPointSize(3.0f);
			glDrawElements(GL_POINTS, _flm->numIndices, _flm->indicesFormat, (void*)0);

			//glDisable(GL_LIGHTING);
			//glColor4ub(0, 0, 255, 255);
			//glLineWidth(1.0f);
			//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			//glDrawElements(GL_TRIANGLES, _flm->numIndices, _flm->indicesFormat, (void*)0);
		}

		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable( GL_TEXTURE_2D );

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
		glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	}
	else if(_drawMode == IMMEDIATE_MODE)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisable(GL_TEXTURE_2D);

		if(_drawTexture && !_wireFrame && _textureID && _flm->uvBuf)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _textureID);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, _flm->uvBuf);
		}

		if(_flm->normalBuf)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, 0, _flm->normalBuf);
		}

		if(_wireFrame)
			glEnable(GL_LIGHTING);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, _flm->vertexBuf);
		glDrawElements(GL_TRIANGLES, _flm->numIndices, _flm->indicesFormat, _flm->indicesBuf);
		
		if(_wireFrame)
		{
			bool lightOn = glIsEnabled(GL_LIGHTING);

			glDisable(GL_LIGHTING);
			glColor4ub(255, 0, 0, 255);
			glPointSize(5.0f);
			glDrawElements(GL_POINTS, _flm->numIndices, _flm->indicesFormat, _flm->indicesBuf);

			glColor4ub(0, 0, 255, 255);
			glLineWidth(1.0f);
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			glDrawElements(GL_TRIANGLES, _flm->numIndices, _flm->indicesFormat, _flm->indicesBuf);
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

			if(lightOn)
				glEnable(GL_LIGHTING);
			else
				glDisable(GL_LIGHTING);
		}

		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	else if(_drawMode == DISPLAY_LIST_MODE)
	{
		glCallList(_displayListID);
	}

	glPopMatrix();

	return _flm->numIndices/3;
}


void Model3D::AddConnectedVIndex(int val, vector<int>*  connectedVIndexArr, int* indicesBuf, int totIndiLen, unsigned char* existArr)
{
	int indicesBufSize = totIndiLen;

	for(int i=0; i<indicesBufSize; i+=3)
	{		
		int val1 = indicesBuf[i+0];
		int val2 = indicesBuf[i+1];
		int val3 = indicesBuf[i+2];
		
		if(val1 == val)
		{
			if(existArr[val2] == 0)
			{
				connectedVIndexArr->push_back(val2);
				existArr[val2] = 1;
			}

			if(existArr[val3] == 0)
			{
				connectedVIndexArr->push_back(val3);
				existArr[val3] = 1;
			}
		}
		else if(val2 == val)
		{
			if(existArr[val1] == 0)
			{
				connectedVIndexArr->push_back(val1);
				existArr[val1] = 1;
			}

			if(existArr[val3] == 0)
			{
				connectedVIndexArr->push_back(val3);
				existArr[val3] = 1;
			}
		}
		else if(val3 == val)
		{
			if(existArr[val1] == 0)
			{
				connectedVIndexArr->push_back(val1);
				existArr[val1] = 1;
			}

			if(existArr[val2] == 0)
			{
				connectedVIndexArr->push_back(val2);
				existArr[val2] = 1;
			}
		}
	}
}

//def CreateBShapesForObject(obj):
//	
//	#global indicesBuf
//	#global indicesBufSize
//	#global existArr
//		
//	indicesBuf = bpyUtils.GetVertexIndicesArray_AsTriFaces(obj)
//	indicesBufSize = len(indicesBuf)
//	maxVIndex = bpyUtils.GetMaxValueInBuf(indicesBuf, indicesBufSize)
//	existArr = [-1]*(maxVIndex+1)
//
//	loop = 1
//	while(loop == 1):
//		loop = 0
//		startIndex = 0
//		for i in range(0,len(indicesBuf)):
//			if(existArr[indicesBuf[i]] < 0):
//				startIndex = i
//				loop = 1
//				break
//			
//		if(loop == 0):
//			break
//		
//		connectedVIndexArr = []
//		connectedVIndexArr.append(indicesBuf[startIndex])
//		existArr[indicesBuf[startIndex]] = 1
//		
//		chainLen = len(connectedVIndexArr)
//		count = 0
//		while(count < chainLen):
//			AddConnectedVIndex(connectedVIndexArr[count], connectedVIndexArr, indicesBuf, existArr)
//			chainLen = len(connectedVIndexArr)
//			count += 1
//		
//		connectedVertex = []	
//		for i in range(0,len(connectedVIndexArr)):
//			vIndex = connectedVIndexArr[i]
//			connectedVertex.append(obj.data.vertices[vIndex].co[0])
//			connectedVertex.append(obj.data.vertices[vIndex].co[1])
//			connectedVertex.append(obj.data.vertices[vIndex].co[2])
//		
//		verticesArr =  connectedVertex
//		bShapes = BoundingShapesLib.BoundingShapes(verticesArr, len(verticesArr))
//		bShapes.CalcAllBoundingShapes()
//		bShapes.CreateBestBoundingShape_ByAppendObjOri(obj)
//        #bShapes.CreateBestBoundingShape(obj)
//		del verticesArr


float* Model3D::GetConnectedVertex(int triIndex, int* size)
{
	int* indicesBuf = new int[_flm->numIndices];

	for(int i=0; i<_flm->numIndices; i++)
	{
		if(_flm->indicesFormat == GL_UNSIGNED_BYTE)			indicesBuf[i] = _flm->indicesBuf[i];
		else if(_flm->indicesFormat == GL_UNSIGNED_SHORT)	indicesBuf[i] = ((unsigned short*)(_flm->indicesBuf))[i];
		else if(_flm->indicesFormat == GL_UNSIGNED_INT)		indicesBuf[i] = ((unsigned int*)(_flm->indicesBuf))[i];
	}

	int indicesBufSize = _flm->numIndices;

	int maxIndexVal = 0;
	for(int i=0; i<indicesBufSize; i++)
		if(maxIndexVal < indicesBuf[i])
			maxIndexVal = indicesBuf[i];

	unsigned char* existArr = new unsigned char[maxIndexVal];
	memset(existArr, 0, maxIndexVal);

	vector<int> connectedVIndexArr;
	connectedVIndexArr.push_back(indicesBuf[triIndex*3]);
	existArr[indicesBuf[triIndex*3]] = 1;
	
	for(int i=0;i<connectedVIndexArr.size(); i++)
		AddConnectedVIndex(connectedVIndexArr[i], &connectedVIndexArr, indicesBuf, indicesBufSize, existArr);

	float* verBuf = new float[connectedVIndexArr.size() * 3];

	int index = 0;
	for(int i=0; i<connectedVIndexArr.size(); i++)
	{
		index = connectedVIndexArr[i];

		verBuf[3*i+0] = _flm->vertexBuf[3*index + 0];
		verBuf[3*i+1] = _flm->vertexBuf[3*index + 1];
		verBuf[3*i+2] = _flm->vertexBuf[3*index + 2];
	}


	delete[] indicesBuf;
	delete[] existArr;

	size[0] = connectedVIndexArr.size() * 3;
	return verBuf;
}

void Model3D::DrawWithUniqueColors()
{
	unsigned int colorBufSize = _flm->numIndices * 4;
	unsigned char* colorBuf = (unsigned char*) malloc( colorBufSize );
	memset(colorBuf, 255, colorBufSize);

	int r = 0;
	int g = 0;
	int b = 0;

	int inc = 1;

	for(int i=0;i<_flm->numIndices*4;i+=12)
	{
		colorBuf[i+0] = r;
		colorBuf[i+1] = g;
		colorBuf[i+2] = b;
		colorBuf[i+3] = 255;

		colorBuf[i+4] = r;
		colorBuf[i+5] = g;
		colorBuf[i+6] = b;
		colorBuf[i+7] = 255;

		colorBuf[i+8] = r;
		colorBuf[i+9] = g;
		colorBuf[i+10] = b;
		colorBuf[i+11] = 255;

		r+=inc;
		if(r >= 255)
		{
			r = 0;
			g+=inc;
			if(g >= 255)
			{
				g = 0;
				b+=inc;
				if(b >= 255)
				{
					b = 0;
				}
			}
		}
	}

	float* verBuf = new float[_flm->numIndices * 3];

	int index = 0;
	for(int i=0; i<_flm->numIndices; i++)
	{
		if(_flm->indicesFormat == GL_UNSIGNED_BYTE)
			index = _flm->indicesBuf[i];
		else if(_flm->indicesFormat == GL_UNSIGNED_SHORT)
			index = ((unsigned short*)(_flm->indicesBuf))[i];
		else if(_flm->indicesFormat == GL_UNSIGNED_INT)
			index = ((unsigned int*)(_flm->indicesBuf))[i];

		verBuf[3*i+0] = _flm->vertexBuf[3*index + 0];
		verBuf[3*i+1] = _flm->vertexBuf[3*index + 1];
		verBuf[3*i+2] = _flm->vertexBuf[3*index + 2];
	}

	bool lightOn = glIsEnabled( GL_LIGHTING);
	glDisable(GL_LIGHTING);
	glPushMatrix();

	glTranslatef(_pos.x, _pos.y, _pos.z);
	glMultMatrixf(_rot.Get());

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, verBuf);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colorBuf);
	glDrawArrays(GL_TRIANGLES, 0, _flm->numIndices);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();

	delete[] verBuf;
	free(colorBuf);

	if(lightOn)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
}

void Model3D::CreateBufferObects()
{
	_helper->CreateBufferObects(_flm, &_vertexBufID, &_uvBufID, &_normalBufID, &_indicesBufID);

	Delete_Vertex_UV_Normal_Indices();
}

void Model3D::CreateCallList()
{
	if(_displayListID == 0)
	{
		_displayListID = _helper->CreateCallList(_flm, _textureID);
		Delete_Vertex_UV_Normal_Indices();
	}
}

void Model3D::Delete_Vertex_UV_Normal_Indices()
{
	_flm->Delete_Vertex_UV_Normal_Indices();
}

Model3D::~Model3D()
{
	if(_textureID != 0)
	{
		glDeleteTextures(1, &_textureID);
		_textureID = 0;
	}

	if(_vertexBufID)
		glDeleteBuffersARB(1, &_vertexBufID);
	if(_uvBufID)
		glDeleteBuffersARB(1, &_uvBufID);
	if(_normalBufID)
		glDeleteBuffersARB(1, &_normalBufID);
	if(_indicesBufID)
		glDeleteBuffersARB(1, &_indicesBufID);

	if(_displayListID)
		glDeleteLists(_displayListID, 1);

	if(_flm)
	{
		delete _flm;
		_flm = NULL;
	}

	if(_helper)
	{
		delete _helper;
		_helper = NULL;
	}

	if(_compoundShape)
	{
		delete _compoundShape;
		_compoundShape = NULL;
	}
}
