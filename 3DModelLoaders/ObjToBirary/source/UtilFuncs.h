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

	static void scanFace_VN(char* str,
		int* v1, int* n1,
		int* v2, int* n2,
		int* v3, int* n3
	);

	static void scanFace_VT(char* str,
		int* v1, int* t1,
		int* v2, int* t2,
		int* v3, int* t3
	);

	static void scanFace_V(char* str, int* x, int* y, int* z);

	static void scan_vertex(char* str, float* x, float* y, float* z);
	static void scan_uv(char* str, float* x, float* y);
	static void scan_normal(char* str, float* x, float* y, float* z);

	static double atof_naive(const char *p);
	static unsigned int atoi_naive(const char *p);
};
