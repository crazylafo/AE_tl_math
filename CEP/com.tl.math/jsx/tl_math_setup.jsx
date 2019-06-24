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
    if (!arbData){return};
    //send data;
    tlmathDataFromSetup = JSON.stringify(arbData);

    var propIndex = null;
    if (app.project.activeItem instanceof CompItem){
      var listSelectedProps = app.project.activeItem.selectedProperties;
      for (var i=0; i<listSelectedProps.length; i++){
        if (listSelectedProps[i].name == "tl_math-BETA"){
            propIndex = i;
            break;
        }     
      }
    }
    var scriptToExe = "var listSelectedProps = null; try{ listSelectedProps = app.project.activeItem.selectedProperties; if (listSelectedProps !=null){ listSelectedProps["+propIndex+"].property('get arb').setValue(1)}  } catch(e){listSelectedProps = null; alert ('select a tl Math Effect')}; ; ";
    return app.scheduleTask(scriptToExe, 200, false);

  }
};








