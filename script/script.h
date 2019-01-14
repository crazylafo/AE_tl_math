//
//  scipt.h
//  tlMath
//
//
#include <iostream>
#include <string>
#ifndef scipt_h
#define scipt_h

std::string json_createJson = "function createJson(redExpr,greenExpr,blueExpr,alphaExpr, pluginVersion, presetName, description){\n\
    ExprObj = {\n\
        effectName   : \"tlMath\",\n\
        exprLang :  \"Exprtk\",\n\
        category :  \"Custom\",\n\
        pluginVesion : \"+ pluginVersion +\",\n\
        minimalPluginVersion : \"1.11\",\n\
        redExpr   : redExpr,\n\
        greenExpr : greenExpr,\n\
        blueExpr  : blueExpr,\n\
        alphaExpr : alphaExpr,\n\
        presetName  : presetName,\n\
        description : description, \n\
        };\n\
        return ExprObj;\n\
}";

std::string json_saveAsJson = "function saveAsJson(redExpr,greenExpr,blueExpr,alphaExpr, pluginVersion,presetName, description){\n\
    ExprObj = createJson(redExpr,greenExpr,blueExpr,alphaExpr, pluginVersion,presetName, description);\n\
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
                ExprObj.exprLang = testObj.exprLang;\n\
                ExprObj.category = testObj.category;\n\
                ExprObj.pluginVesion = testObj.pluginVesion;\n\
                ExprObj.minimalPluginVersion = testObj.minimalPluginVersion;\n\
                ExprObj.redExpr     = testObj.redExpr;\n\
                ExprObj.greenExpr   = testObj.greenExpr;\n\
                ExprObj.blueExpr    = testObj.blueExpr;\n\
                ExprObj.alphaExpr   = testObj.alphaExpr;\n\
                ExprObj.presetName  = testObj.presetName; \n\
                ExprObj.description = testObj.description \n\
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

std::string script_ui = "function exprScript(redExpr,greenExpr,blueExpr,alphaExpr,presetName, presetDescription, pluginMAJORV, pluginMINORV, pluginBUGV){ \n\
var pluginVersion = pluginMAJORV+'.'+pluginMINORV+pluginBUGV;\n\
pluginVersion = parseFloat (pluginVersion); \n\
var w = new Window('dialog', 'Maths Expressions V'+pluginVersion, undefined, {resizeable:true} );\n\
w.sttxt= w.add ('statictext', undefined, 'Write here your math operations for each channels. Math operations are based on Mathematical Expression Toolkit Library');\n\
w.grp = w.add('group');\n\
w.grp.orientation='row';\n\
w.grp.alignment = ['fill', 'fill'];\n\
w.grp.alignChildren = ['fill', 'fill'];\n\
w.grp.tab = w.grp.add('tabbedpanel');\n\
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
    w.grp.tab.expr.redC.redet = w.grp.tab.expr.redC.add ('edittext', undefined, redExpr,{multiline:true});\n\
    w.grp.tab.expr.greenst = w.grp.tab.expr.add ('statictext', undefined,'Green Channel Expression :');\n\
    w.grp.tab.expr.greenC = w. grp.add('group');\n\
    w.grp.tab.expr.greenC.orientation = 'row';\n\
    w.grp.tab.expr.greenC.alignChildren = ['fill', 'fill'];\n\
    w.grp.tab.expr.greenC.greenet = w.grp.tab.expr.greenC.add ('edittext', undefined, greenExpr,{multiline:true});\n\
    w.grp.tab.expr.bluest = w.grp.tab.expr.add('statictext', undefined, 'Blue Channel Expression :'); \n\
    w.grp.tab.expr.blueC = w.grp.tab.expr.add('group');\n\
    w.grp.tab.expr.blueC.orientation = 'row';\n\
    w.grp.tab.expr.blueC.alignChildren = ['fill', 'fill'];\n\
    w.grp.tab.expr.blueC.blueet = w.grp.tab.expr.blueC.add ('edittext', undefined, blueExpr,{multiline:true});\n\
    w.grp.tab.expr.alphast = w.grp.tab.expr.add ('statictext', undefined, 'Alpha Channel Expression :');\n\
    w.grp.tab.expr.alphaC = w.grp.tab.expr.add('group');\n\
    w.grp.tab.expr.alphaC.orientation = 'row';\n\
    w.grp.tab.expr.alphaC.alignChildren = ['fill', 'fill'];\n\
    w.grp.tab.expr.alphaC.alphaet = w.grp.tab.expr.alphaC.add ('edittext', undefined, alphaExpr,{multiline:true});\n\
    // \n\
    \n\
w.grp.PresetN = w.grp.add('group');\n\
w.grp.PresetN.orientation = 'row';\n\
w.grp.PresetN.alignChildren = ['fill', 'fill'];\n\
w.grp.PresetN.stN =w.grp.PresetN.add ('statictext', undefined, 'Preset Name');\n\
w.grp.PresetN.name = w.grp.PresetN.add ('edittext', undefined, presetName);\n\
w.grp.descriptionGrp = w.grp.add('group');\n\
w.grp.descriptionGrp.orientation = 'row';\n\
w.grp.descriptionGrp.alignChildren = ['fill', 'fill'];\n\
w.grp.descriptionGrp.descrst = w.grp.descriptionGrp.add ('statictext', undefined, 'Description:');\n\
w.grp.descriptionGrp.description = w.grp.descriptionGrp.add ('edittext', undefined, presetDescription,{multiline:true});\n\
w.grp.btnGrp = w.grp.add('Group');\n\
w.grp.btnGrp.orientation = 'row';\n\
w.grp.btnGrp.Ok =w.grp.btnGrp.add ('button', undefined, 'Apply');\n\
w.grp.btnGrp.Cancel =w.grp.btnGrp.add ('button', undefined, 'Cancel');\n\
w.grp.btnGrp.loadBtn = w.grp.btnGrp.add ('button', undefined, 'Load Preset');\n\
w.grp.btnGrp.saveBtn =w.grp.btnGrp.add('button', undefined, 'Save Preset');\n\
var result_temp = createJson(w.grp.tab.expr.redC.redet.text, w.grp.tab.expr.greenC.greenet.text, w.grp.tab.expr.blueC.blueet.text, w.grp.tab.expr.alphaC.alphaet.text, pluginVersion, w.grp.tab.expr.PresetN.name.text,w.grp.tab.expr.descriptionGrp.description.text);\n\
var result = JSON.stringify(result_temp);\n\
w.grp.btnGrp.loadBtn.onClick = function (){\n\
    var exprObj = readJson(pluginVersion);\n\
    if (exprObj.error === \"none\"){\n\
        w.grp.redC.redet.text =		exprObj.redExpr;\n\
        w.grp.greenC.greenet.text=	exprObj.greenExpr;\n\
        w.grp.blueC.blueet.text =	exprObj.blueExpr;\n\
        w.grp.alphaC.alphaet.text=	exprObj.alphaExpr;\n\
        w.grp.PresetN.name.text      =   exprObj.presetName; \n\
        w.grp.descriptionGrp.description.text =   exprObj.description \n\
    }\n\
    else { \n\
        alert (exprObj.error)};\n\
    }\n\
w.grp.btnGrp.saveBtn.onClick = function (){\n\
    saveAsJson (w.grp.tab.expr.redC.redet.text,w.grp.tab.expr.greenC.greenet.text, w.grp.tab.expr.blueC.blueet.text, w.grp.tab.expr.alphaC.alphaet.text, pluginVersion, w.grp.tab.expr.grp.PresetN.name.text,w.grp.tab.expr.grp.descriptionGrp.description.text);\n\
}\n\
w.grp.btnGrp.Ok.onClick = function(){\n\
    var strExpr =createJson(w.grp.tab.expr.redC.redet.text, w.grp.tab.expr.tab.expr.grp.greenC.greenet.text, w.grp.tab.expr.blueC.blueet.text, w.grp.tab.expr.alphaC.alphaet.text, pluginVersion, w.grp.tab.expr.PresetN.name.text,w.grp.tab.expr.descriptionGrp.description.text);\n\
w.close();\n\
result =JSON.stringify(strExpr);\n\
}\n\
w.grp.btnGrp.Cancel.onClick = function(){\n\
var ret = createJson (redExpr,greenExpr, blueExpr, alphaExpr, pluginVersion, w.grp.tab.expr.PresetN.name.text,w.grp.tab.expr.grp.descriptionGrp.description.text);\n\
w.close();\n\
result = JSON.stringify(ret);\n\
}\n\
w.onResizing = w.onResize = function(){this.layout.resize();}\n\
w.show();\n\
return result\n\
}\n\
exprScript(%s,%s,%s,%s,%s,%s,%s,%s,%s);";

std::string script_ae = script_ui.append(json_createJson).append(json_saveAsJson).append(json_readJson);

#endif /* scipt_h */
