#include "Meterial.h"

Meterial* Meterial::_ref = 0;

Meterial::Meterial()
{
	massArr = new float[28];

	massArr[0] = 0.0f;		//Static Object
	massArr[1] = 270.0f;	//Aluminium
	massArr[2] = 70.0f;		//Books
	massArr[3] = 850.0f;	//Brass
	massArr[4] = 240.0f;	//Brick
	massArr[5] = 830.0f;	//Bronze
	massArr[6] = 150.0f;	//Cement
	massArr[7] = 90.0f;		//Coal
	massArr[8] = 100.0f;	//Concrete 
	massArr[9] = 870.0f;	//Copper
	massArr[10] = 10.0f;	//Cork
	massArr[11] = 160.0f;	//Glass
	massArr[12] = 1940.0f;	//Gold
	massArr[13] = 270.0f;	//Granite
	massArr[14] = 720.0f;	//Iron
	massArr[15] = 1140.0f;	//Lead
	massArr[16] = 60.0f;	//Lime
	massArr[17] = 180.0f;	//Magnesium
	massArr[18] = 1360.0f;	//Mercury
	massArr[19] = 170.0f;	//Mud
	massArr[20] = 0.9f;		//Plaster
	massArr[21] = 0.4f;		//Plastic
	massArr[22] = 70.0f;	//Plywood
	massArr[23] = 160.0f;	//Sand
	massArr[24] = 10.0f;	//Snow
	massArr[25] = 160.0f;	//Soils
	massArr[26] = 780.0f;	//Steel
	massArr[27] = 230.0f;	//Stone
	massArr[28] = 720.0f;	//Zinc
}

Meterial* Meterial::GetInstance()
{
	if(_ref == 0)
		_ref = new Meterial();
	return _ref;
}

Meterial::~Meterial()
{
	if(massArr)
	{
		delete[] massArr;
		massArr = 0;
	}
}

void Meterial::DeleteInstance()
{
	if(_ref)
	{
		delete _ref;
		_ref = 0;
	}
}



	//meterialChoice->Add("Select Meterial");
	//meterialChoice->Add("Plastic");
	//meterialChoice->Add("Plaster");
	//meterialChoice->Add("Snow");
	//meterialChoice->Add("Cork");
	//meterialChoice->Add("Lime");
	//meterialChoice->Add("Plywood");
	//meterialChoice->Add("Books");
	//meterialChoice->Add("Coal");
	//meterialChoice->Add("Concrete"); 
	//meterialChoice->Add("Cement");
	//meterialChoice->Add("Sand");
	//meterialChoice->Add("Soils");
	//meterialChoice->Add("Glass");
	//meterialChoice->Add("Mud");
	//meterialChoice->Add("Magnesium");
	//meterialChoice->Add("Stone");
	//meterialChoice->Add("Brick");
	//meterialChoice->Add("Aluminium");
	//meterialChoice->Add("Granite");
	//meterialChoice->Add("Iron");
	//meterialChoice->Add("Zinc");
	//meterialChoice->Add("Steel");
	//meterialChoice->Add("Bronze");
	//meterialChoice->Add("Brass");
	//meterialChoice->Add("Copper");
	//meterialChoice->Add("Lead");
	//meterialChoice->Add("Mercury");
	//meterialChoice->Add("Gold");