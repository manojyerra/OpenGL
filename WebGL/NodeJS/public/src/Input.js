var inputInstanceCount = 0;

class InputState
{
	constructor(curr, prev, store)
	{
		this.curr = curr;
		this.prev = prev;
		this.store = store;
	}
}

class Input
{
	constructor()
	{
		inputInstanceCount++;
		
		if(inputInstanceCount > 1)
		{
			throw "More than one object creation is not allowed for Input Class";
		}
		
		this.setKeyCodes();
		
		this.currKeys = Array(256).fill(false);
		this.prevKeys = Array(256).fill(false);
		this.storeKeys = Array(256).fill(false);
		
		this.mouseX = new InputState(0,0,0);
		this.mouseY = new InputState(0,0,0);

		this.leftMouseDown = new InputState(false, false, false);
		this.rightMouseDown = new InputState(false, false, false);
		this.middleMouseDown = new InputState(false, false, false);
		
		this.scrollVal = new InputState(0,0,0);
	}
	
	//public methods...
	
	update()
	{
		this.mouseX.prev = this.mouseX.curr;
		this.mouseX.curr = this.mouseX.store;
		
		this.mouseY.prev = this.mouseY.curr;
		this.mouseY.curr = this.mouseY.store;
		
		this.leftMouseDown.prev = this.leftMouseDown.curr;
		this.leftMouseDown.curr = this.leftMouseDown.store;
		
		this.rightMouseDown.prev = this.rightMouseDown.curr;
		this.rightMouseDown.curr = this.rightMouseDown.store;

		this.middleMouseDown.prev = this.middleMouseDown.curr;
		this.middleMouseDown.curr = this.middleMouseDown.store;
		
		this.scrollVal.curr = this.scrollVal.store;
		this.scrollVal.store = 0;
		
		for(var i=0; i<256; i++)
		{
			this.prevKeys[i] = this.currKeys[i];
			this.currKeys[i] = this.storeKeys[i];
		}
	}
	
	isMouseMoved()			{ return (this.mouseX.curr != this.mouseX.prev || this.mouseY.curr != this.mouseY.prev); }
	
	isMousePressed()		{ return (this.leftMouseDown.curr && this.leftMouseDown.prev);  	}
	isMouseReleased()		{ return (!this.leftMouseDown.curr && this.leftMouseDown.prev); 	}
	isMouseClicked()		{ return (this.leftMouseDown.curr && !this.leftMouseDown.prev); 	}
	isMouseDragged()		{ return (this.isMouseMoved() && this.isMousePressed());			}

	isRightMousePressed()	{ return (this.rightMouseDown.curr && this.rightMouseDown.prev);	}
	isRightMouseReleased()	{ return (!this.rightMouseDown.curr && this.rightMouseDown.prev);	}
	isRightMouseClicked()	{ return (this.rightMouseDown.curr && !this.rightMouseDown.prev);	}
	isRightMouseDragged()	{ return (this.isMouseMoved() && this.isRightMousePressed());		}
	
	isMiddleMousePressed()	{ return (this.middleMouseDown.curr && this.middleMouseDown.prev);	}
	isMiddleMouseReleased()	{ return (!this.middleMouseDown.curr && this.middleMouseDown.prev);	}
	isMiddleMouseClicked()	{ return (this.middleMouseDown.curr && !this.middleMouseDown.prev);	}
	isMiddleMouseDragged()	{ return (this.isMouseMoved() && this.isMiddleMousePressed());		}
	
	isScrollDown()			{ return (this.scrollVal.curr < 0); }
	isScrollUp()			{ return (this.scrollVal.curr > 0); }
	
	isKeyTyped(key)			{ return !this.prevKeys[key] && this.currKeys[key];	}
	isKeyReleased(key)		{ return this.prevKeys[key] && !this.currKeys[key];	}
	isKeyPressed(key)		{ return this.prevKeys[key] && this.currKeys[key];	}
	
	
	//private methods...
	
	setKeyCodes()
	{
		this.backspace = 8;
		this.tab = 9;
		this.enter = 13;
		this.shift = 16;
		this.ctrl = 17;
		this.alt = 18;
		this.pause = 19;
		this.capsLock = 20;
		this.escape = 27;
		this.space = 32;
		this.pageUp = 33;
		this.pageDown = 34;
		this.end = 35;
		this.home = 36;
		this.left = 37;
		this.up = 38;
		this.right = 39;
		this.down = 40;
		this.insert = 45;
		this.del = 46;
		this.num_0 = 48;
		this.num_1 = 49;
		this.num_2 = 50;
		this.num_3 = 51;
		this.num_4 = 52;
		this.num_5 = 53;
		this.num_6 = 54;
		this.num_7 = 55;
		this.num_8 = 56;
		this.num_9 = 57;
		this.a = 65;
		this.b = 66;
		this.c = 67;
		this.d = 68;
		this.e = 69;
		this.f = 70;
		this.g = 71;
		this.h = 72;
		this.i = 73;
		this.j = 74;
		this.k = 75;
		this.l = 76;
		this.m = 77;
		this.n = 78;
		this.o = 79;
		this.p = 80;
		this.q = 81;
		this.r = 82;
		this.s = 83;
		this.t = 84;
		this.u = 85;
		this.v = 86;
		this.w = 87;
		this.x = 88;
		this.y = 89;
		this.z = 90;
		this.leftWindowKey = 91;
		this.rightWindowKey = 92;
		this.selectKey = 93;
		this.numpad_0 = 96;
		this.numpad_1 = 97;
		this.numpad_2 = 98;
		this.numpad_3 = 99;
		this.numpad_4 = 100;
		this.numpad_5 = 101;
		this.numpad_6 = 102;
		this.numpad_7 = 103;
		this.numpad_8 = 104;
		this.numpad_9 = 105;
		this.multiply = 106;
		this.add = 107;
		this.subtract = 109;
		this.decimalPoint = 110;
		this.divide = 111;
		this.f1 = 112;
		this.f2 = 113;
		this.f3 = 114;
		this.f4 = 115;
		this.f5 = 116;
		this.f6 = 117;
		this.f7 = 118;
		this.f8 = 119;
		this.f9 = 120;
		this.f10 = 121;
		this.f11 = 122;
		this.f12 = 123;
		this.numLock = 144;
		this.scrollLock = 145;
		this.semiColon = 186;
		this.equalSign = 187;
		this.comma = 188;
		this.dash = 189;
		this.period = 190;
		this.forwardSlash = 191;
		this.graveAccent = 192;
		this.openBracket = 219;
		this.backSlash = 220;
		this.closeBraket = 221;
		this.singleQuote = 222;
	}
}


var input = new Input();