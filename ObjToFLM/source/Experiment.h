#include <vector>
using namespace std;

class Experiment
{
private:

public:
	Experiment();
	~Experiment();

	static short ConvFloatToShort(float val);
	static float ConvShortToFloat(short val);

	static vector<unsigned char> GetFloatBits(float floatVal);
	static void PrintFloatBits(float val);
};
