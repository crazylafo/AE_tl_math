//
//  scipt.h
//  tlMath
//
//


#include "script_main.h"
#include "json_to_js.h"
#ifndef scipt_h
#define scipt_h




std::string evalScript = "exprScript(%s, %s, %s, %s);";


std::string script_ae = jsontojs.append(script_main_functions)
								  .append(script_main_ui)
	                              .append(script_main_ui_callbacks);


#endif /* scipt_h */
