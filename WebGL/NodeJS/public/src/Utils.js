function sleep(ms)
{
	return new Promise(resolve => setTimeout(resolve, ms));
}
		
async function loadTextFile(url)
{	
	var request = new XMLHttpRequest();
	request.open('GET', url + '?please-dont-cache=' + Math.random(), true);
	
	request.onload = function (text){
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

