function sleep(ms)
{
	return new Promise(resolve => setTimeout(resolve, ms));
}
		
async function loadTextFile(url)
{	
	var request = new XMLHttpRequest();
	request.open('GET', url + '?please-dont-cache=' + Math.random(), true);
	
	request.onload = function() {
	};
	
	request.send();

	while(request.status == 0) {
		await this.sleep(2);
	}
	
	var text = null;
	
	if (request.status < 200 || request.status > 299) {
		text = 'Error: HTTP Status ' + request.status + ' on resource ' + url;
	}
	else {
		text = request.responseText;
	}

	return text;
}

async function loadTexture(texturePath)
{	
	var img = new Image();
	img.src = texturePath;
	
	img.onload = function()
	{
		//console.log('img loaded');
		img.onload = null;
	};

	while(img.onload) {	
		await this.sleep(2);
	}
	
	//console.log('image width', img.width);
	
	return img;
}

