# AE_tl_math
Copyright © Thomas Laforge. 2018
Free  use  of  tl_math  is
permitted under the guidelines and in accordance with the most current
version of the https://opensource.org/licenses/gpl-3.0.html.

The plugin uses Exprtk by Arash Partow
https://github.com/ArashPartow/exprtk


An After Effect plugin project.
The user can write math expression for each pixels and by channels (RGBA). 
whatever your Color Space, the expected values are bewteen 0 and 1.


Verion note: 
 Alpha 02 (v 1.1)
 
 -the plugin works with After Effect CC and above.
 
 -the plugin works in 8 and 16 bits (32bits coming soon)
 
 -Possibilities to write an expression per channels
 
 -the plugin expressions can access to 4 sliders, two points and two colors pipets to comunicate with other AE elements.
 
 -presets can be saved as json files.
 
 -The expression language is based on Exprtk grammar  https://github.com/ArashPartow/exprtk
 
 
 
 HOW TO INSTALL
extract the zip file in the folder : 
On mac : /Library/Application Support/Adobe/Common/Plug-ins/7.0/MediaCore/

on Windows : C:\Program Files\Adobe\Common\Plug-ins\7.0\MediaCore\

**1-THE UI**



2- BASIC EXEMPLE

3- RULES

**4-THE VARIABLES**

4.1 Pixel Access

NAME| TYPE | DESCRIPTION|
---------|--------|--------|
`xL`       | float  | Coordonate of the current pixel on the x axis|
`yL`       | float  | Coordonate of the current pixel on the y axis|
`in_red`  | float  | value of red channel from the current pixel|
`in_green`| float  | value of green channel from the current pixel|
`in_blue` | float  |  value of blue channel from the current pixel|
`in_alpha`| float  |  value of alpha channel from the current pixel|
`in_luma` | float  | value of luma  from the current pixel|
`extL_red`| float  | value of red channel from the input layer with time and pixel offset|
`extL_green`| float  | value of green channel from the input layer with time and pixel offset|
`extL_blue`| float  | value of blue channel from the input layer with time and pixel offset|
`extL_alpha`| float  | value of alphachannel from the input layer with time and pixel offset|

4.2 Matrix 3*3of pixels around

You can access to the pixels around with the Vec3_channel[mat] function where mat is an integer between 0 and 8.
In the layout, the number 4 represents the current pixel, the other number represents the index of each pixel arounds  
   
 0 | 1 | 2 |
---|---|---|
 3 | **4 CURRENT PIXEL** | 5 |
 6 | 7 | 8 |

MATRIX NAME| TYPE INPUT | TYPE OUPUT| DESCRIPTION|
---------|--------|--------|--------|
`vec3_red[mat]`  | int   | float  | Access to the red channel from pixel around. mat is an integer between 0 and 8
`vec3_green[mat]`  | int   | float  | Access to the green channel from pixel around. mat is an integer between 0 and 8
`vec3_blue[mat]`  | int   | float  | Access to the blue channel from pixel around. mat is an integer between 0 and 8
`vec3_alpha[mat]`  | int   | float  | Access to the alpha channel from pixel around. mat is an integer between 0 and 8

4.3 Effect UI parameters

The plugin has some classical parameters to communicate with the other After Effects parameters, other effects and the classical expressions. The next table describe the access to the value with the math Parser.

NAME| TYPE | DESCRIPTION|
---------|--------|--------|
`var1`| float | Value of the slider variable 1 |
`var2`| float | Value of the slider variable 2 |
`var3`| float | Value of the slider variable 3 |
`var4`| float | Value of the slider variable 4|
`pt1_x`| float | X value of Point one |
`pt1_y`| float | Y value of Point one |
`pt2_x`| float | X value of Point Two |
`pt2_y`| float  | Y value of Point Two  |
`cl1_red`| float | red value of parameter Color One |
`cl1_green`| float  | green value of parameter Color One |
`cl1_blue`| float  | blue value of parameter Color One |
`cl2_red`| float  | red value of parameter Color Two |
`cl2_green`| float | green value of parameter Color Two |
`cl2_blue`| float | blue value of parameter Color Two |

4.4 Layer Properties

Access to the transform properties of the current layer from the math expressions

NAME| TYPE | DESCRIPTION|
---------|--------|--------|
`layerWidth` | float | Width of the layer |
`layerHeight` | float | height of the layer |
`layerTime_sec` | float | Current time of the layer in seconds |
`layerTime_frame` | float | Current time of the layer in frames |
`layerDuration` | float | Duration of the layer |
`layerPosition_x` | float | Position of the layer on the x axis | 
`layerPosition_y` | float | Position of the layer on the y axis |
`layerPosition_z` | float | Position of the layer on the z axis |
`layerScale_z` | float | Scale of the layer  on the x axis | 
`layerScale_y` | float | Scale of the layer  on the y axis | 
`layerScale_z` | float | Scale of the layer  on the z axis | 

4.5 Composition properties

Access to the compositions properties

NAME| TYPE | DESCRIPTION|
---------|--------|--------|
`compWidth`| float |  Width of the composition |
`compHeight` | float | Height of the composition |
`compFps` | float |   Return the parameter number of frame per seconds of the composition |


6- FUNCTIONS


