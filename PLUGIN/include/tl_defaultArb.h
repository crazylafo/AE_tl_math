//
//  tl_defaultArb.h
//  tlMath
//
//

#ifndef tl_defaultArb_h
#define tl_defaultArb_h

static std::string defaultArb = R"=====(
{

    "composition": {


        "camera_position": "camera_pos",


        "camera_rotation": "camera_rotation",


        "camera_target": "camera_target",


        "camera_zoom": "camera_zoom",


        "compResolution": "compResolution",


        "frame_rate": "fps",


        "layerPosition": "layerPosition",


        "layerScale": "layerScale",


        "resolution": "resolution",


        "time_frame": "timef",


        "time_sec": "time"

    },

    "effectInfo": {


        "description": "simple skeleton_effect\\npeterre",


        "effectName": "tlMath",


        "minimalPluginVersion": 115,


        "pluginVersion": 115,


        "presetName": "default_preset",


        "tags": [



                 "default",



                 "glsl",



                 "expr",



                 "skeleton"


                 ]

    },

    "effectMode": {


        "evalModeB": false,


        "expr_modeB": false,


        "gl33_modeB": true

    },

    "flags": {


        "needsLumaB": false,


        "pixelsCallExternalInputB": [



                                     true,



                                     false,



                                     false,



                                     false


                                     ],


        "presetHasWideInputB": true,


        "usesCameraB": false

    },

    "gl_expression": {


        "gl33_frag_error": "compiled successfully",


        "gl33_frag_sh": "#version 330\\n//EXEMPE TO SHOW HOW THE PLUGIN WORKS\\n//A SHADER MUST START ITH A DEFINITION OF THE GLSL VERSION USED\\n// FOR NOW THE PLUGIN WORKS ONLY ITH 330 (for opengl 3.3)\\n\\n\\n\\n// let's call variables from After Effect\\n// THE PROCES IS uniform [TYPE] [variableName];\\n//uniform--> the function to call an external element in glsl\\n//type--> sampler2D for a texture from AE\\n// ------ float for a single dimention (time, slider or rotation param)\\n//-------- vec2  for a two dimension item (resolution)\\n//--------vec3 for a three dimension item (points/ color)\\n//--------bool  for checkbox\\n// Points are alayws defined as vec3 with this plugin. So if you have \\n// a shader with a 2D point, you can convert it latter.\\n// Slider are alays imported as float. if you want an integer you can convert\\n//on the fly: int(sliderName) \\n//variable Name: you define it in param setting.\\n//for this exemple we keep the default name\\nuniform sampler2D texture0; //get the layer source\\nuniform sampler2D texture1; // get an other layer\\nuniform float slider_1;// get  a slider from the UI\\nuniform vec3 point_1; // get  a pioint from the UI\\nuniform bool cbox_1; // get  a checkbox from the UI\\nuniform vec3 color_1; //get a color input\\nuniform vec2 resolution; //get  the resolution\\nin vec2 out_uvs; // get the uv (1,1) size of the layer\\nout vec4 fragColorOut; // the output varaible\\n// in glsl you can use gl_FragColorOut for the output but\\n// on some version of macos we have to define an out vec4 variable.\\n//if you develop on pc and want to share shaders, don't forget mac users...\\n// you can give it the name you want \\n\\n\\nvoid main(void)\\n{\\n    vec4 textureInput = texture(texture0, out_uvs); // call the orginal  layer with the orginal position\\n    vec4 premix =mix (textureInput,vec4(color_1, 1.0), slider_1/100); //let's make a simple mix with color 1 influence by a slider\\n    //note we wrote vec4(color_1, 1.0). becaufe we have to mix a four color channel so we add a full alpha\\n    if (cbox_1){ // if the checkbox is activated\\n        // we import the layer1 ith an offset ith point1.\\n        // an offset has to be nromalized: (1,1) =(the layer x size normalized, the layer y scale normalized)\\n        // so an offset to stay in the picture has to be defined wetween 0 and 1\\n        // lets start with resolution/2 get the center. (for ex in fullhd wi get center (960/540)\\n        //if we substract the point coord we get the off value\\n        // 0 if the point is in the center. to get a normalized value to the layer we divide by the resoluton \\n        vec2 off;\\n        off.x =out_uvs.x +( resolution.x/2 -point_1.x)/resolution.x;\\n        off.y =out_uvs.y +( resolution.y/2 -point_1.y)/resolution.y;\\n        vec4 textureLayer = texture (texture1,off );\\n        fragColorOut = mix (textureInput,textureLayer, slider_1/100); //let's do some mix with a slider\\n    }else{\\n         fragColorOut =premix; // if checkbox not selected return premix\\n\\n    }\\n             //if you want to play with pixel coordonate activate the alpha ramp\\n         //alpha ramp \\n         //fragColorOut.a = gl_FragCoord.x/resolution.x;\\n}",


        "gl33_vert_error": "compiled successfully",


        "gl33_vert_sh": "#version 330 \\n in vec4 Position;\\nin vec2 UVs;\\nout vec4 out_pos;\\nout vec2 out_uvs;\\nuniform mat4 ModelviewProjection;\\nvoid main(void)\\n{\\nout_pos = ModelviewProjection * Position; \\n gl_Position = out_pos; \\nout_uvs = UVs;\\n}"

    },

    "gui_settings": {


        "cboxGrp": {



            "grpName": "check box Group",



            "grpVisibleB": true,



            "params": [




            {





                "defaultVal": [






                               0





                               ],





                "name": "cbox_1",





                "visibleB": true




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "cbox_2",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "cbox_3",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "cbox_4",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "cbox_5",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "cbox_6",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "cbox_7",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "cbox_8",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "cbox_9",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "cbox_10",





                "visibleB": false




            }



                       ]


        },


        "colorGrp": {



            "grpName": "3d color Group",



            "grpVisibleB": true,



            "params": [




            {





                "defaultVal": [






                               1,






                               1,






                               1





                               ],





                "name": "color_1",





                "visibleB": true




            },




            {





                "defaultVal": [






                               1,






                               1,






                               1





                               ],





                "name": "color_2",





                "visibleB": false




            },




            {





                "defaultVal": [






                               1,






                               1,






                               1





                               ],





                "name": "color_3",





                "visibleB": false




            },




            {





                "defaultVal": [






                               1,






                               1,






                               1





                               ],





                "name": "color_4",





                "visibleB": false




            },




            {





                "defaultVal": [






                               1,






                               1,






                               1





                               ],





                "name": "color_5",





                "visibleB": false




            },




            {





                "defaultVal": [






                               1,






                               1,






                               1





                               ],





                "name": "color_6",





                "visibleB": false




            },




            {





                "defaultVal": [






                               1,






                               1,






                               1





                               ],





                "name": "color_7",





                "visibleB": false




            },




            {





                "defaultVal": [






                               1,






                               1,






                               1





                               ],





                "name": "color_8",





                "visibleB": false




            },




            {





                "defaultVal": [






                               1,






                               1,






                               1





                               ],





                "name": "color_9",





                "visibleB": false




            },




            {





                "defaultVal": [






                               1,






                               1,






                               1





                               ],





                "name": "color_10",





                "visibleB": false




            }



                       ]


        },


        "layerGrp": {



            "current_layer": {




                "name": "texture0"



            },



            "extLayer_1": {




                "name": "texture1",




                "visibleB": true



            },



            "extLayer_2": {




                "name": "texture2",




                "visibleB": false



            },



            "extLayer_3": {




                "name": "texture3",




                "visibleB": false



            },



            "extLayer_4": {




                "name": "texture4",




                "visibleB": false



            },



            "grpName": "Layer Group",



            "grpVisibleB": true


        },


        "pointGrp": {



            "grpName": "3d Point Group",



            "grpVisibleB": true,



            "params": [




            {





                "defaultVal": [






                               50,






                               50,






                               50





                               ],





                "name": "point_1",





                "visibleB": true




            },




            {





                "defaultVal": [






                               50,






                               50,






                               50





                               ],





                "name": "point_2",





                "visibleB": false




            },




            {





                "defaultVal": [






                               50,






                               50,






                               50





                               ],





                "name": "point_3",





                "visibleB": false




            },




            {





                "defaultVal": [






                               50,






                               50,






                               50





                               ],





                "name": "point_4",





                "visibleB": false




            },




            {





                "defaultVal": [






                               50,






                               50,






                               50





                               ],





                "name": "point_5",





                "visibleB": false




            },




            {





                "defaultVal": [






                               50,






                               50,






                               50





                               ],





                "name": "point_6",





                "visibleB": false




            },




            {





                "defaultVal": [






                               50,






                               50,






                               50





                               ],





                "name": "point_7",





                "visibleB": false




            },




            {





                "defaultVal": [






                               50,






                               50,






                               50





                               ],





                "name": "point_8",





                "visibleB": false




            },




            {





                "defaultVal": [






                               50,






                               50,






                               50





                               ],





                "name": "point_9",





                "visibleB": false




            },




            {





                "defaultVal": [






                               50,






                               50,






                               50





                               ],





                "name": "point_10",





                "visibleB": false




            }



                       ]


        },


        "rotationGrp": {



            "grpName": "rotation Group",



            "grpVisibleB": false,



            "params": [




            {





                "defaultVal": [






                               0





                               ],





                "name": "rotation_1",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "rotation_2",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "rotation_3",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "rotation_4",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "rotation_5",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "rotation_6",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "rotation_7",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "rotation_8",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "rotation_9",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "rotation_10",





                "visibleB": false




            }



                       ]


        },


        "sliderGrp": {



            "grpName": "Slider Group",



            "grpVisibleB": true,



            "params": [




            {





                "defaultVal": [






                               0





                               ],





                "name": "slider_1",





                "visibleB": true




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "slider_2",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "slider_3",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "slider_4",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "slider_5",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "slider_6",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "slider_7",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "slider_8",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "slider_9",





                "visibleB": false




            },




            {





                "defaultVal": [






                               0





                               ],





                "name": "slider_10",





                "visibleB": false




            }



                       ]


        }

    },

    "math_expression": {


        "alphaExpr": "return inChannel;",


        "alpha_error": "compiled successfully",


        "blueExpr": "return inChannel;",


        "blue_error": "compiled successfully",


        "exprRGBModeB": true,


        "expr_current_channel": "inChannel",


        "expr_pix": "pix",


        "greenExpr": "return inChannel;",


        "green_error": "compiled successfully",


        "redExpr": "return inChannel;",


        "red_error": "compiled successfully",


        "rgbExpr": "return texture0;",


        "rgb_error": "compiled successfully"

    }
}
)=====";


#endif /* tl_defaultArb_h */
