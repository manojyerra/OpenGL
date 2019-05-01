
varying lowp vec4 lightColor;
varying lowp vec2 uv_varying;

uniform sampler2D baseTexture;

void main (void)  
{   
   gl_FragColor = lightColor * texture2D(baseTexture, uv_varying);
}
