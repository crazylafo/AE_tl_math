
#include "tl_math.h"

typedef struct {
	A_u_long	index;
	A_char		str[256];
} TableString;





TableString		g_strs[StrID_NUMTYPES] = {
	StrID_NONE,						    "",
	StrID_Name,						    "tl Maths",
	StrID_Description,				    "Execute GLSL shaders or Math operations in the pixel world.",
	STR_ID_BUTTON_SETUP_Param_Name,     "SETUP",
    StrID_TOPIC_SLIDER_Param_Name,      "Variables",
    StrID_INPUTONE_Param_Name,          "variable 1",
    StrID_INPUTTWO_Param_Name,          "variable 2",
    StrID_INPUTTHREE_Param_Name,        "variable 3",
    StrID_INPUTFOUR_Param_Name,         "variable 4",
    StrID_TOPIC_POINTS_Param_Name,      "Points",
    strID_INPUTPOINTONE_Param_Name,     "Point One",
    strID_INPUTPOINTTWO_Param_Name,     "Point Two",
    StrID_TOPIC_COLORS_Param_Name,      "Colors",
    strID_INPUTCOLORONE_Param_Name,     "Color One",
    strID_INPUTCOLORTWO_Param_Name,     "Color Two",
    StrID_TOPIC_INPUTS_Param_Name,      "Inputs",
    StrID_LAYER_ONE_Param_Name,         "Layer input",
    StrID_TOFF_ONE_Param_Name,          "Frames Offset",
    StrID_POFF_ONE_Param_Name,          "Shift Center to",
	StrID_CEP_GETARB_Param_Name,         "get arb",


};


char	*GetStringPtr(int strNum)
{
	return g_strs[strNum].str;
}



