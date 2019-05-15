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




