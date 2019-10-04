#include "tl_math.h"

//fast copy/find/replace all method
std::string strCopyAndReplace(std::string str,
	const std::string& oldStr,
	const std::string& newStr)
{
	std::string copyStr = str;
	std::string::size_type pos = 0u;
	while ((pos = copyStr.find(oldStr, pos)) != std::string::npos) {
		copyStr.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
	return copyStr;

}

//quick find/replace all method
void strReplace(std::string& str,
	const std::string& oldStr,
	const std::string& newStr)
{
	std::string::size_type pos = 0u;
	while ((pos = str.find(oldStr, pos)) != std::string::npos) {
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
}

void descriptionCorrectorStr (std::string& str){
    strReplace(str, "\\n", "\r");
}
void scriptCorrectorStr(std::string& str)
{
	strReplace(str, "\\n", "\n");
	strReplace(str, "\\r", "\r");
	strReplace(str, "\\t", "  ");
	strReplace(str, "\\'", "\'");
}

void ExprtkCorrectorStr(std::string& str)
{
	scriptCorrectorStr(str);
	//convert some AE javascript operator to exprtk operators
	strReplace(str, "&&", "&");
	strReplace(str, "||", "|");
	strReplace(str, "++", "+=1");
	strReplace(str, "--", "-=1");
	strReplace(str, " = ", " := ");
	strReplace(str, "\t", "    ");
	strReplace(str, "\"", " '");
}
 void jsonCorrectorStr(std::string& str)
{
	//strReplace(str, "\\", " ");
	strReplace(str, "\n", "\\n");
	strReplace(str, "\b", "\\b");
	strReplace(str, "\f", "\\f");
	strReplace(str, "\r", "\\r");
	strReplace(str, "\t", "\\t");
}

PF_Boolean strToBoolean(std::string str)
{
	if (str == "1" || str == "true") {
		return true;
	}
	else {
		return false;
	}
}
