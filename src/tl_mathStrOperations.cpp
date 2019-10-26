#include "tl_math.h"

//fast copy/find/replace all method
std::string tlmath::strCopyAndReplace(std::string str,
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
void tlmath::strReplace(std::string& str,
	const std::string& oldStr,
	const std::string& newStr)
{
	std::string::size_type pos = 0u;
	while ((pos = str.find(oldStr, pos)) != std::string::npos) {
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
}

void tlmath::descriptionCorrectorStr (std::string& str){
    strReplace(str, "\\n", "\r");
}
void tlmath::scriptCorrectorStr(std::string& str)
{
	strReplace(str, "\\n", "\n");
	strReplace(str, "\\r", "\r");
	strReplace(str, "\\t", "  ");
	strReplace(str, "\\'", "\'");
}
 void tlmath::jsonCorrectorStr(std::string& str)
{
	//strReplace(str, "\\", " ");
	strReplace(str, "\n", "\\n");
	strReplace(str, "\b", "\\b");
	strReplace(str, "\f", "\\f");
	strReplace(str, "\r", "\\r");
	strReplace(str, "\t", "\\t");
	//strReplace(str, "\&", "\\&");

}

PF_Boolean tlmath::strToBoolean(std::string str)
{
	if (str == "1" || str == "true") {
		return true;
	}
	else {
		return false;
	}
}
