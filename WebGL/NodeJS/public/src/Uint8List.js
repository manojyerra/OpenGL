class Uint8List
{
	constructor(capacity)
	{
		this._capacity = capacity;
		this.arr = new Uint8Array(this._capacity);
		this._size = 0;
	}
	
	reCreateMem()
	{
		var newCapacity = 2*this._capacity;

		var newArr = new Uint8Array(newCapacity);
		newArr.set(this.arr);

		this.arr = null;
		this.arr = newArr;
		
		this._capacity = newCapacity;
	}	

	push(val)
	{
		this.arr[this._size] = val;
		this._size++;
		
		if(this._size >= this._capacity)
			this.reCreateMem();
	}
}


//var endIndex = this._colorArr._size;
//var arr = this._colorArr.arr.slice(0,endIndex);
//console.log(this._colorArr);
