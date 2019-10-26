/*************************************************************************
* tl math plugin and CEP
*thomas laforge  Copyright 2019
**************************************************************************/
//global variables
function getLastDotOfFile (scannedFile){
  var dot = 0;
  dot = scannedFile.fsName.lastIndexOf(".");
  return dot;
  }
function createUserDataFolder(plugIdStr){
      var userLibFolder = new Folder (Folder.userData);
    var tlFolder =  new Folder (userLibFolder.absoluteURI+"\\tl");
    if (!tlFolder.exists){
      tlFolder.create();
    }
    var thisPluginFolder =  new Folder (tlFolder.absoluteURI+"\\"+plugIdStr);
    if (!thisPluginFolder.exists){
      thisPluginFolder.create();
      }
    return thisPluginFolder;
}
function createUserPresetFolder(plugIdStr){
    var userFolder = createUserDataFolder(plugIdStr);
    var userPresetsFolder =new Folder (userFolder.absoluteURI+"\\presets");
    if(!userPresetsFolder.exists){
      userPresetsFolder .create();
      }
    return userPresetsFolder;
}
function getLastSlashofFilePath (scannedFile){
  var slash = 0;
  if ($.os.indexOf ("Windows") !=-1){
      var delimitationStr = "\\";
      slash = scannedFile.fsName.lastIndexOf (delimitationStr);
      }
      else {
      slash =  scannedFile.fsName.lastIndexOf("/");
      }
  return slash;
  }
function searchFileInFolder (presetFileName, presetFolder, extensionPath){
    var iconFile = null;
    var newIconFolder = Folder (presetFolder.fsName);
    var iconFileName = presetFileName.toString().substr(0,presetFileName.lastIndexOf("."))+".png";
      try{
        iconFile = newIconFolder.getFiles(iconFileName);
        if (typeof (iconFile)=== "undefined" || iconFile.length==0){
          iconFile = extensionPath+"/imgs/tl_defaultPreset.png";
        }
    }
    catch (e){
      iconFile = extensionPath+"/imgs/tl_defaultPreset.png";
    }
    return iconFile;
    
};

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
  sendMessageToPlugin : function(){
    //try{ pluginId} catch(e){return};
    if ( typeof pluginId==="undefined" || !pluginId || pluginId ==null ){return};
    app.project.activeItem.layer(pluginId[1]).effect(pluginId[2]).property("arb received").setValue(1);
    pluginId = undefined;
  }, 
  sendDataToPlugin : function(arbData){
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
          listSelectedProps[propIndex].property("get arb").setValue(1);
          break;
      }     
    }
  },
  loadJSONFile : function(){
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
  listJsonFiles : function (objData){
    var folderPluginPath = objData.folderPluginpresetsPath;
    var pluginFolder = new Folder (folderPluginPath);
    var folderPresetPath = createUserPresetFolder(objData.plugIdStr);
    if (!pluginFolder || !folderPresetPath){return};
    var listJsonFiles = {};
    listJsonFiles.preset = [];
    var userFolder = new Folder (folderPresetPath);
    var jsonFiles = pluginFolder.getFiles("*.json");
    var userJsonFiles = userFolder.getFiles("*.json");

    if (typeof (userJsonFiles) !="undefined"){
      for(var i=0;i<userJsonFiles.length; i++){
        jsonFiles.push(userJsonFiles[i]);
        }
      }
    if(!jsonFiles || typeof (jsonFiles)==="undefined"){return};
    for (var i=0; i<jsonFiles.length; i++){
      if (jsonFiles[i].open('r')){
        var jsonTemp = jsonFiles[i].read();
        jsonFiles[i].close();
        var jsonObj = JSON.parse(jsonTemp.toString());
        if (jsonObj.effectInfo.effectName =="tlMath"){
          var preset = {};
          preset.jsonPath = jsonFiles[i];
          var dot = getLastDotOfFile (preset.jsonPath);
          var slash = getLastSlashofFilePath (preset.jsonPath);
          preset.fileExt =  preset.jsonPath.fsName.substr( dot, preset.jsonPath.length);
          preset.fileName = preset.jsonPath.fsName.substr (slash+1, dot )
          preset.parentFolder = preset.jsonPath.fsName.substr (0, slash);
          preset.name = jsonObj.effectInfo.presetName;
          preset.tags = jsonObj.effectInfo.tags;
          preset.tags.unshift (preset.name);
          preset.description = jsonObj.effectInfo.description;
         
          preset.icon =  searchFileInFolder (preset.fileName, preset.parentFolder, objData.extensionPath);
          preset.str = jsonTemp.toString();
          listJsonFiles.preset[i] = preset;
          listJsonFiles.length =i+1;
        }
      }
    }
    listJsonFiles.preset.sort();
    var newData = JSON.stringify (listJsonFiles);
    var externalObjectName = "PlugPlugExternalObject"; 
    var csxslib = new ExternalObject( "lib:" + externalObjectName);
    var mathEventPreset = new CSXSEvent();
    mathEventPreset.type="tlmath.preset";
    mathEventPreset.data=newData;
    mathEventPreset.dispatch();
    },
  exportPresetFile : function(dataStr){
    var presetFile =File.saveDialog('save your preset as a json');
    if (presetFile && presetFile.open('w')){
        presetFile.encoding ='UTF-8';
        presetFile.write(JSON.stringify(dataStr,undefined, '\r\n'));
        presetFile.close();
      }
    },
  exportPresetFileToUserLib : function(dataStr){
    var plugIdStr = dataStr.effectInfo.effectName.toString()+dataStr.effectInfo.pluginVersion; 
    var userPresetsFolder = createUserPresetFolder(plugIdStr);
    var presetFile = new File (userPresetsFolder.absoluteURI+"\\"+dataStr.effectInfo.presetName+".JSON");
      if (presetFile.open("w")){
         presetFile.encoding ='UTF-8';
         presetFile.write(JSON.stringify(dataStr,undefined, '\r\n'));
         presetFile.close();
      } 
     
    },
};









