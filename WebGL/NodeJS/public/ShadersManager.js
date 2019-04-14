
class ShadersManager
{
	constructor(name, level) {
        this.name = name;
        this.level = level;
    }

    greet() {
        return `${this.name} says hello.`;
    }
	
	add()
	{
		var temp = this.level + 20;
		
		return temp;
	}
}
















