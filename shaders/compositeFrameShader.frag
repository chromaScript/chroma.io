#version 460 core
out vec4 fragColor;

in vec2 TexCoords;

uniform bool preMultiply;

uniform sampler2D dstTex;
uniform sampler2D srcTex;

uniform float strokeOpacity;
uniform float opacity;

void main()
{
    // Process used for compositing each brush tip
    if (preMultiply)
    {
        vec4 dst = texture(dstTex, TexCoords);
        vec4 src = texture(srcTex, TexCoords);
        vec3 mixCol = mix(dst.rgb, src.rgb, 1);

        float final_alpha = src.a + dst.a * (1.0 - src.a);
       
        fragColor = vec4(
           (src.rgb * src.a + dst.rgb * dst.a * (1.0 - src.a)) / final_alpha,
           final_alpha);

		//fragColor.r = 1.0;
		//fragColor.g = 0.5;
		//fragColor.b = 0.2;

        

        if (fragColor.a > opacity) { fragColor.a = opacity; }
        if (fragColor.a < dst.a) { fragColor.a = dst.a; }
    }
    // Process used for compositing the stroke with the default framebuffer
    else{
       // fragColor = texture(dstTex, TexCoords); // Old non-premultiplied behavior
       // fragColor.a *= strokeOpacity;
       fragColor = texture(dstTex, TexCoords);
    }
   
}