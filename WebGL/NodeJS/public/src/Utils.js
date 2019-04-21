function sleep(ms)
{
	return new Promise(resolve => setTimeout(resolve, ms));
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

function generateGLTexureID(img)
{
	var textureID = gl.createTexture();
	gl.bindTexture(gl.TEXTURE_2D, textureID);
	
	gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
	gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, img);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
	
	return textureID;
}

function randomInt(range)
{	
	return Math.floor(Math.random() * range);
}
