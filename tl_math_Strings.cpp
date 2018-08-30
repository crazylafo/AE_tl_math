/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

#include "tl_math.h"

typedef struct {
	A_u_long	index;
	A_char		str[256];
} TableString;



TableString		g_strs[StrID_NUMTYPES] = {
	StrID_NONE,						"",
	StrID_Name,						"tl Maths",
	StrID_Description,				"Execute Math Geo in After Effects.",
    StrID_MRed_Param_Name,          "red Intesity",
    StrID_MGreen_Param_Name,        "green Intesity",
    StrID_MBlue_Param_Name,         "blue Intesity",
    StrID_MAlpha_Param_Name,        "alpha Intesity",
    StrID_MExecute_Param_Name,      "Execute"
};


char	*GetStringPtr(int strNum)
{
	return g_strs[strNum].str;
}
	