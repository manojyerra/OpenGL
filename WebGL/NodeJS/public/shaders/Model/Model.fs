
varying lowp vec4 lightColor;
varying lowp vec2 uv_varying;

uniform sampler2D textureID;

void main (void)  
{   
   gl_FragColor = lightColor * texture2D(textureID, uv_varying);
}
