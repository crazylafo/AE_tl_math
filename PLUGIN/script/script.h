#include "json_to_js.h"


std::string script_callMathCEP = R"=====(
	//
    //  scipt TO CALL THE CEP 
    //  tlMath
    //
    //

	function callCEP(plugIdObj, compId, layerIndex, effectIndex){
		tlmathPluginId = [compId, layerIndex, effectIndex]; //global variable to communicate between plugin and CEP, when cep is opened.
        var externalObjectName = "PlugPlugExternalObject";
        var csxslib = new ExternalObject( "lib:" + externalObjectName);
        var mathEventCEPCall = new CSXSEvent();
        mathEventCEPCall.type="tlmath.setupOpeningFromPlugin";
        mathEventCEPCall.data= plugIdObj;
        mathEventCEPCall.dispatch();
        }
		try{
			callCEP(%s, %d, %d, %d);	
		}catch(e){
			alert ("error calling cep: "+e)
		}
    )=====";


std::string script_sendToMathCEP = R"=====(
	//
    //  scipt TO  SEND DATA TO CEP
    //  tlMath
    //
    //
    function sendToMathCEP(arbData){
        //send the arb data in a event
		var externalObjectName = "PlugPlugExternalObject"; 
		var csxslib = new ExternalObject( "lib:" + externalObjectName);
		var mathEventToCEPObj = new CSXSEvent();
        mathEventToCEPObj.type="tlmath.arbSentfromPlugin";
        mathEventToCEPObj.data=arbData;
        mathEventToCEPObj.dispatch();
    }
    try{
		sendToMathCEP(%s);
    }catch(e){
        alert("error sending Data To CEP: "+e);
    }
    
)=====";


std::string script_getDataBackFromMathCEP  =R"=====(
//
//  scipt TO CALL IN AEGP
//  tlMath
//
//

//get variable string from cep
function getDataFromCEP(){
    if (!tlmathDataFromSetup){return}
    var result = tlmathDataFromSetup.toString();
    tlmathDataFromSetup = null;//delte the variable to free the memory (poor AE memory)
	return result;
}
try{
    getDataFromCEP();
}catch(e){
    alert ("error getting Data From CEP: " +e);
}



)=====";



