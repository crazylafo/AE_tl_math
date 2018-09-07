
#include "tl_math.h"

typedef struct {
	A_u_long	index;
	A_char		str[256];
} TableString;



TableString		g_strs[StrID_NUMTYPES] = {
	StrID_NONE,						"",
	StrID_Name,						"tl Maths",
	StrID_Description,				"Execute Math Geo in After Effects.",
    StrID_MRed_Param_Name,          "red variable",
    StrID_MGreen_Param_Name,        "green variable",
    StrID_MBlue_Param_Name,         "blue variable",
    StrID_MAlpha_Param_Name,        "alpha variable",
    StrID_Default_expr,              "layerTime_frame/(layerDuration*25)"
};


char	*GetStringPtr(int strNum)
{
	return g_strs[strNum].str;
}
	