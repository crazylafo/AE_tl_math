/*************************************************************************
* ADOBE CONFIDENTIAL
* ___________________
*
* Copyright 2014 Adobe
* All Rights Reserved.
*
* NOTICE: Adobe permits you to use, modify, and distribute this file in
* accordance with the terms of the Adobe license agreement accompanying
* it. If you have received this file from a source other than Adobe,
* then your use, modification, or distribution of it requires the prior
* written permission of Adobe. 
**************************************************************************/

function onLoaded() {
    var csInterface = new CSInterface();
	
    var appName = csInterface.hostEnvironment.appName;
    
	loadJSX();

    updateThemeWithAppSkinInfo(csInterface.hostEnvironment.appSkinInfo);
	// Update the color of the panel when the theme color of the product changed.
	csInterface.addEventListener(CSInterface.THEME_COLOR_CHANGED_EVENT, onAppThemeColorChanged);

	csInterface.addEventListener("setupOpeningFromPlugin", function(plugIdObj){
		var stringifyObj =JSON.stringify(plugIdObj.data);
		var parseObj = JSON.parse (stringifyObj);
		var script_sendSignalToPlugin = " function sendSignalToPlugin (plugIdObj){ \r" +
			"app.project.itemByID("+parseObj.compId+").layer("+parseObj.layerIndex+").effect("+parseObj.effectIndex+").property(1).setValue(1);} \r"+
			"sendSignalToPlugin ()";
		csInterface.evalScript(script_sendSignalToPlugin);
	});

	
	
	csInterface.addEventListener("arbSentfromPlugin", function(fromArbEvent) {
		var script_closeSignalToPlugin = " function closeSignalToPlugin (plugIdObj){ \r" +
		"app.project.itemByID("+parseObj.compId+").layer("+parseObj.layerIndex+").effect("+parseObj.effectIndex+").property(1).setValue(0);} \r"+
		"closeSignalToPlugin ()";
		csInterface. evalScript(script_closeSignalToPlugin);
		$("#gl_frageditor").text(fromArbEvent.data.glFragExpr.toString());
	});
	
	defaultVal(); //load default ddl val
	
}


/**
 * Update the theme with the AppSkinInfo retrieved from the host product.
 */
function updateThemeWithAppSkinInfo(appSkinInfo) {
	
    //Update the background color of the panel
    var panelBackgroundColor = appSkinInfo.panelBackgroundColor.color;
    document.body.bgColor = toHex(panelBackgroundColor);
        
    var styleId = "ppstyle";
    
    var csInterface = new CSInterface();
	var appName = csInterface.hostEnvironment.appName;
    
    if(appName == "PHXS"){
	    addRule(styleId, "button, select, input[type=button], input[type=submit]", "border-radius:3px;");
	}
	if(appName == "PHXS" || appName == "AEFT" || appName == "PRLD") {
		////////////////////////////////////////////////////////////////////////////////////////////////
		// NOTE: Below theme related code are only suitable for Photoshop.                            //
		// If you want to achieve same effect on other products please make your own changes here.    //
		////////////////////////////////////////////////////////////////////////////////////////////////
		
	    
	    var gradientBg = "background-image: -webkit-linear-gradient(top, " + toHex(panelBackgroundColor, 40) + " , " + toHex(panelBackgroundColor, 10) + ");";
	    var gradientDisabledBg = "background-image: -webkit-linear-gradient(top, " + toHex(panelBackgroundColor, 15) + " , " + toHex(panelBackgroundColor, 5) + ");";
	    var boxShadow = "-webkit-box-shadow: inset 0 1px 0 rgba(255, 255, 255, 0.4), 0 1px 1px rgba(0, 0, 0, 0.2);";
	    var boxActiveShadow = "-webkit-box-shadow: inset 0 1px 4px rgba(0, 0, 0, 0.6);";
	    
	    var isPanelThemeLight = panelBackgroundColor.red > 127;
	    var fontColor, disabledFontColor;
	    var borderColor;
	    var inputBackgroundColor;
	    var gradientHighlightBg;
	    if(isPanelThemeLight) {
	    	fontColor = "#000000;";
	    	disabledFontColor = "color:" + toHex(panelBackgroundColor, -70) + ";";
	    	borderColor = "border-color: " + toHex(panelBackgroundColor, -90) + ";";
	    	inputBackgroundColor = toHex(panelBackgroundColor, 54) + ";";
	    	gradientHighlightBg = "background-image: -webkit-linear-gradient(top, " + toHex(panelBackgroundColor, -40) + " , " + toHex(panelBackgroundColor,-50) + ");";
	    } else {
	    	fontColor = "#ffffff;";
	    	disabledFontColor = "color:" + toHex(panelBackgroundColor, 100) + ";";
	    	borderColor = "border-color: " + toHex(panelBackgroundColor, -45) + ";";
	    	inputBackgroundColor = toHex(panelBackgroundColor, -20) + ";";
	    	gradientHighlightBg = "background-image: -webkit-linear-gradient(top, " + toHex(panelBackgroundColor, -20) + " , " + toHex(panelBackgroundColor, -30) + ");";
	    }
	    
	
	    //Update the default text style with pp values
	    
	    addRule(styleId, ".default", "font-size:" + appSkinInfo.baseFontSize + "px" + "; color:" + fontColor + "; background-color:" + toHex(panelBackgroundColor) + ";");
	    addRule(styleId, "button, select, input[type=text], input[type=button], input[type=submit]", borderColor);    
	    addRule(styleId, "button, select, input[type=button], input[type=submit]", gradientBg);    
	    addRule(styleId, "button, select, input[type=button], input[type=submit]", boxShadow);
	    addRule(styleId, "button:enabled:active, input[type=button]:enabled:active, input[type=submit]:enabled:active", gradientHighlightBg);
	    addRule(styleId, "button:enabled:active, input[type=button]:enabled:active, input[type=submit]:enabled:active", boxActiveShadow);
	    addRule(styleId, "[disabled]", gradientDisabledBg);
	    addRule(styleId, "[disabled]", disabledFontColor);
	    addRule(styleId, "input[type=text]", "padding:1px 3px;");
	    addRule(styleId, "input[type=text]", "background-color: " + inputBackgroundColor) + ";";
	    addRule(styleId, "input[type=text]:focus", "background-color: #ffffff;");
	    addRule(styleId, "input[type=text]:focus", "color: #000000;");
	    
	} else {
		// For AI, ID and FL use old implementation	
		addRule(styleId, ".default", "font-size:" + appSkinInfo.baseFontSize + "px" + "; color:" + reverseColor(panelBackgroundColor) + "; background-color:" + toHex(panelBackgroundColor, 20));
	    addRule(styleId, "button", "border-color: " + toHex(panelBgColor, -50));
	}
}

function addRule(stylesheetId, selector, rule) {
    var stylesheet = document.getElementById(stylesheetId);
    
    if (stylesheet) {
        stylesheet = stylesheet.sheet;
           if( stylesheet.addRule ){
               stylesheet.addRule(selector, rule);
           } else if( stylesheet.insertRule ){
               stylesheet.insertRule(selector + ' { ' + rule + ' }', stylesheet.cssRules.length);
           }
    }
}


function reverseColor(color, delta) {
    return toHex({red:Math.abs(255-color.red), green:Math.abs(255-color.green), blue:Math.abs(255-color.blue)}, delta);
}

/**
 * Convert the Color object to string in hexadecimal format;
 */
function toHex(color, delta) {
    function computeValue(value, delta) {
        var computedValue = !isNaN(delta) ? value + delta : value;
        if (computedValue < 0) {
            computedValue = 0;
        } else if (computedValue > 255) {
            computedValue = 255;
        }

        computedValue = Math.round(computedValue).toString(16);
        return computedValue.length == 1 ? "0" + computedValue : computedValue;
    }

    var hex = "";
    if (color) {
        with (color) {
             hex = computeValue(red, delta) + computeValue(green, delta) + computeValue(blue, delta);
        };
    }
    return "#" + hex;
}

function onAppThemeColorChanged(event) {
    // Should get a latest HostEnvironment object from application.
    var skinInfo = JSON.parse(window.__adobe_cep__.getHostEnvironment()).appSkinInfo;
    // Gets the style information such as color info from the skinInfo, 
    // and redraw all UI controls of your extension according to the style info.
    updateThemeWithAppSkinInfo(skinInfo);
} 

/**
 * Load JSX file into the scripting context of the product. All the jsx files in 
 * folder [ExtensionRoot]/jsx will be loaded. 
 */
function loadJSX() {
    var csInterface = new CSInterface();
	var extensionRoot = csInterface.getSystemPath(SystemPath.EXTENSION) + "/jsx/";
	csInterface.evalScript('$._ext.evalFiles("' + extensionRoot + '")');
}

function evalScript(script, callback) {
    new CSInterface().evalScript(script, callback);
}

function onClickButton(ppid) {
	var extScript = "$._ext_" + ppid + ".run()";
	evalScript(extScript);
}


function defaultVal(){
	document.getElementById("langSelec").onchange();
	openEditor(event, 'expr_red_tab', 'expr_red_editor');
	}

function openEditor(evt, tabName, editorName) {
		// Declare all variables
		var i, tabEditorList, glslGUILinks;

		// Get all elements with class="tabcontent" and hide them
		tabEditorList = document.getElementsByClassName("tabEditor");
		for (i = 0; i < tabEditorList.length; i++) {
			tabEditorList[i].style.display = "none";
		}


		// Get all elements with class="tablinks" and remove the class "active"
		glslGUILinks = document.getElementsByClassName("glslGUI");
		for (i = 0; i < glslGUILinks.length; i++) {
			glslGUILinks[i].className = glslGUILinks[i].className.replace(" active", "");
		}
		// Show the current tab, and add an "active" class to the button that opened the tab
		document.getElementById(tabName).style.display = "block";
		
		if (editorName.indexOf ('gl') != -1){
			glslEditor(editorName);
			}
		else{
			exprEditor(editorName);
			}
		evt.currentTarget.className += " active";
	} 

function mathGuiModeFunc(){
		var mathGui = document.getElementsByClassName("mathGUI");
		 var glslGui = document.getElementsByClassName("glslGUI");
		for (var i =0; i< mathGui.length; i++){
				$(mathGui[i]).show();
			}
			for (var i =0; i< glslGui.length; i++){
				$(glslGui[i]).hide();
			}

	}

function glslGuiModeFunc(){
		var mathGui = document.getElementsByClassName("mathGUI");
		 var glslGui = document.getElementsByClassName("glslGUI");
		for (var i =0; i< mathGui.length; i++){
				$(mathGui[i]).hide();
			}
			for (var i =0; i< glslGui.length; i++){
				$(glslGui[i]).show();
			}

	}
	
function langSelecFunc() {

		var langSelec = document.getElementById("langSelec").value;

		if (langSelec === "mExpr"){
			mathGuiModeFunc();
		  }
	  else if (langSelec ==="GLSL"){
			glslGuiModeFunc();
		  }

	 }
	
function defaultModeGui(){ 
		mathGuiModeFunc();
	}

function glslEditor(glMode){
		var editor = ace.edit(glMode);
		editor.setTheme("ace/theme/textemate");
		editor.session.setMode("ace/mode/glsl");
	}

function exprEditor(exprChan){
		var editor = ace.edit(exprChan);
		editor.setTheme("ace/theme/textemate");
		editor.session.setMode("ace/mode/javascript");
	}

