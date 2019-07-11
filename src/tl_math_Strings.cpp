
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
    STR_ID_MATH_TOPIC_POINTS_Param_Name,   "slider group",
    STR_ID_MATH_SLIDER_ONE_Param_Name,      "slider 01",
    STR_ID_MATH_SLIDER_TWO_Param_Name,      "slider 02",
    STR_ID_MATH_SLIDER_THREE_Param_Name,    "slider 03",
    STR_ID_MATH_SLIDER_FOUR_Param_Name,     "slider 04",
    STR_ID_MATH_SLIDER_FIVE_Param_Name,     "slider 05",
    STR_ID_MATH_SLIDER_SIX_Param_Name,      "slider 06",
    STR_ID_MATH_SLIDER_SEVEN_Param_Name,    "slider 07",
    STR_ID_MATH_SLIDER_HEIGHT_Param_Name,   "slider 08",
    STR_ID_MATH_SLIDER_NINE_Param_Name,     "slider 09",
    STR_ID_MATH_SLIDER_TEN_Param_Name,      "slider 10",
    STR_ID_MATH_TOPIC_POINTS_Param_Name,    "point group",
    STR_ID_MATH_POINT_ONE_Param_Name,       "point 01",
    STR_ID_MATH_POINT_TWO_Param_Name,       "point 02",
    STR_ID_MATH_POINT_THREE_Param_Name,     "point 03",
    STR_ID_MATH_POINT_FOUR_Param_Name,      "point 04",
    STR_ID_MATH_POINT_FIVE_Param_Name,       "point 05",
    STR_ID_MATH_POINT_SIX_Param_Name,       "point 06",
    STR_ID_MATH_POINT_SEVEN_Param_Name,     "point 07",
    STR_ID_MATH_POINT_HEIGHT_Param_Name,     "point 08",
    STR_ID_MATH_POINT_NINE_Param_Name,      "point 09",
    STR_ID_MATH_POINT_TEN_Param_Name,       "point 10",
    STR_ID_MATH_TOPIC_CB_Param_Name,        "checkbox group",
    STR_ID_MATH_CB_ONE_Param_Name,          "cb 01",
    STR_ID_MATH_CB_TWO_Param_Name,          "cb 02",
    STR_ID_MATH_CB_THREE_Param_Name,        "cb 03",
    STR_ID_MATH_CB_FOUR_Param_Name,         "cb 04",
    STR_ID_MATH_CB_FIVE_Param_Name,         "cb 05",
    STR_ID_MATH_CB_SIX_Param_Name,          "cb 06",
    STR_ID_MATH_CB_SEVEN_Param_Name,        "cb 07",
    STR_ID_MATH_CB_HEIGHT_Param_Name,       "cb 08",
    STR_ID_MATH_CB_NINE_Param_Name,         "cb 09",
    STR_ID_MATH_CB_TEN_Param_Name,           "cb 10",
    STR_ID_MATH_TOPIC_COLORS_Param_Name,      "color group",
    STR_ID_MATH_COLOR_ONE_Param_Name,       "color 01",
    STR_ID_MATH_COLOR_TWO_Param_Name,       "color 02",
    STR_ID_MATH_COLOR_THREE_Param_Name,      "color 03",
    STR_ID_MATH_COLOR_FOUR_Param_Name,      "color 04",
    STR_ID_MATH_COLOR_FIVE_Param_Name,      "color 05",
    STR_ID_MATH_COLOR_SIX_Param_Name,       "color 06",
    STR_ID_MATH_COLOR_SEVEN_Param_Name,      "color 07",
    STR_ID_MATH_COLOR_HEIGHT_Param_Name,     "color 08",
    STR_ID_MATH_COLOR_NINE_Param_Name,      "color 09",
    STR_ID_MATH_COLOR_TEN_Param_Name,       "color 10",
    StrID_TOPIC_INPUTS_Param_Name,      "Inputs",
    StrID_LAYER_ONE_Param_Name,         "Layer input",
    StrID_TOFF_ONE_Param_Name,          "Frames Offset",
    StrID_POFF_ONE_Param_Name,          "Shift Center to",
	StrID_CEP_GETARB_Param_Name,         "get arb",
    StrID_CEP_REturnMessage_Param_Name,  "arb received",
    StrID_VersionError,                 "Error : your plugin version is lower than the preset",
    StrID_JSONError,                   "Error: error in JSON data reading"


};


char	*GetStringPtr(int strNum)
{
	return g_strs[strNum].str;
}



