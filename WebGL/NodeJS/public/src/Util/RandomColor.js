var RandomColor_colorVec = Array(0);

class RandomColor
{
	constructor()
	{
		if(RandomColor_colorVec.length == 0)
		{
			RandomColor_colorVec.push(0x9a4faaff);
			RandomColor_colorVec.push(0x9a4faaff);
			RandomColor_colorVec.push(0x587f0aff);
			RandomColor_colorVec.push(0x37ad77ff);
			RandomColor_colorVec.push(0xfe76e2ff);
			RandomColor_colorVec.push(0x06f53eff);
			RandomColor_colorVec.push(0xa5306dff);
			RandomColor_colorVec.push(0x70de33ff);
			RandomColor_colorVec.push(0xf8ec36ff);
			RandomColor_colorVec.push(0xda1903ff);
			RandomColor_colorVec.push(0x7d63e1ff);
			RandomColor_colorVec.push(0xb2bd31ff);
			RandomColor_colorVec.push(0xa7e2b6ff);
			RandomColor_colorVec.push(0xf7cc93ff);
			RandomColor_colorVec.push(0x85641cff);
			RandomColor_colorVec.push(0x65340eff);
			RandomColor_colorVec.push(0xcd0e69ff);
			RandomColor_colorVec.push(0x0ee4cbff);
			RandomColor_colorVec.push(0xae98b0ff);
			RandomColor_colorVec.push(0x4b337dff);
			RandomColor_colorVec.push(0x3ce37aff);
			RandomColor_colorVec.push(0xf77a9bff);
			RandomColor_colorVec.push(0x665e8cff);
			RandomColor_colorVec.push(0x6b01b7ff);
			RandomColor_colorVec.push(0x0baf55ff);
			RandomColor_colorVec.push(0x0cfe8bff);
			RandomColor_colorVec.push(0xbeaa01ff);
			RandomColor_colorVec.push(0x46bcbbff);
			RandomColor_colorVec.push(0x9b8c64ff);
			RandomColor_colorVec.push(0x16fb91ff);
			RandomColor_colorVec.push(0x4be9b2ff);
			RandomColor_colorVec.push(0x4c7ef8ff);
			RandomColor_colorVec.push(0xd6bb02ff);
			RandomColor_colorVec.push(0x305053ff);
			RandomColor_colorVec.push(0xd24ecdff);
			RandomColor_colorVec.push(0x01e9f9ff);
			RandomColor_colorVec.push(0xcad871ff);
			RandomColor_colorVec.push(0x506736ff);
			RandomColor_colorVec.push(0x8f413aff);
			RandomColor_colorVec.push(0x0bfb70ff);
			RandomColor_colorVec.push(0x941213ff);
			RandomColor_colorVec.push(0xfb3291ff);
			RandomColor_colorVec.push(0x157cb3ff);
			RandomColor_colorVec.push(0x0dd2e6ff);
		}
		
		this._index = randomInt( RandomColor_colorVec.length );
	}
	
	nextColor()
	{
		this._index++;

		if(this._index >= RandomColor_colorVec.length)
			this._index = 0;

		return RandomColor_colorVec[this._index];
	}

	reset()
	{
		this._index = 0;
	}
}