#ifndef VectorByte_H
#define VectorByte_H

#include "Prim.h"
#include "Def.h"

class VectorByte
{
private:
	int _size;
	int _capacity;
	int _increment;

public:
	byte* arr;    

	VectorByte();  
	~VectorByte();

	void Clear();
	void add(VectorByte* vecByte);
	void add(unsigned char b);
	void addShort(short val);
	void addInt(int val);
	void addFloat(float val);
	void addVertex(Vertex* vertex);

	//***************  Pre Add *********************//

	void preAddFloat(float val);
	void preAddInt(int val);
	int size();
};


#endif