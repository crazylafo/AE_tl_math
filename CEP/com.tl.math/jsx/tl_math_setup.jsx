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
    
  }
};








