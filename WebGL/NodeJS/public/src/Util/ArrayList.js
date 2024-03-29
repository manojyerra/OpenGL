class ArrayList
{
	static get Uint8Array() 		{ return 1; }
	static get Uint8ClampedArray()	{ return 2; }
	static get Uint16Array() 		{ return 3; }
	static get Uint32Array() 		{ return 4; }

	static get Int8Array() 		{ return 5; }
	static get Int16Array()		{ return 6; }
	static get Int32Array() 	{ return 7; }

	static get Float32Array() 	{ return 8; }
	static get Float64Array() 	{ return 9; }
	
	
	constructor(capacity, type)
	{
		this._capacity = capacity;	
		this._type = type;
		this._size = 0;
		
		this._arr = this.createArray( this._capacity, this._type);
	}
	
	createArray(capacity, type)
	{
		if(capacity < 1)
			throw "ArrayList capactity should be >= 1";
		
		if(type == ArrayList.Uint8Array)				return new Uint8Array(capacity);
		else if(type == ArrayList.Uint8ClampedArray)	return new Uint8ClampedArray(capacity);
		else if(type == ArrayList.Uint16Array)			return new Uint16Array(capacity);
		else if(type == ArrayList.Uint32Array)			return new Uint32Array(capacity);
			
		else if(type == ArrayList.Int8Array)			return new Int8Array(capacity);
		else if(type == ArrayList.Int16Array)			return new Int16Array(capacity);
		else if(type == ArrayList.Int32Array)			return new Int32Array(capacity);
		
		else if(type == ArrayList.Float16Array)			return new Float16Array(capacity);
		else if(type == ArrayList.Float32Array)			return new Float32Array(capacity);
		
		throw "Invalid ArrayList type";
		
		return null;
	}
	
	push(val)
	{
		if(this._size >= this._capacity)
			this.reCreateMem();
		
		this._arr[this._size] = val;
		this._size++;		
	}
	
	array()
	{
		return this._arr;
	}
	
	size()
	{
		return this._size;
	}
	
	capacity()
	{
		return this._capacity;
	}
	
	sliceToSize()
	{
		this._arr = this._arr.slice(0,this._size);
		this._capacity = this._size;
	}
	
	//private methods...
	
	reCreateMem()
	{
		var newCapacity = 2*this._capacity;

		var newArr = this.createArray(newCapacity, this._type);
		newArr.set(this._arr);

		this._arr = null;
		this._arr = newArr;
		
		this._capacity = newCapacity;
	}		
}
