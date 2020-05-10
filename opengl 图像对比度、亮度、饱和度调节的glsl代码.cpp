#ifdef GL_ES
precision highp float;
#endif
uniform sampler2D from, to;
uniform float progress;
uniform vec2 resolution;
uniform float brightness;
uniform float contrast;
uniform float saturation;
vec3 hsv2rgb(vec3 c) {
    const vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 rgb2hsv(vec3 c) {
    const vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + 0.001)), d / (q.x + 0.001), q.x);
}

void getTable()
{
    float c=(100.0 + contrast)/100.0;
    float brightnesstmp = brightness + 128.0;

    float cTable[256]; //<nipper>
    for (int i=0; i<256; i++)   {
   // cTable[i] = max(0,min(255,((i-128.0)*c + brightnesstmp + 0.5f)));
    cTable[i] = max(0.0, min(255.0,((float(i-128))*c + brightnesstmp + 0.5)));
  } 
}


void main() {
  vec2 p = gl_FragCoord.xy / resolution.xy;
  vec3 a = rgb2hsv(texture2D(from, p).rgb);
  vec3 m = a+vec3(contrast,saturation ,brightness);
  gl_FragColor = vec4(hsv2rgb(m), 1.0);
}
