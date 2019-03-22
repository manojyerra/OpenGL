varying vec2 uvVary;

uniform sampler2D textureID;

void main (void)
{     
	gl_FragColor = texture2D(textureID, uvVary);
}
