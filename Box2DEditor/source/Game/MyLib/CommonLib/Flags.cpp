#include "Flags.h"
#include "Sprite.h"

Flags* Flags::_ref = 0;

Flags::Flags()
{
	_map.add("Albania", NULL);
	_map.add("Algeria", NULL);
	_map.add("Angola", NULL);
	_map.add("Antigua and Barbuda", NULL);
	_map.add("Argentina", NULL);
	_map.add("Armenia", NULL);
	_map.add("Aruba", NULL);
	_map.add("Australia", NULL);
	_map.add("Austria", NULL);
	_map.add("Azerbaijan", NULL);
	_map.add("Bahamas", NULL);
	_map.add("Bahrain", NULL);
	_map.add("Bangladesh", NULL);
	_map.add("Belarus", NULL);
	_map.add("Belgium", NULL);
	_map.add("Belize", NULL);
	_map.add("Benin", NULL);
	_map.add("Bolivia", NULL);
	_map.add("Bosnia and Herzegovina", NULL);
	_map.add("Botswana", NULL);
	_map.add("Brazil", NULL);
	_map.add("Bulgaria", NULL);
	_map.add("Burkina Faso", NULL);
	_map.add("Cambodia", NULL);
	_map.add("Cameroon", NULL);
	_map.add("Canada", NULL);
	_map.add("Cape Verde", NULL);
	_map.add("Chile", NULL);
	_map.add("China", NULL);
	_map.add("Colombia", NULL);
	_map.add("Costa Rica", NULL);
	_map.add("Cote d Ivoire", NULL);
	_map.add("Croatia", NULL);
	_map.add("Cyprus", NULL);
	_map.add("Czech Republic", NULL);
	_map.add("Denmark", NULL);
	_map.add("Dominican Republic", NULL);
	_map.add("Ecuador", NULL);
	_map.add("Egypt", NULL);
	_map.add("El Salvador", NULL);
	_map.add("Estonia", NULL);
	_map.add("Fiji", NULL);
	_map.add("Finland", NULL);
	_map.add("France", NULL);
	_map.add("Gabon", NULL);
	_map.add("Germany", NULL);
	_map.add("Ghana", NULL);
	_map.add("Greece", NULL);
	_map.add("Guatemala", NULL);
	_map.add("Guinea-Bissau", NULL);
	_map.add("Haiti", NULL);
	_map.add("Honduras", NULL);
	_map.add("Hong Kong", NULL);
	_map.add("Hungary", NULL);
	_map.add("Iceland", NULL);
	_map.add("India", NULL);
	_map.add("Indonesia", NULL);
	_map.add("Iran", NULL);
	_map.add("Ireland", NULL);
	_map.add("Israel", NULL);
	_map.add("Italy", NULL);
	_map.add("Jamaica", NULL);
	_map.add("Japan", NULL);
	_map.add("Jordan", NULL);
	_map.add("Kazakhstan", NULL);
	_map.add("Kenya", NULL);
	_map.add("Kuwait", NULL);
	_map.add("Kyrgyzstan", NULL);
	_map.add("Laos", NULL);
	_map.add("Latvia", NULL);
	_map.add("Lebanon", NULL);
	_map.add("Liechtenstein", NULL);
	_map.add("Lithuania", NULL);
	_map.add("Luxembourg", NULL);
	_map.add("Macedonia [FYROM]", NULL);
	_map.add("Malaysia", NULL);
	_map.add("Mali", NULL);
	_map.add("Malta", NULL);
	_map.add("Mauritius", NULL);
	_map.add("Mexico", NULL);
	_map.add("Moldova", NULL);
	_map.add("Morocco", NULL);
	_map.add("Mozambique", NULL);
	_map.add("Myanmar [Burma]", NULL);
	_map.add("Namibia", NULL);
	_map.add("Nepal", NULL);
	_map.add("Netherlands", NULL);
	_map.add("Netherlands Antilles", NULL);
	_map.add("New Zealand", NULL);
	_map.add("Nicaragua", NULL);
	_map.add("Niger", NULL);
	_map.add("Nigeria", NULL);
	_map.add("Norway", NULL);
	_map.add("Oman", NULL);
	_map.add("Pakistan", NULL);
	_map.add("Panama", NULL);
	_map.add("Papua New Guinea", NULL);
	_map.add("Paraguay", NULL);
	_map.add("Peru", NULL);
	_map.add("Philippines", NULL);
	_map.add("Poland", NULL);
	_map.add("Portugal", NULL);
	_map.add("Qatar", NULL);
	_map.add("Romania", NULL);
	_map.add("Russia", NULL);
	_map.add("Rwanda", NULL);
	_map.add("Saudi Arabia", NULL);
	_map.add("Senegal", NULL);
	_map.add("Serbia", NULL);
	_map.add("Singapore", NULL);
	_map.add("Slovakia", NULL);
	_map.add("Slovenia", NULL);
	_map.add("South Africa", NULL);
	_map.add("South Korea", NULL);
	_map.add("Spain", NULL);
	_map.add("Sri Lanka", NULL);
	_map.add("Sweden", NULL);
	_map.add("Switzerland", NULL);
	_map.add("Taiwan", NULL);
	_map.add("Tajikistan", NULL);
	_map.add("Tanzania", NULL);
	_map.add("Thailand", NULL);
	_map.add("Togo", NULL);
	_map.add("Trinidad and Tobago", NULL);
	_map.add("Tunisia", NULL);
	_map.add("Turkey", NULL);
	_map.add("Turkmenistan", NULL);
	_map.add("Uganda", NULL);
	_map.add("Ukraine", NULL);
	_map.add("United Arab Emirates", NULL);
	_map.add("United Kingdom", NULL);
	_map.add("United States", NULL);
	_map.add("Uruguay", NULL);
	_map.add("Uzbekistan", NULL);
	_map.add("Venezuela", NULL);
	_map.add("Vietnam", NULL);
	_map.add("Yemen", NULL);
	_map.add("Zambia", NULL);
	_map.add("Zimbabwe", NULL);

	char path[256];

	for(int i=0;i<_map.size();i++)
	{
		string keyVal = _map.getKeyByIndex(i);
		sprintf(path, "Flags/%s.png",keyVal.c_str());
		_map.set(keyVal, new Sprite(path, 0,0,64,64));
	}
}

Flags* Flags::GetInstance()
{
	if(_ref == NULL)
		_ref = new Flags();

	return _ref;
}

void Flags::Draw(std::string countryName,  int x, int y, int w, int h)
{
	bool find = false;

	Sprite* sprite = (Sprite*)(_map.get(countryName, &find));

	if(sprite)
	{
		sprite->SetBounds(x, y, w, h);
		sprite->Draw();
	}
}

void Flags::DeleteInstance()
{
	if(_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}

Flags::~Flags()
{
	for(int i=0;i<_map.size();i++)
	{
		Sprite* sprite = (Sprite*)_map.getValByIndex(i);
		delete sprite;
	}	
}

