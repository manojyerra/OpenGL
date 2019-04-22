var DEG_RAD = 0.0174532925194;
var RAD_DEG = 57.295780490443;
var PI_VAL = 3.1415926;


function sleep(ms)
{
	return new Promise(resolve => setTimeout(resolve, ms));
}

function randomInt(range)
{	
	return Math.floor(Math.random() * range);
}
		
async function loadTextFile(url)
{	
	var request = new XMLHttpRequest();
	request.open('GET', url + '?please-dont-cache=' + Math.random(), true);
	request.responseType = 'blob';
		
	request.onload = function() {
		request.onload = null;
	};
	
	request.send();

	while(request.onload) {
		await this.sleep(2);
	}
	
	var text = null;
	
	if (request.status < 200 || request.status > 299) {
		text = 'Error: HTTP Status ' + request.status + ' on resource ' + url;
	}
	else {
		var blob = new Blob([request.response], {type: 'text/html'});
		var text = await (new Response(blob)).text();
		//console.log('text : ',text);
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

