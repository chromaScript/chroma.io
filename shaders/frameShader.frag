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
        
        float final_alpha = clamp(src.a + dst.a * (1.0 - src.a), 0, opacity);

        //fragColor = vec4(
        //   (src.rgb * src.a + dst.rgb * final_alpha * (1.0 - src.a)) / final_alpha,
        //   final_alpha);

        // For now, using 0 as the destination color. Seems to produce the correct results,
        // not sure how it will continue to hold up with more features.
        fragColor = vec4(
           ((src.rgb * src.a + 0 * final_alpha * (1.0 - src.a))) / final_alpha,
           final_alpha);
    }
    // Process used for compositing the stroke with the default framebuffer
    else{
        // fragColor = texture(dstTex, TexCoords);
        // fragColor.a = fragColor.a * strokeOpacity;

          fragColor = texture(dstTex, TexCoords) * strokeOpacity;
        // fragColor.a = fragColor.a * strokeOpacity;
    }
   
}