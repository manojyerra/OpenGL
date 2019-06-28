#include <vector>
using namespace std;

class UtilFuncs
{
private:

public:

	static short ConvFloatToShort(float val);
	static float ConvShortToFloat(short val);

	static vector<unsigned char> GetFloatBits(float floatVal);
	static void PrintFloatBits(float val);

	static void Normalize(float* nx, float* ny, float* nz);

	static void scanFace_VTN(char* str,
		int* v1, int* t1, int* n1,
		int* v2, int* t2, int* n2,
		int* v3, int* t3, int* n3
		);

	static void scan_vertex(char* str, float* x, float* y, float* z);
	static void scan_uv(char* str, float* x, float* y);
	static void scan_normal(char* str, float* x, float* y, float* z);


	//sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v[0], &t[0], &n[0], &v[1], &t[1], &n[1], &v[2], &t[2], &n[2]);

};
