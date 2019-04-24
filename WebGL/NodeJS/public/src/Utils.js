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

async function loadArrayBuffer(url)
{	
	var request = new XMLHttpRequest();
	request.open('GET', url + '?please-dont-cache=' + Math.random(), true);
	request.responseType = 'arraybuffer';
		
	request.onload = function() {
		request.onload = null;
	};
	
	request.send();

	while(request.onload) {
		await this.sleep(2);
	}
	
	if (request.status < 200 || request.status > 299) {
		cosole.error('Error: HTTP Status ' + request.status + ' on resource ' + url);
		return null;
	}
	
	return request.response;
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
	
	if (request.status < 200 || request.status > 299) {
		console.error('Error: HTTP Status ' + request.status + ' on resource ' + url);
		return null;
	}
	
	var blob = new Blob([request.response], {type: 'text/html'});
	var text = await (new Response(blob)).text();

	return text;
}

async function loadTexture(texturePath)
{	
	var img = new Image();
	img.src = texturePath;
	
	img.onload = function() {
		img.onload = null;
	};

	while(img.onload) {	
		await this.sleep(2);
	}
	
	return img;
}

