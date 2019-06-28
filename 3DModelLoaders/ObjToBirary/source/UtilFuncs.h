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
};
