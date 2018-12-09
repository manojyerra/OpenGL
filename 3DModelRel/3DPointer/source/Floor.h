#ifndef Floor_H
#define Floor_H

class Floor
{
private:
	bool _visible;
	bool _axisVisible;
	bool _gridVisible;
	bool _gridLinesVisible;

public:
	Floor();

	bool IsVisible();
	bool IsAxisVisible();
	bool IsGridVisible();
	bool IsGridLinesVisible();

	void SetVisible(bool visible);
	void SetAxisVisible(bool axisVisible);
	void SetGridVisible(bool floorVisible);
	void SetGridLinesVisible(bool linesVisible);

	void Draw();
};

#endif
