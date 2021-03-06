//
//  tl_defaultShaders.h
//  tlMath
//
//  
//

#ifndef tl_defaultShaders_h
#define tl_defaultShaders_h


//GLSL SCRIPTS
class tlmath_shaders {
public:
	std::string compile_success = "compiled successfully";
	std::string endLineStr = " ;\n";
	std::string importFloatGlStr = "uniform float ";
    std::string importVec2GlStr = "uniform vec3 ";
	std::string importVec3GlStr = "uniform vec3 ";
	std::string importBoolGlStr = "uniform bool ";
	std::string import2dTextGlStr = "uniform sampler2D ";
	std::string  endFunctionStr = "\n } \n";
	std::string redFunctionStr = "float redExpr(vec2 fragCoord, float colorCh){ \n";
	std::string greenFunctionStr = "float greenExpr(vec2 fragCoord, float colorCh){ \n";
	std::string blueFunctionStr = "float blueExpr(vec2 fragCoord, float colorCh){ \n";
	std::string alphaFunctionStr = "float alphaExpr(vec2 fragCoord, float colorCh){ \n";
	std::string rgbFunctionStr = "vec3 rgbExpr(vec2 fragCoord, vec3 inputRgb){ \n";


	 std::string gl33GeneriqueShInput = R"=====(#version 330 // glsls version for opengl 3.3
out vec4 fragColorOut;
in vec2 out_uvs;
vec2 textUvs = vec2(out_uvs.x, 1-out_uvs.y);
uniform vec2 resolution;
)=====";







	 std::string gl33InputMainGrp = R"=====(
void main(void)
{
    vec4 text0= texture(inputLayer0, textUvs);
    fragColorOut.rgb = rgbExpr(gl_FragCoord.xy, text0.rgb);
    fragColorOut.a = alphaExpr(gl_FragCoord.xy, text0.a);

})=====";

	std::string gl33InputMainSplit = R"=====(
void main(void)
{
	vec4 text0= texture(inputLayer0, textUvs);
	fragColorOut.r =  redExpr(gl_FragCoord.xy, text0.r);
    fragColorOut.g =  greenExpr(gl_FragCoord.xy, text0.g);
    fragColorOut.b = blueExpr (gl_FragCoord.xy, text0.b);
	fragColorOut.a= alphaExpr(gl_FragCoord.xy, text0.a);

})=====";



	 std::string glvertstr = "#version 330 \n\
in vec4 Position;\n\
in vec2 UVs;\n\
out vec4 out_pos;\n\
out vec2 out_uvs;\n\
uniform mat4 ModelviewProjection;\n\
void main(void)\n\
{\n\
out_pos = ModelviewProjection * Position; \n\
gl_Position = out_pos; \n\
out_uvs = UVs;\n\
}";

	 std::string glfrag2str = "#version 330\n\
uniform sampler2D layerTex;\n\
uniform float multiplier16bit;\n\
uniform vec2 resolution;\n\
in vec4 out_pos; \n\
in vec2 out_uvs; \n\
out vec4 colorOut; \n\
void main(void)\n\
{\n\
    colorOut = texture(layerTex, vec2(out_uvs.x, 1-out_uvs.y));\n\
    colorOut = vec4(colorOut.a, colorOut.r, colorOut.g, colorOut.b);\n\
    colorOut = colorOut / multiplier16bit;\n\
}";

	  std::string glErrorMessageStr = R"=====(
// based on http://glslsandbox.com/e#53346.0
#version 330 // glsls version for opengl 3.3
uniform float multiplier16bit; //proper to AE 16 bits depth.
out vec4 fragColorOut;
vec2 uv;
uniform vec2 resolution;

const vec2 ch_size  = vec2(1.0, 2.0) * 0.6;              // character size (X,Y)
const vec2 ch_space = ch_size + vec2(1.0, 1.0);    // character distance Vector(X,Y)
const vec2 ch_start = vec2 (ch_space.x * -5., 1.); // start position
vec2 ch_pos   = vec2 (0.0, 0.0);             // character position(X,Y)


#define REPEAT_SIGN false // True/False; True=Multiple, False=Single

#define S ddigit(0x8866);
#define H ddigit(0x1199);
#define A ddigit(0x119F);
#define D ddigit(0x44E7);
#define E ddigit(0x107E);
#define R ddigit(0x911F);
#define _ ch_pos.x += ch_space.x;
#define O ddigit(0x00FF);
#define nl1 ch_pos = ch_start;  ch_pos.y -= 3.0;


float dseg(vec2 p0, vec2 p1)
{
    vec2 dir = normalize(p1 - p0);
    vec2 cp = (uv - ch_pos - p0) * mat2(dir.x, dir.y,-dir.y, dir.x);
    return distance(cp, clamp(cp, vec2(0), vec2(distance(p0, p1), 0)));
}

bool bit(int n, int b)
{
    return mod(floor(float(n) / exp2(floor(float(b)))), 2.0) != 0.0;
}

float d = 1e6;

void ddigit(int n)
{
    float v = 1e6;
    vec2 cp = uv - ch_pos;
    if (n == 0)     v = min(v, dseg(vec2(-0.405, -1.000), vec2(-0.500, -1.000)));
    if (bit(n,  0)) v = min(v, dseg(vec2( 0.500,  0.063), vec2( 0.500,  0.937)));
    if (bit(n,  1)) v = min(v, dseg(vec2( 0.438,  1.000), vec2( 0.063,  1.000000000)));
    if (bit(n,  2)) v = min(v, dseg(vec2(-0.063,  1.000), vec2(-0.438,  1.000)));
    if (bit(n,  3)) v = min(v, dseg(vec2(-0.500,  0.937), vec2(-0.500,  0.062)));
    if (bit(n,  4)) v = min(v, dseg(vec2(-0.500, -0.063), vec2(-0.500, -0.938)));
    if (bit(n,  5)) v = min(v, dseg(vec2(-0.438, -1.000), vec2(-0.063, -1.000)));
    if (bit(n,  6)) v = min(v, dseg(vec2( 0.063, -1.000), vec2( 0.438, -1.000)));
    if (bit(n,  7)) v = min(v, dseg(vec2( 0.500, -0.938), vec2( 0.500, -0.063)));
    if (bit(n,  8)) v = min(v, dseg(vec2( 0.063,  0.000), vec2( 0.438, -0.000)));
    if (bit(n,  9)) v = min(v, dseg(vec2( 0.063,  0.063), vec2( 0.438,  0.938)));
    if (bit(n, 10)) v = min(v, dseg(vec2( 0.000,  0.063), vec2( 0.000,  0.937)));
    if (bit(n, 11)) v = min(v, dseg(vec2(-0.063,  0.063), vec2(-0.438,  0.938)));
    if (bit(n, 12)) v = min(v, dseg(vec2(-0.438,  0.000), vec2(-0.063, -0.000)));
    if (bit(n, 13)) v = min(v, dseg(vec2(-0.063, -0.063), vec2(-0.438, -0.938)));
    if (bit(n, 14)) v = min(v, dseg(vec2( 0.000, -0.938), vec2( 0.000, -0.063)));
    if (bit(n, 15)) v = min(v, dseg(vec2( 0.063, -0.063), vec2( 0.438, -0.938)));
    ch_pos.x += ch_space.x;
    d = min(d, v);
}
mat2 rotate(float a)
{
    float c = cos(a);
    float s = sin(a);
    return mat2(c, s, -s, c);
}
vec3 hsv2rgb_smooth( in vec3 c )
{
    vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb); // cubic smoothing
    return c.z * mix( vec3(1.0), rgb, c.y);
}
void main( void )
{
    vec2 aspect = resolution.xy / resolution.y;
    uv =  ( gl_FragCoord.xy / resolution.y ) - aspect / 2.0;
    float _d =  1.0-length(uv);
    uv *= 18.0 ;
    uv.y -= 3.;
    vec3 ch_color = hsv2rgb_smooth(vec3(1*-.04+uv.y*0.1,0.5,1.0));
    vec3 bg_color = vec3(_d*0.4, _d*0.2, _d*0.1);
    ch_pos = ch_start;
    nl1  S H A D E R _ E R R O R
    vec3 color = mix(ch_color, bg_color, 1.0- (0.08 / d*2.0));  // shading
    fragColorOut = vec4(1.-pow(length(color)-0.25, 4.)*vec3(.5,.5,.5), 1.0);
}
)=====";

};


#endif /* tl_defaultShaders_h*/
