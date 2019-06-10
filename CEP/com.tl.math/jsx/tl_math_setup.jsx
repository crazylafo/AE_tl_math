/*************************************************************************
* tl math plugin and CEP
*thomas laforge  Copyright 2019
**************************************************************************/
//global variables


/*@in empty
 @return bool*/
function isCompActiveItem(){
    if (app.project.activeItem instanceof (compItem)){
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
      alert(arbData);
      //send data;
      tlmathDataFromSetup = arbData;

      if (app.project.activeitem instanceof compItem){
        var listSelectedProps = app.project.activeitem.selectedProperties;
        for (var i=0; i<listSelectedProps.length; i++){
          alert (listSelectedProps[i].name);
          alert (listSelectedProps[i].layer);
        }
      }

  }
};





