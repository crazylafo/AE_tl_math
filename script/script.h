//
//  scipt.h
//  tlMath
//
//

#ifndef scipt_h
#define scipt_h

std::string script_ae = "function exprScript(redExpr,greenExpr,blueExpr,alphaExpr,presetName, presetDescription, pluginMAJORV, pluginMINORV, pluginBUGV){ \n\
var pluginVersion = pluginMAJORV+'.'+pluginMINORV+pluginBUGV;\n\
pluginVersion = parseFloat (pluginVersion); \n\
var w = new Window('dialog', 'Maths Expressions V'+pluginVersion, undefined, {resizeable:true} );\n\
w.sttxt= w.add ('statictext', undefined, 'Write here your math operations for each channels. Math operations are based on Mathematical Expression Toolkit Library');\n\
w.grp = w.add('group');\n\
w.grp.orientation='column';\n\
w.grp.alignment = ['fill', 'fill'];\n\
w.grp.alignChildren = ['fill', 'fill'];\n\
w.grp.redst = w.grp.add ('statictext', undefined,'Red Channel Expression : ');\n\
w.grp.redC = w.grp.add('group');\n\
w.grp.redC.orientation = 'row';\n\
w.grp.redC.alignment = ['fill', 'fill'];\n\
w.grp.redC.alignChildren = ['fill', 'fill'];\n\
w.grp.redC.redet = w.grp.redC.add ('edittext', undefined, redExpr,{multiline:true});\n\
w.grp.greenst = w.grp.add ('statictext', undefined,'Green Channel Expression :');\n\
w.grp.greenC = w. grp.add('group');\n\
w.grp.greenC.orientation = 'row';\n\
w.grp.greenC.alignChildren = ['fill', 'fill'];\n\
w.grp.greenC.greenet = w.grp.greenC.add ('edittext', undefined, greenExpr,{multiline:true});\n\
w.grp.bluest = w.grp.add('statictext', undefined, 'Blue Channel Expression :'); \n\
w.grp.blueC = w.grp.add('group');\n\
w.grp.blueC.orientation = 'row';\n\
w.grp.blueC.alignChildren = ['fill', 'fill'];\n\
w.grp.blueC.blueet = w.grp.blueC.add ('edittext', undefined, blueExpr,{multiline:true});\n\
w.grp.alphast = w.grp.add ('statictext', undefined, 'Alpha Channel Expression :');\n\
w.grp.alphaC = w.grp.add('group');\n\
w.grp.alphaC.orientation = 'row';\n\
w.grp.alphaC.alignChildren = ['fill', 'fill'];\n\
w.grp.alphaC.alphaet = w.grp.alphaC.add ('edittext', undefined, alphaExpr,{multiline:true});\n\
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
var result_temp = createJson(w.grp.redC.redet.text, w.grp.greenC.greenet.text, w.grp.blueC.blueet.text, w.grp.alphaC.alphaet.text, pluginVersion, w.grp.PresetN.name.text,w.grp.descriptionGrp.description.text);\n\
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
else {    alert (exprObj.error)};\n\
}\n\
w.grp.btnGrp.saveBtn.onClick = function (){\n\
    saveAsJson (w.grp.redC.redet.text, w.grp.greenC.greenet.text, w.grp.blueC.blueet.text, w.grp.alphaC.alphaet.text, pluginVersion, w.grp.PresetN.name.text,w.grp.descriptionGrp.description.text);\n\
}\n\
w.grp.btnGrp.Ok.onClick = function(){\n\
    var strExpr =createJson(w.grp.redC.redet.text, w.grp.greenC.greenet.text, w.grp.blueC.blueet.text, w.grp.alphaC.alphaet.text, pluginVersion, w.grp.PresetN.name.text,w.grp.descriptionGrp.description.text);\n\
w.close();\n\
result =JSON.stringify(strExpr);\n\
}\n\
w.grp.btnGrp.Cancel.onClick = function(){\n\
var ret = createJson (redExpr,greenExpr, blueExpr, alphaExpr, pluginVersion, w.grp.PresetN.name.text,w.grp.descriptionGrp.description.text);\n\
w.close();\n\
result = JSON.stringify(ret);\n\
}\n\
w.onResizing = w.onResize = function(){this.layout.resize();}\n\
w.show();\n\
return result\n\
}\n\
\n\
function createJson(redExpr,greenExpr,blueExpr,alphaExpr, pluginVersion, presetName, description){\n\
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
    }\n\
\n\
function saveAsJson(redExpr,greenExpr,blueExpr,alphaExpr, pluginVersion,presetName, description){\n\
    ExprObj = createJson(redExpr,greenExpr,blueExpr,alphaExpr, pluginVersion,presetName, description);\n\
    var presetFile =File.saveDialog('save your preset as a json');\n\
    if (presetFile && presetFile.open('w')){\n\
        presetFile.encoding ='UTF-8';\n\
        presetFile.write(JSON.stringify(ExprObj, undefined, '\\r\\n'));\n\
        presetFile.close();\n\
        }\n\
    };\n\
\n\
function readJson(pluginVersion){\n\
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
    };\n\
\n\
\n\
exprScript(%s,%s,%s,%s,%s,%s,%s,%s,%s);";
#endif /* scipt_h */
