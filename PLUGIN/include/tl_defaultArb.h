//
//  tl_defaultArb.h
//  tlMath
//
//

#ifndef tl_defaultArb_h
#define tl_defaultArb_h

static std::string defaultArb = R"=====(
{
    "effectInfo":{
        "effectName":"tlMath",
        "pluginVersion":115,
        "minimalPluginVersion":115,
        "tags":["default", "glsl", "expr","skeleton"],
        "presetName":"default_preset",
        "description":"simple skeleton_effect"
    },
    "effectMode":{
        "gl33_modeB":false,
        "expr_modeB":true,
        "evalModeB":false
    },
    "gl_expression":{
       "gl33_frag_sh":"#version 330 // glsls version for opengl 3.3\\nuniform sampler2D texture0; //call the layer source\\nuniform float slider_1;// call somes variables from the ui\\nuniform float multiplier16bit; //proper to AE 16 bits depth.\\nin vec4 out_pos;\\nin vec2 out_uvs;\\nout vec4 fragColorOut;\\n// to use instead of texture(sampler2D, vec2 uv) because of swizzle RGBA/ ARGBs\\nvec4 loadTextureFromAE (sampler2D tex2d)\\n{\\n    vec2 uv_AE = out_uvs;\\n    uv_AE.y = 1.- out_uvs.y;\\n    vec4 textureIn = texture( tex2d, uv_AE);\\n    textureIn =  textureIn * multiplier16bit;\\n    textureIn= vec4( textureIn.g,  textureIn.b,  textureIn.a,  textureIn.r);\\n    textureIn= vec4( textureIn.a *  textureIn.r,  textureIn.a *  textureIn.g,  textureIn.a * textureIn.b,  textureIn.a);\\n    return  textureIn ;\\n}\\nvec4 loadTextureOffset (sampler2D tex2d,  vec2 off){\\n    vec2 uv_AE = out_uvs;\\n    uv_AE.x = out_uvs.x +off.x;\\n    uv_AE.y = 1.- (out_uvs.y+off.y);\\n    vec4 textureIn = texture( tex2d, uv_AE);\\n    textureIn =  textureIn * multiplier16bit;\\n    textureIn= vec4( textureIn.g,  textureIn.b,  textureIn.a,  textureIn.r);\\n    textureIn= vec4( textureIn.a *  textureIn.r,  textureIn.a *  textureIn.g,  textureIn.a * textureIn.b,  textureIn.a);\\n    return  textureIn ;\\n\\n}\\n\\nvoid main(void)\\n{\\n    fragColorOut= loadTextureFromAE(texture0);\\n    fragColorOut.r *=slider_1/100; \\n}",
        "gl33_vert_sh":"#version 330 \\n in vec4 Position;\\nin vec2 UVs;\\nout vec4 out_pos;\\nout vec2 out_uvs;\\nuniform mat4 ModelviewProjection;\\nvoid main(void)\\n{\\nout_pos = ModelviewProjection * Position; \\n gl_Position = out_pos; \\nout_uvs = UVs;\\n}",
        "gl33_frag_error" :  "compiled successfully",
        "gl33_vert_error" :  "compiled successfully"
    },
    "math_expression":{
        "redExpr":"return inChannel;",
        "greenExpr":"return inChannel;",
        "blueExpr":"return inChannel;",
        "rgbExpr":"return texture0;",
        "alphaExpr":"return inChannel;",
        "red_error":  "compiled successfully",
        "green_error" :  "compiled successfully",
        "blue_error" :  "compiled successfully",
        "alpha_error" :  "compiled successfully",
        "rgb_error" :  "compiled successfully",
        "exprRGBModeB":true,
        "expr_current_channel":"inChannel",
        "expr_pix":"pix"
        },
    "flags":{
        "pixelsCallExternalInputB":[false,false,false,false],
        "needsLumaB":false,
        "presetHasWideInputB":false,
        "usesCameraB":false
    },
    "composition":{
        "resolution":"resolution",
        "layerPosition": "layerPosition",
        "layerScale":"layerScale",
        "compResolution": "compResolution",
        "time_sec":"time",
        "time_frame":"timef",
        "frame_rate":"fps",
        "camera_position":"camera_pos",
        "camera_target":"camera_target",
        "camera_zoom":"camera_zoom",
        "camera_rotation": "camera_rotation"
    },
    "gui_settings":{
        "sliderGrp":{
            "grpVisibleB":true,
            "grpName":"Slider Group",
            "params":[
                      {
                          "visibleB":true,
                          "name":"slider_1",
                          "defaultVal":[0]
                      },
                      {
                          "visibleB":true,
                          "name":"slider_2",
                          "defaultVal":[0]
                      },
                      {
                          "visibleB":true,
                          "name":"slider_3",
                          "defaultVal":[0]
                      },
                      {
                          "visibleB":true,
                          "name":"slider_4",
                          "defaultVal":[0]
                      },
                      {
                          "visibleB":true,
                          "name":"slider_5",
                          "defaultVal":[0]
                      },
                      {
                          "visibleB":true,
                          "name":"slider_6",
                          "defaultVal":[0]
                      },
                      {
                          "visibleB":true,
                          "name":"slider_7",
                          "defaultVal":[0]
                      },
                      {
                          "visibleB":true,
                          "name":"slider_8",
                          "defaultVal":[0]
                      },
                      {
                          "visibleB":true,
                          "name":"slider_9",
                          "defaultVal":[0]
                      },
                      {
                          "visibleB":true,
                          "name":"slider_10",
                          "defaultVal":[0]
                      }
                      ]
        },
        "pointGrp":{
            "grpVisibleB":true,
            "grpName":"3d Point Group",
            "params": [
                       {
                           "visibleB":true,
                           "name":"point_1",
                           "defaultVal":[50,50,50]
                       },
                       {
                           "visibleB":true,
                           "name":"point_2",
                           "defaultVal":[50,50,50]
                       },
                       {
                           "visibleB":true,
                           "name":"point_3",
                           "defaultVal":[50,50,50]
                       },
                       {
                           "visibleB":true,
                           "name":"point_4",
                           "defaultVal":[50,50,50]
                       },
                       {
                           "visibleB":true,
                           "name":"point_5",
                           "defaultVal":[50,50,50]
                       },
                       {
                           "visibleB":true,
                           "name":"point_6",
                           "defaultVal":[50,50,50]
                       },
                       {
                           "visibleB":true,
                           "name":"point_7",
                           "defaultVal":[50,50,50]
                       },
                       {
                           "visibleB":true,
                           "name":"point_8",
                           "defaultVal":[50,50,50]
                       },
                       {
                           "visibleB":true,
                           "name":"point_9",
                           "defaultVal":[50,50,50]
                       },
                       {
                           "visibleB":true,
                           "name":"point_10",
                           "defaultVal":[50,50,50]
                       }
                       ]
        },
        "colorGrp":{
            "grpVisibleB":true,
            "grpName":"3d color Group",
            "params": [
                       {
                           "visibleB":true,
                           "name":"color_1",
                           "defaultVal":[1.0,1.0,1.0]
                       },
                       {
                           "visibleB":true,
                           "name":"color_2",
                           "defaultVal":[1.0,1.0,1.0]
                       },
                       {
                           "visibleB":true,
                           "name":"color_3",
                           "defaultVal":[1.0,1.0,1.0]
                       },
                       {
                           "visibleB":true,
                           "name":"color_4",
                           "defaultVal":[1.0,1.0,1.0]
                       },
                       {
                           "visibleB":true,
                           "name":"color_5",
                           "defaultVal":[1.0,1.0,1.0]
                       },
                       {
                           "visibleB":true,
                           "name":"color_6",
                           "defaultVal":[1.0,1.0,1.0]
                       },
                       {
                           "visibleB":true,
                           "name":"color_7",
                           "defaultVal":[1.0,1.0,1.0]
                       },
                       {
                           "visibleB":true,
                           "name":"color_8",
                           "defaultVal":[1.0,1.0,1.0]
                       },
                       {
                           "visibleB":true,
                           "name":"color_9",
                           "defaultVal":[1.0,1.0,1.0]
                       },
                       {
                           "visibleB":true,
                           "name":"color_10",
                           "defaultVal":[1.0,1.0,1.0]
                       }
                       ]
        },
        "cboxGrp":{
            "grpVisibleB":true,
            "grpName":"check box Group",
            "params": [
                       {
                           "visibleB":true,
                           "name":"cbox_1",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"cbox_2",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"cbox_3",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"cbox_4",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"cbox_5",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"cbox_6",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"cbox_7",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"cbox_8",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"cbox_9",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"cbox_10",
                           "defaultVal":[0]
                       }
                       ]
        },
        "rotationGrp":{
            "grpVisibleB":true,
            "grpName":"rotation Group",
            "params": [
                       {
                           "visibleB":true,
                           "name":"rotation_1",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"rotation_2",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"rotation_3",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"rotation_4",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"rotation_5",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"rotation_6",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"rotation_7",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"rotation_8",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"rotation_9",
                           "defaultVal":[0]
                       },
                       {
                           "visibleB":true,
                           "name":"rotation_10",
                           "defaultVal":[0]
                       }
                       ]
        },
        "layerGrp":{
            "grpVisibleB":true,
            "grpName":"Layer Group",
            "current_layer":{
                "name":"texture0"
            },
            "extLayer_1":{
                "visibleB":true,
                "name":"texture1"
            },
            "extLayer_2":{  
                "visibleB":true,
                "name":"texture2"

            },
            "extLayer_3":{
                "visibleB":true,
                "name":"texture3"
            },
            "extLayer_4":{
                "visibleB":true,
                "name":"texture4"
            }
        }
    }
}
)=====";


#endif /* tl_defaultArb_h */
