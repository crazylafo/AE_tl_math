//
//  scipt.h
//  tlMath
//
//
#include <iostream>
#include <string>
#ifndef scipt_h
#define scipt_h

std::string script_parseBoolToInt = "function parseBoolToInt (bin){ \n\
alert (bin)\n\
if (bin === 'false'|| bin === 'FALSE'  ){return 0}\n\
else {return 1}\n\
}";
std::string script_parseIntToBool = "function parseIntToBool (intvar){ \n\
if (intvar == 0){return false}\n\
else {return true}\n\
}";

std::string script_getcallBacks = "function getcallBacks(w){\n\
    var exprCb = {};\n\
    exprCb.parserModeB   =  parseIntToBool(w.grp.parserModeB.ddl.selection); \n\
    exprCb.presetName   =   w.grp.PresetN.name.text; \n\
    exprCb.description  =   w.grp.tab.paramUI.descriptionGrp.description.text; \n\
    exprCb.redExpr      =   w.grp.tab.expr.redC.redet.text;\n\
    exprCb.greenExpr    =   w.grp.tab.expr.greenC.greenet.text;\n\
    exprCb.blueExpr     =   w.grp.tab.expr.blueC.blueet.text;\n\
    exprCb.alphaExpr    =   w.grp.tab.expr.alphaC.alphaet.text;\n\
    exprCb.glslExpr     =   w.grp.tab.glsl.fragSh.fragShet.text;\n\
	exprCb.uiSliderGrpVisible  = w.grp.tab.paramUI.param.paramslider.grpName.cbvisible.value;\n\
	exprCb.uiSliderGrpName = w.grp.tab.paramUI.param.paramslider.grpName.editNameet.text;\n\
	exprCb.uiSlider1Visible = w.grp.tab.paramUI.param.paramslider.paramSlider1.cbvisible.value; \n\
	exprCb.uiSlider1Name = w.grp.tab.paramUI.param.paramslider.paramSlider1.editNameet.text;\n\
	exprCb.uiSlider2Visible = w.grp.tab.paramUI.param.paramslider.paramSlider2.cbvisible.value;\n\
	exprCb.uiSlider2Name = w.grp.tab.paramUI.param.paramslider.paramSlider2.editNameet.text;\n\
	exprCb.uiSlider3Visible = w.grp.tab.paramUI.param.paramslider.paramSlider3.cbvisible.value;\n\
	exprCb.uiSlider3Name = w.grp.tab.paramUI.param.paramslider.paramSlider3.editNameet.text;\n\
	exprCb.uiSlider4Visible = w.grp.tab.paramUI.param.paramslider.paramSlider4.cbvisible.value;\n\
	exprCb.uiSlider4Name = w.grp.tab.paramUI.param.paramslider.paramSlider4.editNameet.text;\n\
	exprCb.uiPointGrpVisible = w.grp.tab.paramUI.param.parampoint.grpName.cbvisible.value;\n\
	exprCb.uiPointGrpName= w.grp.tab.paramUI.param.parampoint.grpName.editNameet.text;\n\
	exprCb.uiPoint1Visible =w.grp.tab.paramUI.param.parampoint.parampoint1.cbvisible.value;\n\
	exprCb.uiPoint1Name = w.grp.tab.paramUI.param.parampoint.parampoint1.editNameet.text;\n\
	exprCb.uiPoint2Visible = w.grp.tab.paramUI.param.parampoint.parampoint2.cbvisible.value;\n\
	exprCb.uiPoint2Name = w.grp.tab.paramUI.param.parampoint.parampoint2.editNameet.text;\n\
	exprCb.uiColorGrpVisible =w.grp.tab.paramUI.param.paramcolor.grpName.cbvisible.value;\n\
	exprCb.uiColorGrpName = w.grp.tab.paramUI.param.paramcolor.grpName.editNameet.text;\n\
	exprCb.uiColor1Visible = w.grp.tab.paramUI.param.paramcolor.paramcolor1.cbvisible.value;\n\
	exprCb.uiColor1Name = w.grp.tab.paramUI.param.paramcolor.paramcolor1.editNameet.text;\n\
	exprCb.uiColor2Visible = w.grp.tab.paramUI.param.paramcolor.paramcolor2.cbvisible.value;\n\
	exprCb.uiColor2Name = w.grp.tab.paramUI.param.paramcolor.paramcolor2.editNameet.text;\n\
	exprCb.extLGrpVisible = w.grp.tab.paramUI.param.paramextL.grpName.cbvisible.value;\n\
	exprCb.extLGrpName =w.grp.tab.paramUI.param.paramextL.grpName.editNameet.text;\n\
	return exprCb ;\n\
}";

std::string json_createJson = "function createJson(exprCl, pluginVersion,  inEvalB){\n\
	pluginVersionStr = pluginVersion.toString();\n\
    ExprObj = {\n\
        effectName   : \"tlMath\",\n\
        parserModeB : exprCl.parserModeB, \n\
        category :  \"Custom\",\n\
        presetName  : exprCl.presetName,\n\
        description : exprCl.description, \n\
        pluginVesion : pluginVersionStr,\n\
        minimalPluginVersion : \"1.13\",\n\
        glslExpr  : exprCl.glslExpr,\n\
		evalB :     inEvalB,\n\
        redExpr   : exprCl.redExpr,\n\
        greenExpr : exprCl.greenExpr,\n\
        blueExpr  : exprCl.blueExpr,\n\
        alphaExpr : exprCl.alphaExpr,\n\
		uiSliderGrpVisible :	exprCl.uiSliderGrpVisible,\n\
		uiSliderGrpName :  exprCl.uiSliderGrpName,\n\
		uiSlider1Visible : exprCl.uiSlider1Visible,\n\
		uiSlider1Name : exprCl.uiSlider1Name,\n\
		uiSlider2Visible : exprCl.uiSlider2Visible,\n\
		uiSlider2Name : exprCl.uiSlider2Name,\n\
		uiSlider3Visible : exprCl.uiSlider3Visible,\n\
		uiSlider3Name : exprCl.uiSlider3Name,\n\
		uiSlider4Visible : exprCl.uiSlider4Visible,\n\
		uiSlider4Name : exprCl.uiSlider4Name,\n\
		uiPointGrpVisible : exprCl.uiPointGrpVisible,\n\
		uiPointGrpName : exprCl.uiPointGrpName,\n\
		uiPoint1Visible : exprCl.uiPoint1Visible,\n\
		uiPoint1Name : exprCl.uiPoint1Name,\n\
		uiPoint2Visible : exprCl.uiPoint2Visible,\n\
		uiPoint2Name : exprCl.uiPoint2Name,\n\
		uiColorGrpVisible : exprCl.uiColorGrpVisible,\n\
		uiColorGrpName :  exprCl.uiColorGrpName,\n\
		uiColor1Visible: exprCl.uiColor1Visible,\n\
		uiColor1Name   : exprCl.uiColor1Name,\n\
		uiColor2Visible: exprCl.uiColor2Visible,\n\
		uiColor2Name   : exprCl.uiColor2Name,\n\
		extLGrpVisible : exprCl.extLGrpVisible,\n\
		extLGrpName: exprCl.extLGrpName\n\
        };\n\
        return ExprObj;\n\
}";

std::string json_saveAsJson = "function saveAsJson(exprCl, pluginVersion){\n\
    ExprObj = createJson(exprCl, pluginVersion,false);\n\
    var presetFile =File.saveDialog('save your preset as a json');\n\
    if (presetFile && presetFile.open('w')){\n\
        presetFile.encoding ='UTF-8';\n\
        presetFile.write(JSON.stringify(ExprObj, undefined, '\\r\\n'));\n\
        presetFile.close();\n\
    }\n\
};";

std::string json_readJson ="function readJson(pluginVersion){\n\
    var ExprObj ={};\n\
    var loadFile =File.openDialog('load your preset json');\n\
    if (loadFile && loadFile.open('r')){\n\
        loadFile.encoding ='UTF-8';\n\
        var jsonFile = loadFile.read();\n\
        var testObj = JSON.parse(jsonFile);\n\
        try{\n\
            if (testObj.effectName === \"tlMath\" && testObj.minimalPluginVersion <=pluginVersion){\n\
                ExprObj.pluginVesion = testObj.pluginVesion;\n\
                ExprObj.minimalPluginVersion = testObj.minimalPluginVersion;\n\
                ExprObj.parserModeB  = parseBoolToInt (testObj.parserModeB);\n\
                ExprObj.category = testObj.category;\n\
				if (testObj.glslExpr){ExprObj.glslExpr = testObj.glslExpr;}\n\
				if (testObj.evalglslExp){ExprObj.evalglslExp = testObj.evalglslExp}\n\
				else {ExprObj.evalglslExp = 'NONE'}\n\
                if (testObj.presetName) {ExprObj.presetName  = testObj.presetName;} \n\
                if (testObj.description){ExprObj.description = testObj.description;}\n\
                if (testObj.redExpr){ExprObj.redExpr     = testObj.redExpr;}\n\
                if (testObj.greenExpr){ExprObj.greenExpr   = testObj.greenExpr;}\n\
                if (testObj.blueExpr){ExprObj.blueExpr    = testObj.blueExpr;}\n\
                if (testObj.alphaExpr){ExprObj.alphaExpr   = testObj.alphaExpr;}\n\
				if (testObj.uiSliderGrpVisible) {ExprObj.uiSliderGrpVisible = testObj.uiSliderGrpVisible}\n\
				if (testObj.uiSliderGrpName){ExprObj.uiSliderGrpName = testObj.uiSliderGrpName}\n\
				if (testObj.uiSlider1Visible){ExprObj.uiSlider1Visible= testObj.uiSlider1Visible}\n\
				if (testObj.uiSlider1Name){ExprObj.uiSlider1Name= testObj.uiSlider1Name}\n\
				if (testObj.uiSlider2Visible){ExprObj.uiSlider2Visible= testObj.uiSlider2Visible}\n\
				if (testObj.uiSlider2Name){ExprObj.uiSlider2Name= testObj.uiSlider2Name}\n\
				if (testObj.uiSlider3Visible){ExprObj.uiSlider3Visible= testObj.uiSlider3Visible}\n\
				if (testObj.uiSlider3Name){ExprObj.uiSlider3Name= testObj.uiSlider3Name}\n\
				if (testObj.uiSlider4Visible){ExprObj.uiSlider4Visible= testObj.uiSlider4Visible}\n\
				if (testObj.uiSlider4Name){ExprObj.uiSlider4Name= testObj.uiSlider4Name}\n\
				if (testObj.uiPointGrpName){ExprObj.uiPointGrpName= testObj.uiPointGrpName}\n\
				if (testObj.uiPointGrpName){ExprObj.uiPointGrpName= testObj.uiPointGrpName}\n\
				if (testObj.uiPoint1Visible){ExprObj.uiPoint1Visible= testObj.uiPoint1Visible}\n\
				if (testObj.uiPoint1Name){ExprObj.uiPoint1Name= testObj.uiPoint1Name}\n\
				if (testObj.uiPoint2Visible){ExprObj.uiPoint2Visible= testObj.uiPoint2Visible}\n\
				if (testObj.uiPoint2Name){ExprObj.uiPoint2Name= testObj.uiPoint2Name}\n\
				if (testObj.uiColorGrpVisible){ExprObj.uiColorGrpVisible= testObj.uiColorGrpVisible}\n\
				if (testObj.uiColorGrpName){ExprObj.uiColorGrpName= testObj.uiColorGrpName}\n\
				if (testObj.uiColor1Visible){ExprObj.uiColor1Visible= testObj.uiColor1Visible}\n\
				if (testObj.uiColor1Name){ExprObj.uiColor1Name= testObj.uiColor1Name}\n\
				if (testObj.uiColor2Visible){ExprObj.uiColor2Visible= testObj.uiColor2Visible}\n\
				if (testObj.uiColor2Name){ExprObj.uiColor2Name= testObj.uiColor2Name}\n\
				if (testObj.extLGrpVisible){ExprObj.extLGrpVisible= testObj.extLGrpVisible}\n\
				if (testObj.extLGrpName){ExprObj.extLGrpName= testObj.extLGrpName}\n\
                ExprObj.error       = \"none\";\n\
                }\n\
            else {\n\
                alert (\"You must use plugin version \"+ testObj.minimalPluginVersion+ \" or higher\");\n\
                ExprObj.error = \"err\";\n\
                }\n\
        }catch (e) {\n\
            alert(e)\n\
            ExprObj.error = \"err\";\n\
            }\n\
        loadFile.close();\n\
        }\n\
    return ExprObj;\n\
};";

std::string script_updateNumOfLines = "function updateNumOfLines(textEditorStr) { \n\
    var numLines =1;\n\
	var newStr = '';\n\
	var splitStr = textEditorStr.split(/\\r\\n|\\n/);\n\
	for (var i=0; i<splitStr.length; i++)\n\
	{\n\
		newStr += numLines.toString() +'  -'+splitStr[i] +'\\n';\n\
		numLines +=1;\n\
	}\n\
	return newStr;\n\
}";

std::string script_ui = "function exprScript( jsonInput, pluginMAJORV, pluginMINORV, pluginBUGV){ \n\
var pluginVersion = pluginMAJORV+'.'+pluginMINORV+pluginBUGV;\n\
pluginVersion = parseFloat (pluginVersion); \n\
var exprCl;\n\
try {\n\
	 exprCl= JSON.parse(jsonInput);\n\
}catch(e) { alert(e) }\n\
var w = new Window('dialog', 'Maths Expressions V'+pluginVersion, undefined, {resizeable:true} );\n\
w.grp = w.add('group');\n\
w.grp.orientation='column';\n\
w.grp.alignment = ['fill', 'fill'];\n\
w.grp.alignChildren = ['fill', 'fill'];\n\
w.grp.btnGrp = w.grp.add('Group');\n\
w.grp.btnGrp.orientation = 'row';\n\
w.grp.btnGrp.alignChildren = ['fill', 'fill'];\n\
w.grp.btnGrp.alignChildren = ['left', 'fill'];\n\
w.grp.btnGrp.Ok =w.grp.btnGrp.add ('button', undefined, 'Apply');\n\
w.grp.btnGrp.Cancel =w.grp.btnGrp.add ('button', undefined, 'Cancel');\n\
w.grp.btnGrp.loadBtn = w.grp.btnGrp.add ('button', undefined, 'Load Preset');\n\
w.grp.btnGrp.saveBtn =w.grp.btnGrp.add('button', undefined, 'Save Preset');\n\
\n\
//PARSER MODE\n\
w.grp.parserModeB = w.grp.add('group');\n\
w.grp.parserModeB.orientation = 'row';\n\
w.grp.parserModeB.alignChildren = ['fill', 'fill'];\n\
w.grp.parserModeB.alignChildren = ['left', 'fill'];\n\
w.grp.parserModeB.st =w.grp.parserModeB.add ('statictext', undefined, 'Parser Mode');\n\
w.grp.parserModeB.ddl = w.grp.parserModeB.add ('dropdownlist', undefined, ['Math Expressions','Glsl'])\n\
w.grp.parserModeB.ddl.selection =  exprCl.parserModeB;\n\
// \n\
//PRESET NAME\n\
w.grp.PresetN = w.grp.add('group');\n\
w.grp.PresetN.orientation = 'row';\n\
w.grp.PresetN.alignChildren = ['fill', 'fill'];\n\
w.grp.PresetN.alignChildren = ['left', 'fill'];\n\
w.grp.PresetN.stN =w.grp.PresetN.add ('statictext', undefined, 'Preset Name');\n\
w.grp.PresetN.name = w.grp.PresetN.add ('edittext', undefined, exprCl.presetName, {characters :30});\n\
// \n\
w.grp.tab = w.grp.add('tabbedpanel');\n\
//Mode expr \n\
w.grp.tab.expr = w.grp.tab.add('tab', undefined, 'Math Expressions');\n\
w.grp.tab.glsl= w.grp.tab.add('tab', undefined, 'Glsl');\n\
w.grp.tab.paramUI= w.grp.tab.add('tab', undefined, 'UI Settings');\n\
w.grp.tab.selection = w.grp.parserModeB.ddl.selection.index;\n\
 //EXPR TAB \n\
    w.grp.tab.expr.orientation='column';\n\
    w.grp.tab.expr.alignment = ['fill', 'center'];\n\
    w.grp.tab.expr.redst = w.grp.tab.expr.add ('statictext', undefined,'Red Channel Expression : ');\n\
    w.grp.tab.expr.redC = w.grp.tab.expr.add('group');\n\
    w.grp.tab.expr.redC.orientation = 'row';\n\
    w.grp.tab.expr.redC.alignment = ['fill', 'fill'];\n\
    w.grp.tab.expr.redC.alignChildren = ['fill', 'fill'];\n\
    w.grp.tab.expr.redC.redet = w.grp.tab.expr.redC.add ('edittext', undefined, exprCl.redExpr,{multiline:true});\n\
    w.grp.tab.expr.greenst = w.grp.tab.expr.add ('statictext', undefined,'Green Channel Expression :');\n\
    w.grp.tab.expr.greenC = w.grp.tab.expr.add('group');\n\
    w.grp.tab.expr.greenC.orientation = 'row';\n\
	w.grp.tab.expr.greenC.alignment = ['fill', 'fill'];\n\
    w.grp.tab.expr.greenC.alignChildren = ['fill', 'fill'];\n\
    w.grp.tab.expr.greenC.greenet = w.grp.tab.expr.greenC.add ('edittext', undefined, exprCl.greenExpr,{multiline:true});\n\
    w.grp.tab.expr.bluest = w.grp.tab.expr.add('statictext', undefined, 'Blue Channel Expression :'); \n\
    w.grp.tab.expr.blueC = w.grp.tab.expr.add('group');\n\
    w.grp.tab.expr.blueC.orientation = 'row';\n\
	w.grp.tab.expr.blueC.alignment = ['fill', 'fill'];\n\
    w.grp.tab.expr.blueC.alignChildren = ['fill', 'fill'];\n\
    w.grp.tab.expr.blueC.blueet = w.grp.tab.expr.blueC.add ('edittext', undefined, exprCl.blueExpr,{multiline:true});\n\
    w.grp.tab.expr.alphast = w.grp.tab.expr.add ('statictext', undefined, 'Alpha Channel Expression :');\n\
    w.grp.tab.expr.alphaC = w.grp.tab.expr.add('group');\n\
    w.grp.tab.expr.alphaC.orientation = 'row';\n\
	w.grp.tab.expr.alphaC.alignment = ['fill', 'fill'];\n\
    w.grp.tab.expr.alphaC.alignChildren = ['fill', 'fill'];\n\
    w.grp.tab.expr.alphaC.alphaet = w.grp.tab.expr.alphaC.add ('edittext', undefined, exprCl.alphaExpr,{multiline:true});\n\
    // \n\
    \n\
\n\
// GLSL TAB \n\
    w.grp.tab.glsl.orientation='column';\n\
    w.grp.tab.glsl.alignment = ['fill', 'center'];\n\
    w.grp.tab.glsl.fragShst = w.grp.tab.glsl.add ('statictext', undefined,'GLSL Fragment Shader : ');\n\
	w.grp.tab.glsl.btnGrp =w.grp.tab.glsl.add ('group');\n\
	w.grp.tab.glsl.btnGrp.orientation='row';\n\
	w.grp.tab.glsl.btnGrp.fragShevalbtn = w.grp.tab.glsl.btnGrp.add ('button', undefined, 'Evaluate Shader');\n\
	w.grp.tab.glsl.btnGrp.fragShShpwBtn =  w.grp.tab.glsl.btnGrp.add ('button', undefined, 'Preview Code');\n\
    w.grp.tab.glsl.fragSh = w.grp.tab.glsl.add('group');\n\
    w.grp.tab.glsl.fragSh.orientation = 'column';\n\
    w.grp.tab.glsl.fragSh.alignment = ['fill', 'center'];\n\
	w.grp.tab.glsl.fragSh.alignChildren = ['fill', 'fill'];\n\
	w.grp.tab.glsl.fragSh.fragShet = w.grp.tab.glsl.fragSh.add('edittext', undefined, exprCl.glslExpr, { multiline:true }); \n\
	w.grp.tab.glsl.fragSh.fragShet.font = ScriptUI.newFont('Arial', 'Regular', 50); \n\
	w.grp.tab.glsl.fragSh.glslconsole = w.grp.tab.glsl.fragSh.add ('edittext', undefined, 'glsl console ' +'\\r'+exprCl.evalglslExp ,{readonly:true, multiline:true});\n\
// \n\
"
"//tab UI\n\
	w.grp.tab.paramUI.orientation = 'column';\n\
	w.grp.tab.paramUI.paramSt = w.grp.tab.paramUI.add ('statictext', undefined, 'Define Parameters Name');\n\
	w.grp.tab.paramUI.param = w.grp.tab.paramUI.add('group');\n\
	w.grp.tab.paramUI.param.orientation = 'row';\n\
	w.grp.tab.paramUI.param.paramslider = w.grp.tab.paramUI.param.add('group');\n\
	w.grp.tab.paramUI.param.paramslider.orientation = 'column';\n\
	w.grp.tab.paramUI.param.paramslider.grpName =w.grp.tab.paramUI.param.paramslider.add('group');\n\
	w.grp.tab.paramUI.param.paramslider.grpName.orientation = 'row';\n\
	w.grp.tab.paramUI.param.paramslider.grpName.originalNameSt = w.grp.tab.paramUI.param.paramslider.grpName.add ('statictext', undefined, 'Slider group :');\n\
	w.grp.tab.paramUI.param.paramslider.grpName.cbvisible = w.grp.tab.paramUI.param.paramslider.grpName.add ('checkbox', undefined, 'visible');\n\
	w.grp.tab.paramUI.param.paramslider.grpName.cbvisible.value = exprCl.uiSliderGrpVisible;\n\
	w.grp.tab.paramUI.param.paramslider.grpName.editNameet = w.grp.tab.paramUI.param.paramslider.grpName.add('edittext', undefined,exprCl.uiSliderGrpName, { characters: 30 }); \n\
	w.grp.tab.paramUI.param.paramslider.paramSlider1 = w.grp.tab.paramUI.param.paramslider.add('group'); \n\
	w.grp.tab.paramUI.param.paramslider.paramSlider1.orientation = 'row'; \n\
	w.grp.tab.paramUI.param.paramslider.paramSlider1.originalNameSt = w.grp.tab.paramUI.param.paramslider.paramSlider1.add('statictext', undefined, 'slider 1 :'); \n\
	w.grp.tab.paramUI.param.paramslider.paramSlider1.cbvisible = w.grp.tab.paramUI.param.paramslider.paramSlider1.add('checkbox', undefined, 'visible'); \n\
	w.grp.tab.paramUI.param.paramslider.paramSlider1.cbvisible.value =	exprCl.uiSlider1Visible;\n\
	w.grp.tab.paramUI.param.paramslider.paramSlider1.editNameet = w.grp.tab.paramUI.param.paramslider.paramSlider1.add ('edittext', undefined, exprCl.uiSlider1Name, { characters: 30 });\n\
	w.grp.tab.paramUI.param.paramslider.paramSlider2 = w.grp.tab.paramUI.param.paramslider.add('group');\n\
    w.grp.tab.paramUI.param.paramslider.paramSlider2.orientation = 'row';\n\
	w.grp.tab.paramUI.param.paramslider.paramSlider2.originalNameSt = w.grp.tab.paramUI.param.paramslider.paramSlider2.add ('statictext', undefined, 'slider 2 :');\n\
	w.grp.tab.paramUI.param.paramslider.paramSlider2.cbvisible = w.grp.tab.paramUI.param.paramslider.paramSlider2.add ('checkbox', undefined, 'visible');\n\
	w.grp.tab.paramUI.param.paramslider.paramSlider2.cbvisible.value = exprCl.uiSlider2Visible;\n\
	w.grp.tab.paramUI.param.paramslider.paramSlider2.editNameet = w.grp.tab.paramUI.param.paramslider.paramSlider2.add ('edittext', undefined, exprCl.uiSlider2Name,{characters: 30});\n\
	w.grp.tab.paramUI.param.paramslider.paramSlider3 = w.grp.tab.paramUI.param.paramslider.add('group');\n\
    w.grp.tab.paramUI.param.paramslider.paramSlider3.orientation = 'row';\n\
	w.grp.tab.paramUI.param.paramslider.paramSlider3.originalNameSt = w.grp.tab.paramUI.param.paramslider.paramSlider3.add ('statictext', undefined, 'slider 3 :');\n\
	w.grp.tab.paramUI.param.paramslider.paramSlider3.cbvisible = w.grp.tab.paramUI.param.paramslider.paramSlider3.add ('checkbox', undefined, 'visible');\n\
	w.grp.tab.paramUI.param.paramslider.paramSlider3.cbvisible.value = exprCl.uiSlider3Visible; \n\
	w.grp.tab.paramUI.param.paramslider.paramSlider3.editNameet = w.grp.tab.paramUI.param.paramslider.paramSlider3.add ('edittext', undefined, exprCl.uiSlider3Name,{characters: 30});\n\
	w.grp.tab.paramUI.param.paramslider.paramSlider4 = w.grp.tab.paramUI.param.paramslider.add('group');\n\
    w.grp.tab.paramUI.param.paramslider.paramSlider4.orientation = 'row';\n\
	w.grp.tab.paramUI.param.paramslider.paramSlider4.originalNameSt = w.grp.tab.paramUI.param.paramslider.paramSlider4.add ('statictext', undefined, 'slider 4 :');\n\
	w.grp.tab.paramUI.param.paramslider.paramSlider4.cbvisible = w.grp.tab.paramUI.param.paramslider.paramSlider4.add ('checkbox', undefined, 'visible');\n\
	w.grp.tab.paramUI.param.paramslider.paramSlider4.cbvisible.value = exprCl.uiSlider4Visible;\n\
	w.grp.tab.paramUI.param.paramslider.paramSlider4.editNameet = w.grp.tab.paramUI.param.paramslider.paramSlider4.add('edittext', undefined, exprCl.uiSlider4Name, { characters: 30 }); \n\
	w.grp.tab.paramUI.param.parampoint = w.grp.tab.paramUI.param.add('group'); \n\
	w.grp.tab.paramUI.param.parampoint.orientation = 'column'; \n\
	w.grp.tab.paramUI.param.parampoint.grpName = w.grp.tab.paramUI.param.parampoint.add('group'); \n\
	w.grp.tab.paramUI.param.parampoint.grpName.orientation = 'row'; \n\
	w.grp.tab.paramUI.param.parampoint.grpName.originalNameSt = w.grp.tab.paramUI.param.parampoint.grpName.add('statictext', undefined, '2d points group :'); \n\
	w.grp.tab.paramUI.param.parampoint.grpName.cbvisible = w.grp.tab.paramUI.param.parampoint.grpName.add('checkbox', undefined, 'visible'); \n\
	w.grp.tab.paramUI.param.parampoint.grpName.cbvisible.value = exprCl.uiPointGrpVisible;\n\
	w.grp.tab.paramUI.param.parampoint.grpName.editNameet = w.grp.tab.paramUI.param.parampoint.grpName.add('edittext', undefined,exprCl.uiPointGrpName, { characters: 30 }); \n\
	w.grp.tab.paramUI.param.parampoint.parampoint1 = w.grp.tab.paramUI.param.parampoint.add('group'); \n\
	w.grp.tab.paramUI.param.parampoint.parampoint1.orientation = 'row'; \n\
	w.grp.tab.paramUI.param.parampoint.parampoint1.originalNameSt = w.grp.tab.paramUI.param.parampoint.parampoint1.add('statictext', undefined, 'point 1 :'); \n\
	w.grp.tab.paramUI.param.parampoint.parampoint1.cbvisible = w.grp.tab.paramUI.param.parampoint.parampoint1.add('checkbox', undefined, 'visible'); \n\
	w.grp.tab.paramUI.param.parampoint.parampoint1.cbvisible.value = exprCl.uiPoint1Visible;\n\
	w.grp.tab.paramUI.param.parampoint.parampoint1.editNameet = w.grp.tab.paramUI.param.parampoint.parampoint1.add('edittext', undefined, exprCl.uiPoint1Name, { characters: 30 }); \n\
	w.grp.tab.paramUI.param.parampoint.parampoint2 = w.grp.tab.paramUI.param.parampoint.add('group'); \n\
	w.grp.tab.paramUI.param.parampoint.parampoint2.orientation = 'row'; \n\
	w.grp.tab.paramUI.param.parampoint.parampoint2.originalNameSt = w.grp.tab.paramUI.param.parampoint.parampoint2.add('statictext', undefined, 'point 2 :'); \n\
	w.grp.tab.paramUI.param.parampoint.parampoint2.cbvisible = w.grp.tab.paramUI.param.parampoint.parampoint2.add('checkbox', undefined, 'visible'); \n\
	w.grp.tab.paramUI.param.parampoint.parampoint2.cbvisible.value=	exprCl.uiPoint2Visible;\n\
	w.grp.tab.paramUI.param.parampoint.parampoint2.editNameet = w.grp.tab.paramUI.param.parampoint.parampoint2.add('edittext', undefined, exprCl.uiPoint2Name, { characters: 30 }); \n\
	w.grp.tab.paramUI.param.paramcolor = w.grp.tab.paramUI.param.add('group'); \n\
	w.grp.tab.paramUI.param.paramcolor.orientation = 'column'; \n\
	w.grp.tab.paramUI.param.paramcolor.grpName = w.grp.tab.paramUI.param.paramcolor.add('group'); \n\
	w.grp.tab.paramUI.param.paramcolor.grpName.orientation = 'row'; \n\
	w.grp.tab.paramUI.param.paramcolor.grpName.originalNameSt = w.grp.tab.paramUI.param.paramcolor.grpName.add('statictext', undefined, 'colors group :'); \n\
	w.grp.tab.paramUI.param.paramcolor.grpName.cbvisible = w.grp.tab.paramUI.param.paramcolor.grpName.add('checkbox', undefined, 'visible'); \n\
	w.grp.tab.paramUI.param.paramcolor.grpName.cbvisible.value = exprCl.uiColorGrpVisible\n\
	w.grp.tab.paramUI.param.paramcolor.grpName.editNameet = w.grp.tab.paramUI.param.paramcolor.grpName.add('edittext', undefined, exprCl.uiColorGrpName , { characters: 30 }); \n\
	w.grp.tab.paramUI.param.paramcolor.paramcolor1 = w.grp.tab.paramUI.param.paramcolor.add('group'); \n\
	w.grp.tab.paramUI.param.paramcolor.paramcolor1.orientation = 'row'; \n\
	w.grp.tab.paramUI.param.paramcolor.paramcolor1.originalNameSt = w.grp.tab.paramUI.param.paramcolor.paramcolor1.add('statictext', undefined, 'color 1 :'); \n\
	w.grp.tab.paramUI.param.paramcolor.paramcolor1.cbvisible = w.grp.tab.paramUI.param.paramcolor.paramcolor1.add('checkbox', undefined, 'visible'); \n\
	w.grp.tab.paramUI.param.paramcolor.paramcolor1.cbvisible.value = exprCl.uiColor1Visible;\n\
	w.grp.tab.paramUI.param.paramcolor.paramcolor1.editNameet = w.grp.tab.paramUI.param.paramcolor.paramcolor1.add('edittext', undefined, exprCl.uiColor1Name, { characters: 30 }); \n\
	w.grp.tab.paramUI.param.paramcolor.paramcolor2 = w.grp.tab.paramUI.param.paramcolor.add('group'); \n\
	w.grp.tab.paramUI.param.paramcolor.paramcolor2.orientation = 'row'; \n\
	w.grp.tab.paramUI.param.paramcolor.paramcolor2.originalNameSt = w.grp.tab.paramUI.param.paramcolor.paramcolor2.add('statictext', undefined, 'color 2 :'); \n\
	w.grp.tab.paramUI.param.paramcolor.paramcolor2.cbvisible = w.grp.tab.paramUI.param.paramcolor.paramcolor2.add('checkbox', undefined, 'visible'); \n\
	w.grp.tab.paramUI.param.paramcolor.paramcolor2.cbvisible.value = exprCl.uiColor2Visible;\n\
	w.grp.tab.paramUI.param.paramcolor.paramcolor2.editNameet = w.grp.tab.paramUI.param.paramcolor.paramcolor2.add('edittext', undefined, exprCl.uiColor2Name, { characters: 30 }); \n\
	w.grp.tab.paramUI.param.paramextL = w.grp.tab.paramUI.param.add('group'); \n\
	w.grp.tab.paramUI.param.paramextL.orientation = 'column';\n\
	w.grp.tab.paramUI.param.paramextL.grpName = w.grp.tab.paramUI.param.paramextL.add('group'); \n\
	w.grp.tab.paramUI.param.paramextL.grpName.originalNameSt = w.grp.tab.paramUI.param.paramextL.grpName.add('statictext', undefined, 'external Layer group :'); \n\
	w.grp.tab.paramUI.param.paramextL.grpName.cbvisible = w.grp.tab.paramUI.param.paramextL.grpName.add('checkbox', undefined, 'visible'); \n\
	w.grp.tab.paramUI.param.paramextL.grpName.cbvisible.value = exprCl.extLGrpVisible;\n\
	w.grp.tab.paramUI.param.paramextL.grpName.editNameet = w.grp.tab.paramUI.param.paramextL.grpName.add('edittext', undefined, exprCl.extLGrpName,{ characters: 30 }); \n\
	w.grp.tab.paramUI.descriptionGrp = w.grp.tab.paramUI.add('group');\n\
	w.grp.tab.paramUI.descriptionGrp.orientation = 'row';\n\
	w.grp.tab.paramUI.descriptionGrp.alignment = ['fill', 'fill'];\n\
	w.grp.tab.paramUI.descriptionGrp.descrst = w.grp.tab.paramUI.descriptionGrp.add ('statictext', undefined, 'Description:');\n\
	w.grp.tab.paramUI.descriptionGrp.description = w.grp.tab.paramUI.descriptionGrp.add ('edittext', undefined, exprCl.description,{multiline:true});\n\
"
"//CALLBACK \n\
\n\
var tempRet =getcallBacks (w);\n\
var result_temp = createJson(tempRet, pluginVersion,false);\n\
var result = JSON.stringify(result_temp);\n\
w.grp.parserModeB.ddl.onChange = function (){ \n\
	if (w.grp.parserModeB.ddl.selection == 0){\n\
		w.grp.tab.selection = 0;\n\
		}\n\
	else{\n\
		w.grp.tab.selection =1;\n\
		}\n\
	}\n\
w.grp.btnGrp.loadBtn.onClick = function (){\n\
    var exprObj = readJson(pluginVersion);\n\
    if (exprObj.error === \"none\"){\n\
		w.grp.parserModeB.ddl.selection =  parseInt(exprObj.parserModeB); \n\
		w.grp.tab.glsl.fragSh.fragShet.text = exprObj.glslExpr;\n\
        w.grp.tab.expr.redC.redet.text      = exprObj.redExpr;\n\
        w.grp.tab.expr.greenC.greenet.text  = exprObj.greenExpr;\n\
        w.grp.tab.expr.blueC.blueet.text    = exprObj.blueExpr;\n\
        w.grp.tab.expr.alphaC.alphaet.text  =exprObj.alphaExpr;\n\
        w.grp.PresetN.name.text= exprObj.presetName; \n\
		w.grp.tab.paramUI.param.paramslider.grpName.cbvisible= exprObj.uiSliderGrpVisible;\n\
		w.grp.tab.paramUI.param.paramslider.grpName.editNameet.text= exprObj.uiSliderGrpName;\n\
		w.grp.tab.paramUI.param.paramslider.paramSlider1.cbvisible.value= exprObj.uiSlider1Visible; \n\
		w.grp.tab.paramUI.param.paramslider.paramSlider1.editNameet.text = exprObj.uiSlider1Name; \n\
		w.grp.tab.paramUI.param.paramslider.paramSlider2.cbvisible.value = exprObj.uiSlider2Visible; \n\
		w.grp.tab.paramUI.param.paramslider.paramSlider2.editNameet.text = exprObj.uiSlider2Name; \n\
		w.grp.tab.paramUI.param.paramslider.paramSlider3.cbvisible.value = exprObj.uiSlider3Visible; \n\
		w.grp.tab.paramUI.param.paramslider.paramSlider3.editNameet.text = exprObj.uiSlider3Name; \n\
		w.grp.tab.paramUI.param.paramslider.paramSlider4.cbvisible.value = exprObj.uiSlider4Visible; \n\
		w.grp.tab.paramUI.param.paramslider.paramSlider4.editNameet.text = exprObj.uiSlider4Name; \n\
		w.grp.tab.paramUI.param.parampoint.grpName.cbvisible.value = exprObj.uiPointGrpVisible; \n\
		w.grp.tab.paramUI.param.parampoint.grpName.editNameet.text = exprObj.uiPointGrpName; \n\
		w.grp.tab.paramUI.param.parampoint.parampoint1.cbvisible.value = exprObj.uiPoint1Visible; \n\
		w.grp.tab.paramUI.param.parampoint.parampoint1.editNameet.text = exprObj.uiPoint1Name; \n\
		w.grp.tab.paramUI.param.parampoint.parampoint2.cbvisible.value = exprObj.uiPoint2Visible; \n\
		w.grp.tab.paramUI.param.parampoint.parampoint2.editNameet.text = exprObj.uiPoint2Name; \n\
		w.grp.tab.paramUI.param.paramcolor.grpName.cbvisible.value = exprObj.uiColorGrpVisible;  \n\
		w.grp.tab.paramUI.param.paramcolor.grpName.editNameet.text = exprObj.uiColorGrpName;  \n\
		w.grp.tab.paramUI.param.paramcolor.paramcolor1.cbvisible.value =exprObj.uiColor1Visible;"
		"w.grp.tab.paramUI.param.paramcolor.paramcolor1.editNameet.text=exprObj.uiColor1Name;\n\
		w.grp.tab.paramUI.param.paramcolor.paramcolor2.cbvisible.value=exprObj.uiColor2Visible;\n\
	    w.grp.tab.paramUI.param.paramcolor.paramcolor2.editNameet.text =exprObj.uiColor2Name;\n\
		w.grp.tab.paramUI.param.paramextL.grpName.cbvisible.value = exprObj.extLGrpVisible;\n\
		w.grp.tab.paramUI.param.paramextL.grpName.editNameet.text = exprObj.extLGrpName;\n\
        w.grp.tab.paramUI.param.descriptionGrp.description.text   = exprObj.description; \n\
		}\n\
    else { \n\
        alert (exprObj.error);\n\
		}\n\
    }\n\
\n\
w.grp.btnGrp.saveBtn.onClick = function (){\n\
    var exprRet =getcallBacks (w);\n\
    saveAsJson (exprRet, pluginVersion);\n\
    }\n\
w.grp.tab.glsl.btnGrp.fragShevalbtn.onClick = function(){\n\
	var exprRet = getcallBacks(w);\n\
	 var strExpr =createJson(exprRet, pluginVersion,true);\n\
	 w.close();\n\
	result =JSON.stringify(strExpr);\n\
}\n\
w.grp.tab.glsl.btnGrp.fragShShpwBtn.onClick = function(){\n\
    alert(updateNumOfLines (w.grp.tab.glsl.fragSh.fragShet.text));\n\
}\n\
w.grp.btnGrp.Ok.onClick = function(){\n\
    var exprRet = getcallBacks(w);\n\
    var strExpr =createJson(exprRet, pluginVersion,false);\n\
	w.close();\n\
	result =JSON.stringify(strExpr);\n\
	}\n\
w.grp.btnGrp.Cancel.onClick = function(){\n\
    var exprRet = getcallBacks (w);\n\
    var ret = createJson(exprRet, pluginVersion,false);\n\
	w.close();\n\
	result = JSON.stringify(ret);\n\
	}\n\
w.onResizing = w.onResize = function(){this.layout.resize();}\n\
w.show();\n\
return result\n\
}\n\
\n\
try{\n\
	exprScript(%s, %s, %s, %s); \n\
	}catch (e){alert(e);}\n\
";

std::string script_ae = script_ui.append(script_parseBoolToInt)
								 .append(script_parseIntToBool)
								 .append(script_getcallBacks)
								 .append(json_createJson)
								 .append(json_saveAsJson)
								 .append(json_readJson)
								 .append(script_updateNumOfLines);


#endif /* scipt_h */
