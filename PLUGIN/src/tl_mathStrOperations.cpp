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
	//strReplace(str, "\\'", "\'");
}
 void tlmath::jsonCorrectorStr(std::string& str)
{
	strReplace(str, "(\\)", " ");
	strReplace(str, "\n", "\\n");
	strReplace(str, "\b", "\\b");
	strReplace(str, "\f", "\\f");
	strReplace(str, "\r", "\\r");
	strReplace(str, "\t", "\\t");
	//strReplace(str, "\'", "\\'");
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

std::vector<int> tlmath::getOccurenceFromStr(std::string src, std::string target) {
	int occ = 0;
    std::vector<int> indexVc;
	std::string::size_type pos = 0;
	while ((pos = src.find(target, pos)) != std::string::npos){
        indexVc.emplace_back(pos);
		++occ;
        pos += target.length();
	}
    indexVc.insert (indexVc.begin(), occ);//add the num off occurence at the begining of the vetor
    return indexVc;
}

std::string tlmath::ReIndexErrorInExpr(std::string originalfragSh,
	std::string  evalFragSh,
	std::string errIndex,
	size_t index,
	size_t delimiter) {
	std::string reducedStr = originalfragSh.substr(0, index);
	size_t nlignSt = tlmath::getOccurenceFromStr(reducedStr, "\n")[0];
	size_t numErrSt = tlmath::getOccurenceFromStr(evalFragSh, errIndex)[0];
	std::string exprStr = evalFragSh;
	if (numErrSt > 0) {
		std::string tmpStr = evalFragSh;
		for (int i = 0; i < numErrSt; i++) {
			size_t first = tmpStr.find(errIndex);
			size_t last = tmpStr.find(": '");
			std::string errLignStr = tmpStr.substr(first + errIndex.length(), last - (first + errIndex.length()));
			int errlignInt = atoi(errLignStr.c_str()) - int(nlignSt + 2); //+2 because we add first lign of the programm and the first lign of the expr function
            //if error position > delimiter break because the error appens in an other epxression
            if ( tlmath::getOccurenceFromStr(evalFragSh, errIndex)[numErrSt]> delimiter) {
				break;
			}
			std::string toReplaceStr = errIndex.append(errLignStr);
			std::string newStr = errIndex + std::to_string(errlignInt);
			strReplace(exprStr, toReplaceStr, newStr);
			tmpStr = tmpStr.substr(last + 1);
		}
	}
	return exprStr;
}
