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
	std::string reducedStr = originalfragSh.substr(0, index); //from begining of current shader to the begening of the expr
    std::string startToDelimiter =originalfragSh.substr(0, delimiter); // from begining of shader to the end the expr
	size_t nlignSt = tlmath::getOccurenceFromStr(reducedStr, "\n")[0]; // how many line breaks before the current shader
    size_t nlignDelimiterSt = tlmath::getOccurenceFromStr( startToDelimiter, "\n")[0]; // how many line breaks before the end of the cirrent shader
	size_t numErrSt = tlmath::getOccurenceFromStr(evalFragSh, errIndex)[0]; // how manny error in shader.
	std::string exprStr = evalFragSh; // start from original error repport
	if (numErrSt > 0) {
		std::string tmpStr = evalFragSh;
        int numErrChInt = 0;
		for (int i = 0; i < numErrSt; i++) {
			size_t first = tmpStr.find(errIndex);
			size_t last = (first+6)+tmpStr.substr(first+first+6).find(": ");
			std::string errLignStr = tmpStr.substr(first + errIndex.length(), last - (first + errIndex.length()));
            int originalErrLignInt =atoi(errLignStr.c_str());

            if (originalErrLignInt< nlignSt || // if the error is before current expression channel or after. break
                originalErrLignInt>nlignDelimiterSt ) {
                if (numErrChInt ==0){
                    exprStr = compile_success;
                }
                break;
            }
			int errlignInt =  originalErrLignInt- int(nlignSt + 1); //+1 because we add first lign of the programm and the first lign of the expr function

            std::string toReplaceStr;
            AEFX_CLR_STRUCT(toReplaceStr);
            toReplaceStr = errIndex;
            toReplaceStr.append(errLignStr);

            std::string newStr;
            AEFX_CLR_STRUCT(newStr);
            newStr = errIndex;
            newStr.append(std::to_string(errlignInt));
            
			strReplace(exprStr, toReplaceStr, newStr);
			tmpStr = tmpStr.substr(last + 1);
            numErrChInt ++;
		}
	}
	return exprStr;
}
