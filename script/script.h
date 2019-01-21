//
//  scipt.h
//  tlMath
//
//
#include <iostream>
#include <string>
#ifndef scipt_h
#define scipt_h


std::string script_getcallBacks = "function getcallBacks(w){\n\
    var exprCb = {};\n\
    exprCb.parserModeB   =   parseInt(w.grp.parserModeB.ddl.selection.index); \n\
    exprCb.presetName   =   w.grp.PresetN.name.text; \n\
    exprCb.description  =   w.grp.tab.paramUI.descriptionGrp.description.text; \n\
    exprCb.redExpr      =   w.grp.tab.expr.redC.redet.text;\n\
    exprCb.greenExpr    =   w.grp.tab.expr.greenC.greenet.text;\n\
    exprCb.blueExpr     =   w.grp.tab.expr.blueC.blueet.text;\n\
    exprCb.alphaExpr    =   w.grp.tab.expr.alphaC.alphaet.text;\n\
    exprcB.funcModeB =  w.grp.tab.func.cbMMode.value;\n\
    exprCb.func1Str     =   w.grp.tab.func.func1C.funcet.text;\n\
    exprCb.func2Str     =   w.grp.tab.func.func2C.funcet.text;\n\
    exprCb.func3Str     =   w.grp.tab.func.func3C.funcet.text;\n\
    exprCb.glslExpr     =   w.grp.tab.glsl.fragSh.fragShet.text;\n\
    return exprCb ;\n\
}";

std::string json_createJson = "function createJson(exprCl, pluginVersion){\n\
    ExprObj = {\n\
        effectName   : \"tlMath\",\n\
        parserModeB : exprCl.parserModeB, \n\
        category :  \"Custom\",\n\
        presetName  : exprCl.presetName,\n\
        description : exprCl.description, \n\
        pluginVesion : \"+ pluginVersion +\",\n\
        minimalPluginVersion : \"1.13\",\n\
        glslExpr : exprCl.glslExpr,\n\
        redExpr   : exprCl.redExpr,\n\
        greenExpr : exprCl.greenExpr,\n\
        blueExpr  : exprCl.blueExpr,\n\
        alphaExpr : exprCl.alphaExpr,\n\
        funcModeB : exprCl.functionModeB,\n\
        func1Str   : exprCl.func1Str,\n\
        func2Str   : exprCl.func2Str,\n\
        func3Str   : exprCl.func3Str\n\
        };\n\
        return ExprObj;\n\
}";

std::string json_saveAsJson = "function saveAsJson(exprCl, pluginVersion){\n\
    ExprObj = createJson(exprCl, pluginVersion);\n\
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
                ExprObj.parserModeB  = testObj.parserModeB;\n\
                ExprObj.category = testObj.category;\n\
                ExprObj.glslExpr    = testObj.glslExpr;\n\
                ExprObj.presetName  = testObj.presetName; \n\
                ExprObj.description = testObj.description;\n\
                ExprObj.redExpr     = testObj.redExpr;\n\
                ExprObj.greenExpr   = testObj.greenExpr;\n\
                ExprObj.blueExpr    = testObj.blueExpr;\n\
                ExprObj.alphaExpr   = testObj.alphaExpr;\n\
                ExprObj.funcModeB   = testObj.functionModeB;\n\
                ExprObj.func1Str    = testObj.func1Str;\n\
                ExprObj.func2Str    = testObj.func2Str;\n\
                ExprObj.func3Str    = testObj.func3Str;\n\
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

std::string script_ui = "function exprScript( jsonInput, pluginMAJORV, pluginMINORV, pluginBUGV){ \n\
var pluginVersion = pluginMAJORV+'.'+pluginMINORV+pluginBUGV;\n\
pluginVersion = parseFloat (pluginVersion); \n\
var exprCl = JSON.parse(jsonInput);\n\
var w = new Window('dialog', 'Maths Expressions V'+pluginVersion, undefined, {resizeable:true} );\n\
w.sttxt= w.add ('statictext', undefined, 'Write here your math operations for each channels. Math operations are based on Mathematical Expression Toolkit Library');\n\
w.grp = w.add('group');\n\
w.grp.orientation='column';\n\
w.grp.alignment = ['fill', 'fill'];\n\
w.grp.alignChildren = ['fill', 'fill'];\n\
\n\
//PARSER MODE\n\
w.grp.parserModeB = w.grp.add('group');\n\
w.grp.parserModeB.orientation = 'row';\n\
w.grp.parserModeB.alignChildren = ['left', 'fill'];\n\
w.grp.parserModeB.st =w.grp.parserModeB.add ('statictext', undefined, 'Parser Mode');\n\
w.grp.parserModeB.ddl = w.grp.parserModeB.add ('dropdownlist', undefined, ['Math Expressions','Glsl'])\n\
w.grp.parserModeB.ddl.selection =  parseInt(exprCl.parserModeB);\n\
// \n\
//PRESET NAME\n\
w.grp.PresetN = w.grp.add('group');\n\
w.grp.PresetN.orientation = 'row';\n\
w.grp.PresetN.alignChildren = ['left', 'fill'];\n\
w.grp.PresetN.stN =w.grp.PresetN.add ('statictext', undefined, 'Preset Name');\n\
w.grp.PresetN.name = w.grp.PresetN.add ('edittext', undefined, exprCl.presetName);\n\
// \n\
w.grp.tab = w.grp.add('tabbedpanel');\n\
//Mode expr \n\
w.grp.tab.expr = w.grp.tab.add('tab', undefined, 'Math Expressions');\n\
w.grp.tab.func = w.grp.tab.add('tab', undefined, 'Math Functions');\n\
w.grp.tab.glsl= w.grp.tab.add('tab', undefined, 'Glsl');\n\
w.grp.tab.paramUI= w.grp.tab.add('tab', undefined, 'UI Settings');\n\
 //EXPR TAB \n\
    w.grp.tab.expr.orientation='column';\n\
    w.grp.tab.expr.alignment = ['fill', 'fill'];\n\
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
//FUNC TABLE TAB \n\
    w.grp.tab.func.orientation='column';\n\
    w.grp.tab.func.alignment = ['fill', 'fill'];\n\
    w.grp.tab.func.cbMMode =w.grp.tab.func.funcMode.add ('checkbox', 'Activate Functions');\n\
    w.grp.tab.func.cbMMode.value =exprCl.exprcl.funcModeB;\n\
    w.grp.tab.func.func1st = w.grp.tab.func.add ('statictext', undefined,'Function1 : ');\n\
    w.grp.tab.func.func1C = w.grp.tab.func.add('group');\n\
    w.grp.tab.func.func1C.orientation = 'row';\n\
    w.grp.tab.func.func1C.alignment = ['fill', 'fill'];\n\
    w.grp.tab.func.func1C.alignChildren = ['fill', 'fill'];\n\
    w.grp.tab.func.func1C.funcet = w.grp.tab.func.func1C.add ('edittext', undefined, exprCl.func1Str,{multiline:true});\n\
    w.grp.tab.func.func2st = w.grp.tab.func.add ('statictext', undefined,'Function2 : ');\n\
    w.grp.tab.func.func2C = w.grp.tab.func.add('group');\n\
    w.grp.tab.func.func2C.orientation = 'row';\n\
    w.grp.tab.func.func2C.alignment = ['fill', 'fill'];\n\
    w.grp.tab.func.func2C.alignChildren = ['fill', 'fill'];\n\
    w.grp.tab.func.func2C.funcet = w.grp.tab.func.func2C.add ('edittext', undefined, exprCl.func2Str,{multiline:true});\n\
    w.grp.tab.func.func3st = w.grp.tab.func.add ('statictext', undefined,'Function3 : ');\n\
    w.grp.tab.func.func3C = w.grp.tab.func.add('group');\n\
    w.grp.tab.func.func3C.orientation = 'row';\n\
    w.grp.tab.func.func3C.alignment = ['fill', 'fill'];\n\
    w.grp.tab.func.func3C.alignChildren = ['fill', 'fill'];\n\
    w.grp.tab.func.func3C.funcet = w.grp.tab.func.func3C.add ('edittext', undefined, exprCl.func3Str,{multiline:true});\n\
// \n\
\n\
// GLSL TAB \n\
    w.grp.tab.glsl.orientation='column';\n\
    w.grp.tab.glsl.alignment = ['fill', 'fill'];\n\
    w.grp.tab.glsl.fragShst = w.grp.tab.glsl.add ('statictext', undefined,'GLSL Fragment Shader : ');\n\
    w.grp.tab.glsl.fragSh = w.grp.tab.glsl.add('group');\n\
    w.grp.tab.glsl.fragSh.orientation = 'row';\n\
    w.grp.tab.glsl.fragSh.alignment = ['fill', 'fill'];\n\
    w.grp.tab.glsl.fragSh.alignChildren = ['fill', 'fill'];\n\
    w.grp.tab.glsl.fragSh.fragShet = w.grp.tab.glsl.fragSh.add ('edittext', undefined, exprCl.glslExpr,{multiline:true});\n\
// \n\
\n\
//tab UI\n\
	w.grp.tab.paramUI.descriptionGrp = w.grp.tab.paramUI.add('group');\n\
	w.grp.tab.paramUI.descriptionGrp.orientation = 'row';\n\
	w.grp.tab.paramUI.descriptionGrp.alignment = ['fill', 'fill'];\n\
	w.grp.tab.paramUI.descriptionGrp.descrst = w.grp.tab.paramUI.descriptionGrp.add ('statictext', undefined, 'Description:');\n\
	w.grp.tab.paramUI.descriptionGrp.description = w.grp.tab.paramUI.descriptionGrp.add ('edittext', undefined, exprCl.description,{multiline:true});\n\
w.grp.btnGrp = w.grp.add('Group');\n\
w.grp.btnGrp.orientation = 'row';\n\
w.grp.btnGrp.Ok =w.grp.btnGrp.add ('button', undefined, 'Apply');\n\
w.grp.btnGrp.Cancel =w.grp.btnGrp.add ('button', undefined, 'Cancel');\n\
w.grp.btnGrp.loadBtn = w.grp.btnGrp.add ('button', undefined, 'Load Preset');\n\
w.grp.btnGrp.saveBtn =w.grp.btnGrp.add('button', undefined, 'Save Preset');\n\
\n\
\n\
//CALLBACK \n\
\n\
var tempRet =getcallBacks (w);\n\
var result_temp = createJson(tempRet, pluginVersion);\n\
var result = JSON.stringify(result_temp);\n\
w.grp.btnGrp.loadBtn.onClick = function (){\n\
    var exprObj = readJson(pluginVersion);\n\
    if (exprObj.error === \"none\"){\n\
		w.grp.parserModeB.ddl.selection.index =  parseInt(exprObj.parserModeB); \n\
        w.grp.tab.func.cbMMode.value = exprcB.funcModeB;\n\
		w.grp.tab.func.func1C.funcet.text = exprObj.func1Str;\n\
		w.grp.tab.func.func2C.funcet.text = exprObj.func2Str;\n\
		w.grp.tab.func.func3C.funcet.text = exprObj.func3Str;\n\
		w.grp.tab.glsl.fragSh.fragShet.text = exprObj.glslExpr;\n\
        w.grp.tab.expr.redC.redet.text =		exprObj.redExpr;\n\
        w.grp.tab.expr.greenC.greenet.text=	exprObj.greenExpr;\n\
        w.grp.tab.expr.blueC.blueet.text =	exprObj.blueExpr;\n\
        w.grp.tab.expr.alphaC.alphaet.text=	exprObj.alphaExpr;\n\
        w.grp.PresetN.name.text= exprObj.presetName; \n\
        w.grp.tab.paramUI.descriptionGrp.description.text = exprObj.description; \n\
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
w.grp.btnGrp.Ok.onClick = function(){\n\
    var exprRet = getcallBacks (w);\n\
    var strExpr =createJson(exprRet, pluginVersion);\n\
	w.close();\n\
	result =JSON.stringify(strExpr);\n\
	}\n\
w.grp.btnGrp.Cancel.onClick = function(){\n\
    var exprRet = getcallBacks (w);\n\
    var ret = createJson(exprRet, pluginVersion);\n\
	w.close();\n\
	result = JSON.stringify(ret);\n\
	}\n\
w.onResizing = w.onResize = function(){this.layout.resize();}\n\
w.show();\n\
return result\n\
}\n\
\n\
exprScript(%s,%s,%s,%s);\n\
";

std::string script_ae = script_ui.append(script_getcallBacks).append(json_createJson).append(json_saveAsJson).append(json_readJson);

#endif /* scipt_h */
