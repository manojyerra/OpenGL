varying vec2 uv_varying;

uniform sampler2D textureID;

uniform int w;
uniform int h;
//uniform int filterID = 0;


float rand(float n)
{
	return fract(sin(n) * 43758.5453123);
}

void main (void)  
{
	gl_FragColor = texture2D(textureID, uv_varying);


	//GrayScale
	
	//vec4 texColor = texture2D(textureID, uv_varying);
	//float avg = ( texColor.x + texColor.y + texColor.z ) / 3.0 ; 
	//float avg = 0.2126*texColor.x + 0.7152*texColor.y + 0.0722*texColor.z;
	//gl_FragColor = vec4( avg, avg, avg, texColor.w);
	

	//Colorize...

	//vec4 texColor = texture2D(textureID, uv_varying);
	
	//float grayVal = 0.2126*texColor.x + 0.7152*texColor.y + 0.0722*texColor.z;
	
	//float colorize_x = 75.0 / 255.0;
	//float colorize_y = 0.0;
	//float colorize_z = 75.0 / 255.0;
	
	//texColor.x = (grayVal+colorize_x > 1.0) ? 1.0 : (grayVal+colorize_x);
	//texColor.y = (grayVal+colorize_y > 1.0) ? 1.0 : (grayVal+colorize_y);
	//texColor.z = (grayVal+colorize_z > 1.0) ? 1.0 : (grayVal+colorize_z);
	
	//gl_FragColor = texColor;
	
	
	//Invert RGB
	//vec4 texColor = texture2D(textureID, uv_varying);
	//gl_FragColor = vec4( 1.0 - texColor.x, 1.0 - texColor.y, 1.0 - texColor.z, texColor.w);

	//Invert RG
	//vec4 texColor = texture2D(textureID, uv_varying);
	//gl_FragColor = vec4( 1.0-texColor.x, 1.0-texColor.y, texColor.z, texColor.w);
	
	//Invert GB
	//vec4 texColor = texture2D(textureID, uv_varying);
	//gl_FragColor = vec4( texColor.x, 1.0-texColor.y, 1.0-texColor.z, texColor.w);

	//Invert BR
	//vec4 texColor = texture2D(textureID, uv_varying);
	//gl_FragColor = vec4( 1.0-texColor.x, texColor.y, 1.0-texColor.z, texColor.w);
	
	//Invert R
	//vec4 texColor = texture2D(textureID, uv_varying);
	//gl_FragColor = vec4( 1.0 - texColor.x, texColor.y, texColor.z, texColor.w);
	
	//Invert G
	//vec4 texColor = texture2D(textureID, uv_varying);
	//gl_FragColor = vec4( texColor.x, 1.0 - texColor.y, texColor.z, texColor.w);
	
	//Invert B
	//vec4 texColor = texture2D(textureID, uv_varying);
	//gl_FragColor = vec4( texColor.x, texColor.y, 1.0 - texColor.z, texColor.w);
	
	
	//Blur
	
	// vec4 texColor = texture2D(textureID, uv_varying);
		
	// vec2 size = vec2(w, h);
	// vec2 pos = vec2(uv_varying * size);

	// int posX = int(pos.x);
	// int posY = int(pos.y);
	
	// int blurLevel = 2;
	// float count = 0.0;

	// vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);
	
	// for(int n=-blurLevel; n<=blurLevel; n++)
	// {
		// for(int m=-blurLevel; m<=blurLevel; m++)
		// {
			// int x = posX + m;
			// int y = posY + n;

			// if(x <= w && y < h && x >= 0 && y >= 0)
			// {
				// vec2 uv = vec2( float(x) / float(w), float(y) / float(h));
				
				// finalColor += texture2D(textureID, uv);
				// count++;
			// }
		// }
	// }
	
	// gl_FragColor = vec4( finalColor.r / count, finalColor.g/ count, finalColor.b/ count, texColor.a);
	
	
	//Explosion
	
	// vec4 c = texture2D(textureID, uv_varying);
	
	// vec2 size = vec2(w, h);
	// vec2 pos = vec2(uv_varying * size);
		
	// vec4 temp = c * 255;
	// float sum = temp.x + temp.y + temp.z;
	// sum = sum + sin(pos.x) * 43758.5453123;
	// int div = int(sum/20.0);
	// int randNum = int(sum) - (div * 20);
	// int sign = -1;
	
	// if(randNum < 10)
		// sign = 1;
	
	// float valX = pos.x + randNum * sign;
	// float valY = pos.y + randNum * sign * -1;

	// if(valX >= 0 && valY >= 0 && valX < w && valY < h)
	// {
		// vec2 uv = vec2( float(valX) / float(w), float(valY) / float(h));
		// c = texture2D(textureID, uv);
	// }
	
	// gl_FragColor = c;
}
