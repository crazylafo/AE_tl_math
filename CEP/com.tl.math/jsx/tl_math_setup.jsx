/*************************************************************************
* tl math plugin and CEP
*thomas laforge  Copyright 2019
**************************************************************************/
//global variables


/*@in empty
 @return bool*/
function isCompActiveItem(){
    if (app.project.activeItem instanceof (CompItem)){
        return true
    }
    else{ 
        alert ("please select a composition");
        return false}
}


function alertSelecEffect(){
  alert ('select a tl Math Effect');
}

if ( ! $._ext )
{
  $._ext = {};
}

$._ext = {
  sendMessageToPlugin : function()
  {
    try{ pluginId} catch(e){return};
    if (!pluginId || pluginId ==null){return};
    app.project.activeItem.layer(pluginId[0]).effect(pluginId[1]).property(57).setValue(1);//property 567= send message to ARB
    pluginId = undefined;
  },
  sendDataToPlugin : function(arbData)
  {
    if (!arbData){return};
    //send data;
    tlmathDataFromSetup = JSON.stringify(arbData);
    var propIndex = null;
    if (!app.project.activeItem instanceof CompItem){alertSelecEffect(); return }
    var listSelectedProps = app.project.activeItem.selectedProperties;
    if (!listSelectedProps[0]){alertSelecEffect(); return }
    for (var i=0; i<listSelectedProps.length; i++){
      if (listSelectedProps[i].name == "tl_math-BETA"){
          propIndex = i;
          listSelectedProps[propIndex].property(56).setValue(1);//property 56 = get arb. can't find it by name because hidden
          break;
      }     
    }
  },

  loadJSONFile : function()
  {
    var newFile = null;
    var newJSON = null;
    var loadFile =File.openDialog('Select the file to load');
    if (loadFile && loadFile.open('r')){
        loadFile.encoding ='UTF-8';
        newFile = loadFile.read();
         newJSON = newFile.toString();
         loadFile.close();
    }
    if (newJSON != null){
      var externalObjectName = "PlugPlugExternalObject"; 
      var csxslib = new ExternalObject( "lib:" + externalObjectName);
      var mathEventToCEPObj = new CSXSEvent();
          mathEventToCEPObj.type="tlmath.arbSentfromPreset";
          mathEventToCEPObj.data=newJSON;
          mathEventToCEPObj.dispatch();
    }
  },

  savePresetFile : function(dataStr)
  {
    var presetFile =File.saveDialog('save your preset as a json');
    if (presetFile && presetFile.open('w')){
        presetFile.encoding ='UTF-8';
        presetFile.write(JSON.stringify(dataStr,undefined, '\r\n'));
        presetFile.close();
    }

  }

};








