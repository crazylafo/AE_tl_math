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


if ( ! $._ext )
{
  $._ext = {};
}

$._ext = {
  sendDataToPlugin : function(arbData)
  {
      //send data;
      tlmathDataFromSetup = arbData;
      var propIndex = null;
      if (app.project.activeItem instanceof CompItem){
        var listSelectedProps = app.project.activeItem.selectedProperties;
        for (var i=0; i<listSelectedProps.length; i++){
          if (listSelectedProps[i].name == "tl_math-BETA"){
              propIndex = i;
          }     
        }
      }
      if (propIndex !=null){
        app.scheduleTask("var listSelectedProps = app.project.activeItem.selectedProperties; \r"+
                          "listSelectedProps["+propIndex+"].property('get arb').setValue(1);", 1, false);
        return 
      }
  }
};





