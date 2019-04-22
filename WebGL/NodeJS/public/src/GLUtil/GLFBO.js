
class GLFBO
{
	constructor(w, h)
	{
		this._w = w;
		this._h = h;
		
		this._fboID = gl.createFramebuffer();
		this.bind();
		this._texID = this.generateEmptyTexture(this._w, this._h);
		this.attachTextureToFBO(this._texID);
		this._depthBufID = this.createDepthBuffer(this._w, this._h);
		this.attachDepthBufferToFBO(this._depthBufID);
		
		this.isFBOCreated();
		this.unBind();
	}
	
	bind()
	{
		gl.bindFramebuffer(gl.FRAMEBUFFER, this._fboID);
	}
	
	unBind()
	{
		gl.bindFramebuffer(gl.FRAMEBUFFER, null);
	}
	
	getTextureID()
	{
		return this._texID;
	}

	generateEmptyTexture(w, h)
	{
		var texID = gl.createTexture();
		gl.bindTexture(gl.TEXTURE_2D, texID);
		gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, w, h, 0, gl.RGBA, gl.UNSIGNED_BYTE, null);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
		gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
		//gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
		//gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
		gl.bindTexture(gl.TEXTURE_2D, null);

		return texID;
	}

	attachTextureToFBO(texID)
	{
		gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, texID, 0);
	}
	
	createDepthBuffer(w, h)
	{
		var rbo = gl.createRenderbuffer();
		gl.bindRenderbuffer(gl.RENDERBUFFER, rbo);
		gl.renderbufferStorage(gl.RENDERBUFFER, gl.DEPTH_COMPONENT16, w, h);
		gl.bindRenderbuffer(gl.RENDERBUFFER, null);

		return rbo;
	}

	attachDepthBufferToFBO(depthBufferID)
	{
		gl.framebufferRenderbuffer(gl.FRAMEBUFFER, gl.DEPTH_ATTACHMENT, gl.RENDERBUFFER, depthBufferID);
	}
	
	isFBOCreated()
	{
		var status = gl.checkFramebufferStatus(gl.FRAMEBUFFER);

		if(status == gl.FRAMEBUFFER_COMPLETE) {
			console.log("FRAMEBUFFER_COMPLETE");
			return true;
		}
		
		if(status == gl.FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {	
			console.log("FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
		}
		else if(status == gl.FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {	
			console.log("FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
		}
		else if(status == gl.FRAMEBUFFER_INCOMPLETE_DIMENSIONS) {	
			console.log("FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
		}
		else if(status == gl.FRAMEBUFFER_UNSUPPORTED) {	
			console.log("FRAMEBUFFER_UNSUPPORTED");
		}
		else if(status == gl.FRAMEBUFFER_INCOMPLETE_MULTISAMPLE) {	
			console.log("FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
		}
		else if(status == gl.RENDERBUFFER_SAMPLES) {	
			console.log("RENDERBUFFER_SAMPLES");
		}
		
		return false;
	}

	getW()
	{
		return this._w;
	}

	getH()
	{
		return this._h;
	}
	
	deleteFBO()
	{
		gl.deleteRenderbuffer(this._depthBufID);
  		gl.deleteTexture(this._texID);
		gl.deleteFramebuffers(this._fboID);
	}
}