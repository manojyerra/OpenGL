varying lowp vec4 fragColor;

void main (void)  
{     
   //gl_FragColor = fragColor;
   //gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
   
   gl_FragColor = vec4(fragColor.r/255.0, fragColor.g/255.0, fragColor.b/255.0, fragColor.a/255.0);
}
