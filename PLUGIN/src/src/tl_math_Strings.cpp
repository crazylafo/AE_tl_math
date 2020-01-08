
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
    StrID_EffectDescription,            "Description",
    STR_ID_MATH_TOPIC_POINTS_Param_Name,   "slider_group",
    STR_ID_MATH_SLIDER_ONE_Param_Name,      "slider01",
    STR_ID_MATH_SLIDER_TWO_Param_Name,      "slider02",
    STR_ID_MATH_SLIDER_THREE_Param_Name,    "slider03",
    STR_ID_MATH_SLIDER_FOUR_Param_Name,     "slider04",
    STR_ID_MATH_SLIDER_FIVE_Param_Name,     "slider05",
    STR_ID_MATH_SLIDER_SIX_Param_Name,      "slider06",
    STR_ID_MATH_SLIDER_SEVEN_Param_Name,    "slider07",
    STR_ID_MATH_SLIDER_HEIGHT_Param_Name,   "slider08",
    STR_ID_MATH_SLIDER_NINE_Param_Name,     "slider09",
    STR_ID_MATH_SLIDER_TEN_Param_Name,      "slider10",
    STR_ID_MATH_TOPIC_POINTS_Param_Name,    "point_group",
    STR_ID_MATH_POINT_ONE_Param_Name,       "point01",
    STR_ID_MATH_POINT_TWO_Param_Name,       "point02",
    STR_ID_MATH_POINT_THREE_Param_Name,     "point03",
    STR_ID_MATH_POINT_FOUR_Param_Name,      "point04",
    STR_ID_MATH_POINT_FIVE_Param_Name,       "point05",
    STR_ID_MATH_POINT_SIX_Param_Name,       "point06",
    STR_ID_MATH_POINT_SEVEN_Param_Name,     "point07",
    STR_ID_MATH_POINT_HEIGHT_Param_Name,     "point08",
    STR_ID_MATH_POINT_NINE_Param_Name,      "point09",
    STR_ID_MATH_POINT_TEN_Param_Name,       "point10",
    STR_ID_MATH_TOPIC_CB_Param_Name,        "checkbox_group",
    STR_ID_MATH_CB_ONE_Param_Name,          "cb01",
    STR_ID_MATH_CB_TWO_Param_Name,          "cb02",
    STR_ID_MATH_CB_THREE_Param_Name,        "cb03",
    STR_ID_MATH_CB_FOUR_Param_Name,         "cb04",
    STR_ID_MATH_CB_FIVE_Param_Name,         "cb05",
    STR_ID_MATH_CB_SIX_Param_Name,          "cb06",
    STR_ID_MATH_CB_SEVEN_Param_Name,        "cb07",
    STR_ID_MATH_CB_HEIGHT_Param_Name,       "cb08",
    STR_ID_MATH_CB_NINE_Param_Name,         "cb09",
    STR_ID_MATH_CB_TEN_Param_Name,           "cb10",
    STR_ID_MATH_TOPIC_COLORS_Param_Name,      "color_group",
    STR_ID_MATH_COLOR_ONE_Param_Name,       "color01",
    STR_ID_MATH_COLOR_TWO_Param_Name,       "color02",
    STR_ID_MATH_COLOR_THREE_Param_Name,      "color03",
    STR_ID_MATH_COLOR_FOUR_Param_Name,      "color04",
    STR_ID_MATH_COLOR_FIVE_Param_Name,      "color05",
    STR_ID_MATH_COLOR_SIX_Param_Name,       "color06",
    STR_ID_MATH_COLOR_SEVEN_Param_Name,      "color07",
    STR_ID_MATH_COLOR_HEIGHT_Param_Name,     "color08",
    STR_ID_MATH_COLOR_NINE_Param_Name,      "color09",
    STR_ID_MATH_COLOR_TEN_Param_Name,       "color10",
	STR_ID_MATH_TOPIC_ROTS_Param_Name,     "rotation_group",
	STR_ID_MATH_ROT_ONE_Param_Name,         "rotation01",
	STR_ID_MATH_ROT_TWO_Param_Name,         "rotation02",
	STR_ID_MATH_ROT_THREE_Param_Name,		"rotation03",
	STR_ID_MATH_ROT_FOUR_Param_Name,		"rotation04",
	STR_ID_MATH_ROT_FIVE_Param_Name,		"rotation05",
	STR_ID_MATH_ROT_SIX_Param_Name,			"rotation06",
	STR_ID_MATH_ROT_SEVEN_Param_Name,		"rotation07",
	STR_ID_MATH_ROT_HEIGHT_Param_Name,		"rotation08",
	STR_ID_MATH_ROT_NINE_Param_Name,		"rotation09",
	STR_ID_MATH_ROT_TEN_Param_Name,			"rotation10",

    StrID_TOPIC_INPUTS_Param_Name,      "Inputs",
    StrID_LAYER_ONE_Param_Name,         "Layer1 input",
    StrID_TOFF_ONE_Param_Name,          "layer1 Frames Offset",
	StrID_LAYER_TWO_Param_Name,         "Layer2 input",
	StrID_TOFF_TWO_Param_Name,          "layer2 Frames Offset",
	StrID_LAYER_THREE_Param_Name,         "Layer3 input",
	StrID_TOFF_THREE_Param_Name,          "layer3 Frames Offset",
	StrID_LAYER_FOUR_Param_Name,         "Layer4 input",
	StrID_TOFF_FOUR_Param_Name,          "layer4 Frames Offset",
	StrID_CEP_GETARB_Param_Name,         "get arb",
	StrID_MATH_RESET_Param_Name,                "reset preset",
    StrID_CEP_REturnMessage_Param_Name,  "arb received",
    StrID_VersionError,                 "Error : your plugin version is lower than the preset",
    StrID_JSONError,                   "Error: error in JSON data reading"


};


char	*GetStringPtr(int strNum)
{
	return g_strs[strNum].str;
}



