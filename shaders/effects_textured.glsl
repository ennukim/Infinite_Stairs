
uniform sampler2D diffuse;
varying vec2 texCoordVar;

void main() {
    vec4 color = texture2D(diffuse, texCoordVar);

    // Single Color
    gl_FragColor = vec4(color.r, 0, 0, color.a);

    // Inverted Color
    gl_FragColor = vec4(1 - color.r, 1 - color.g, 1 - color.b, color.a);

    // Greyscale
    float c = (color.r + color.g + color.b) / 3.0;
    gl_FragColor = vec4(c, c, c, color.a);

    // Luminosity - The more Realistic GreyScale
    vec3 luminance = vec3(dot(vec3(0.2126,0.7152,0.0722), color.rgb));
    gl_FragColor = vec4(luminance, color.a);

    // Saturation Variance
    vec3 luminance = vec3(dot(vec3(0.2126,0.7152,0.0722), color.rgb));
    vec3 m = mix(luminance, color.rgb, 1.0); // Switch the third Variable to configure configuration
    gl_FragColor = vec4(m, color.a);
}
