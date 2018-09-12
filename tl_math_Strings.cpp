
#include "tl_math.h"

typedef struct {
	A_u_long	index;
	A_char		str[256];
} TableString;



TableString		g_strs[StrID_NUMTYPES] = {
	StrID_NONE,						    "",
	StrID_Name,						    "tl Maths",
	StrID_Description,				    "Execute Math Geo in After Effects.",
    StrID_INPUTONE_Param_Name,          "variable 1",
    StrID_INPUTTWO_Param_Name,          "variable 2",
    StrID_INPUTTHREE_Param_Name,        "variable 3",
    StrID_INPUTFOUR_Param_Name,         "variable 4",
    StrID_Default_expr,              "layerTime_frame/(layerDuration*25)"
};


char	*GetStringPtr(int strNum)
{
	return g_strs[strNum].str;
}
	