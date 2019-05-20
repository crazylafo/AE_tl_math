#include "json_to_js.h"


std::string script_callMathCEP = R"=====(
	//
    //  scipt TO CALL THE CEP 
    //  tlMath
    //
    //
	function callCEP(plugIdObj){
        
        var externalObjectName = "PlugPlugExternalObject";
        var csxslib = new ExternalObject( "lib:" + externalObjectName);
        var mathEventCEPCall = new CSXSEvent();
        mathEventCEPCall.type="tlmath.setupOpeningFromPlugin";
        mathEventCEPCall.data= plugIdObj;
        mathEventCEPCall.dispatch();
        }
    callCEP(%s);
    )=====";


std::string script_sendToMathCEP = R"=====(
	//
    //  scipt TO  SEND DATA TO CEP
    //  tlMath
    //
    //
    function sendToMathCEP(arbData){
        //send the arb value
		var externalObjectName = "PlugPlugExternalObject"; 
		var csxslib = new ExternalObject( "lib:" + externalObjectName);
		var mathEventToCEPObj = new CSXSEvent();
        mathEventToCEPObj.type="tlmath.arbSentfromPlugin";
        mathEventToCEPObj.data=arbData;
        mathEventToCEPObj.dispatch();
    }
    sendToMathCEP(%s);
)=====";



std::string script_getBackDataFromMathCEP = R"=====(
//
//  scipt TO CALL IN AEGP
//  tlMath
//
//

//get variable string from cep


//alert hey we've got it

//delte the variable to free the memory (poor AE memory)


)=====";



