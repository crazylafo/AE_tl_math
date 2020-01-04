/*************************************************************************
* tl math plugin and CEP
*thomas laforge  Copyright 2020
**************************************************************************/
function getDate() {
  var x = new Date();
  var y = x.getFullYear().toString();
  var m = (x.getMonth() + 1).toString();
  var d = x.getDate().toString();
  (d.length == 1) && (d = '0' + d);
  (m.length == 1) && (m = '0' + m);
  var yyyymmdd = y + m + d;
  return yyyymmdd;
}
function compareObjStr(a,b){
  var bandA = a.name.toUpperCase();
  var bandB = b.name.toUpperCase();
  var comparison = 0;
  if (bandA > bandB) {
    comparison = 1;
  } else if (bandA < bandB) {
    comparison = -1;
  }
  return comparison;
}
function copyArrayStr (arr){
  var newArr = [];
  for (var i=0; i< arr.length; i++){
    newArr.push (arr[i].toString());
  }
  return newArr;
}

function getLastDotOfFile (scannedFile){
  var dot = 0;
  dot = scannedFile.fsName.lastIndexOf(".");
  return dot;
  }
function createUserDataFolder(plugIdStr){
    var userLibFolder = new Folder (Folder.userData);
    var folderStr = userLibFolder.absoluteURI.toString()+"/tl";
    var tlFolder =  new Folder (folderStr);
    if (!tlFolder.exists){
      tlFolder.create();
    }
    var thisPluginFolder =  new Folder (tlFolder.absoluteURI.toString()+"/"+plugIdStr);
    if (!thisPluginFolder.exists){
      thisPluginFolder.create();
      }
    return thisPluginFolder;
}
function createUserPresetFolder(plugIdStr){
    var userFolder = createUserDataFolder(plugIdStr);
    var userPresetsFolder =new Folder (userFolder.absoluteURI.toString()+"/presets");
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
function searchFileInFolder(presetFileName, presetFolder, extensionPath){
    var iconFile = null;
    var filePathStr = extensionPath+"/imgs/tl_defaultPreset.png";
    var newIconFolder = Folder (presetFolder);
    var iconFileName = presetFileName.toString().substr(0,presetFileName.lastIndexOf("."))+".png";
    try{
      newFile = newIconFolder.getFiles(iconFileName);
      iconFile = new File (newFile);
      iconFileStr = (iconFile.fsName).toString();
      if (typeof (iconFile)=== "undefined" || newFile.length==0){
        return filePathStr;
      }
    }
    catch (e){
      return filePathStr;
    } 
    return iconFileStr;
    
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
    try{
      if ( typeof tlmathPluginId==="undefined" || !tlmathPluginId || tlmathPluginId ==null ){return};
      app.project.activeItem.layer(tlmathPluginId[1]).effect(tlmathPluginId[2]).property("arb received").setValue(1);
      tlmathPluginId = undefined;
    }catch(e){
      return
    }
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
      if (listSelectedProps[i].name.indexOf("tl_math") !=-1){
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
          preset.tags = copyArrayStr (jsonObj.effectInfo.tags);
          preset.tags.unshift (preset.jsonPath.toString());
          preset.description = jsonObj.effectInfo.description;
          preset.icon =  searchFileInFolder (preset.fileName, preset.parentFolder.toString(), objData.extensionPath);
          preset.str = jsonTemp.toString();
          listJsonFiles.preset[i] = preset;
          listJsonFiles.length =i+1;
        }
      }
    }
    listJsonFiles.preset.sort(compareObjStr);
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
    var presetFileStr =dataStr.effectInfo.presetName+".JSON";
    var scanFolder =userPresetsFolder.getFiles(presetFileStr);
     var presetFile;
    if (scanFolder.length >0){
      var date = getDate();
      presetFile = new File (userPresetsFolder.absoluteURI+"/"+dataStr.effectInfo.presetName+date.toString()+".JSON");
    }else{
      presetFile = new File (userPresetsFolder.absoluteURI+"/"+dataStr.effectInfo.presetName+".JSON");
    }
    
      if (presetFile.open("w")){
         presetFile.encoding ='UTF-8';
         presetFile.write(JSON.stringify(dataStr,undefined,'\r\n'));
         presetFile.close();
      } 
     
    },
};









