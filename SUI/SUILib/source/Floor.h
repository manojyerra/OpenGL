#ifndef Floor_H
#define Floor_H

class Floor
{
private:
	static bool _visible;
	static bool _axisVisible;
	static bool _gridVisible;
	static bool _gridLinesVisible;

public:
	static void SetVisible(bool visible);
	static void SetAxisVisible(bool axisVisible);
	static void SetGridVisible(bool floorVisible);
	static void SetGridLinesVisible(bool linesVisible);

	static void Draw();
};

#endif
