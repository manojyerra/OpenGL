varying lowp vec2 uv_varying;

uniform sampler2D textureID;

void main (void)  
{
	gl_FragColor = texture2D(textureID, uv_varying);
}
