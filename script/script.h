#include "json_to_js.h"


std::string script_callMathCEP = R"=====(
	//
    //  scipt TO CALL THE CEP 
    //  tlMath
    //
    //
	function callCEP (){
		var taskToExecute ="""function evtCepCall (){\r
				var externalObjectName = "PlugPlugExternalObject"; \r
				var csxslib = new ExternalObject( "lib:" + externalObjectName);\r
				var str = %s;
				var mathEventCEPCall = new CSXSEvent();\r
				mathEventCEPCall.type="setupOpeningFromPlugin";\r
				mathEventCEPCall.data=str;\r
				mathEventCEPCall.dispatch();\r
				}\r
			evtCepCall();""";
		app.scheduleTask(taskToExecute, 10, 1);
			}
		callCEP ()
		)=====";


std::string script_sendToMathCEP = R"=====(
	//
    //  scipt TO OPEN CEP IF NOT YET AND SEND DATA
    //  tlMath
    //
    //
	//void. send a csxsevent to call mathsetup 

	//function interpret answer of the cep. Uses a global variable. if smarter solution possible it may change
	function checkCepAnswer(compId, layerIndex, effectIndex){
		if (app.project.itemByID(compId).layer(layerIndex).effect(effectIndex).property(1).value !=1){
			//call CEP if not lauched	
            app.executeCommand(app.findMenuCommandId("tl Math Setup"));	
		}	
	}
    function sendToMathCEP( compId, layerIndex, effectIndex, arbData){
		checkCepAnswer(compId, layerIndex+1, effectIndex+1); //check checkbox value and reset it
        //send the arb value
		var externalObjectName = "PlugPlugExternalObject"; 
		var csxslib = new ExternalObject( "lib:" + externalObjectName);
		var mathEventToCEPObj = new CSXSEvent();
        mathEventToCEPObj.type="arbSentfromPlugin";
        mathEventToCEPObj.data=arbData;
        mathEventToCEPObj.dispatch();
		
    }

    sendToMathCEP( %i, %i, %i, %s);
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



