std::string script_main_functions = R"=====(
	//
//  scipt TO CALL IN AEGP
//  tlMath
//
//


function parseBoolToInt (bin){ 
alert (bin)
if (bin === 'false'|| bin === 'FALSE'  ){return 0}
else {return 1}
}
function parseIntToBool (intvar){ 
if (intvar == 0){return false}
else {return true}
}
function analyseJson (testObj){
	ExprObj = {};
	try{
		if (testObj.effectName === "tlMath" && testObj.minimalPluginVersion <=pluginVersion){
			ExprObj.pluginVesion = testObj.pluginVesion;
			ExprObj.minimalPluginVersion = testObj.minimalPluginVersion;
			ExprObj.parserModeB  = parseBoolToInt (testObj.parserModeB);
			ExprObj.category = testObj.category;
			if (testObj.glslExpr){ExprObj.glslExpr = testObj.glslExpr;}
			if (testObj.evalglslExp){ExprObj.evalglslExp = testObj.evalglslExp}
			else {ExprObj.evalglslExp = 'NONE'}
			if (testObj.evalmathExp){ExprObj.evalmathExp = testObj.evalmathExp}
			else {ExprObj.evalmathExp = 'NONE'}
			if (testObj.presetName) {ExprObj.presetName  = testObj.presetName;} 
			if (testObj.description){ExprObj.description = testObj.description;}
			if (testObj.redExpr){ExprObj.redExpr     = testObj.redExpr;}
			if (testObj.greenExpr){ExprObj.greenExpr   = testObj.greenExpr;}
			if (testObj.blueExpr){ExprObj.blueExpr    = testObj.blueExpr;}
			if (testObj.alphaExpr){ExprObj.alphaExpr   = testObj.alphaExpr;}
			if (testObj.uiSliderGrpVisible) {ExprObj.uiSliderGrpVisible = testObj.uiSliderGrpVisible}
			if (testObj.uiSliderGrpName){ExprObj.uiSliderGrpName = testObj.uiSliderGrpName}
			if (testObj.uiSlider1Visible){ExprObj.uiSlider1Visible= testObj.uiSlider1Visible}
			if (testObj.uiSlider1Name){ExprObj.uiSlider1Name= testObj.uiSlider1Name}
			if (testObj.uiSlider2Visible){ExprObj.uiSlider2Visible= testObj.uiSlider2Visible}
			if (testObj.uiSlider2Name){ExprObj.uiSlider2Name= testObj.uiSlider2Name}
			if (testObj.uiSlider3Visible){ExprObj.uiSlider3Visible= testObj.uiSlider3Visible}
			if (testObj.uiSlider3Name){ExprObj.uiSlider3Name= testObj.uiSlider3Name}
			if (testObj.uiSlider4Visible){ExprObj.uiSlider4Visible= testObj.uiSlider4Visible}
			if (testObj.uiSlider4Name){ExprObj.uiSlider4Name= testObj.uiSlider4Name}
			if (testObj.uiPointGrpName){ExprObj.uiPointGrpName= testObj.uiPointGrpName}
			if (testObj.uiPointGrpName){ExprObj.uiPointGrpName= testObj.uiPointGrpName}
			if (testObj.uiPoint1Visible){ExprObj.uiPoint1Visible= testObj.uiPoint1Visible}
			if (testObj.uiPoint1Name){ExprObj.uiPoint1Name= testObj.uiPoint1Name}
			if (testObj.uiPoint2Visible){ExprObj.uiPoint2Visible= testObj.uiPoint2Visible}
			if (testObj.uiPoint2Name){ExprObj.uiPoint2Name= testObj.uiPoint2Name}
			if (testObj.uiColorGrpVisible){ExprObj.uiColorGrpVisible= testObj.uiColorGrpVisible}
			if (testObj.uiColorGrpName){ExprObj.uiColorGrpName= testObj.uiColorGrpName}
			if (testObj.uiColor1Visible){ExprObj.uiColor1Visible= testObj.uiColor1Visible}
			if (testObj.uiColor1Name){ExprObj.uiColor1Name= testObj.uiColor1Name}
			if (testObj.uiColor2Visible){ExprObj.uiColor2Visible= testObj.uiColor2Visible}
			if (testObj.uiColor2Name){ExprObj.uiColor2Name= testObj.uiColor2Name}
			if (testObj.extLGrpVisible){ExprObj.extLGrpVisible= testObj.extLGrpVisible}
			if (testObj.extLGrpName){ExprObj.extLGrpName= testObj.extLGrpName}
			ExprObj.error       = "none";
			}
		else {
			alert ("You must use plugin version "+ testObj.minimalPluginVersion+ " or higher");
			ExprObj.error = "err";
			}
	}catch (e) {
		ExprObj.error = "err";
		}
	return ExprObj;
}
 function getcallBacks(w){
    var exprCb = {};
    exprCb.parserModeB   =  parseIntToBool(w.grp.parserModeB.ddl.selection); 
    exprCb.presetName   =   w.grp.PresetN.name.text; 
    exprCb.description  =   w.grp.tab.paramUI.descriptionGrp.description.text; 
    exprCb.redExpr      =   w.grp.tab.expr.redC.redet.text;
    exprCb.greenExpr    =   w.grp.tab.expr.greenC.greenet.text;
    exprCb.blueExpr     =   w.grp.tab.expr.blueC.blueet.text;
    exprCb.alphaExpr    =   w.grp.tab.expr.alphaC.alphaet.text;
    exprCb.glslExpr     =   w.grp.tab.glsl.fragSh.fragShet.text;
	exprCb.uiSliderGrpVisible  = w.grp.tab.paramUI.param.paramslider.grpName.cbvisible.value;
	exprCb.uiSliderGrpName = w.grp.tab.paramUI.param.paramslider.grpName.editNameet.text;
	exprCb.uiSlider1Visible = w.grp.tab.paramUI.param.paramslider.paramSlider1.cbvisible.value; 
	exprCb.uiSlider1Name = w.grp.tab.paramUI.param.paramslider.paramSlider1.editNameet.text;
	exprCb.uiSlider2Visible = w.grp.tab.paramUI.param.paramslider.paramSlider2.cbvisible.value;
	exprCb.uiSlider2Name = w.grp.tab.paramUI.param.paramslider.paramSlider2.editNameet.text;
	exprCb.uiSlider3Visible = w.grp.tab.paramUI.param.paramslider.paramSlider3.cbvisible.value;
	exprCb.uiSlider3Name = w.grp.tab.paramUI.param.paramslider.paramSlider3.editNameet.text;
	exprCb.uiSlider4Visible = w.grp.tab.paramUI.param.paramslider.paramSlider4.cbvisible.value;
	exprCb.uiSlider4Name = w.grp.tab.paramUI.param.paramslider.paramSlider4.editNameet.text;
	exprCb.uiPointGrpVisible = w.grp.tab.paramUI.param.parampoint.grpName.cbvisible.value;
	exprCb.uiPointGrpName= w.grp.tab.paramUI.param.parampoint.grpName.editNameet.text;
	exprCb.uiPoint1Visible =w.grp.tab.paramUI.param.parampoint.parampoint1.cbvisible.value;
	exprCb.uiPoint1Name = w.grp.tab.paramUI.param.parampoint.parampoint1.editNameet.text;
	exprCb.uiPoint2Visible = w.grp.tab.paramUI.param.parampoint.parampoint2.cbvisible.value;
	exprCb.uiPoint2Name = w.grp.tab.paramUI.param.parampoint.parampoint2.editNameet.text;
	exprCb.uiColorGrpVisible =w.grp.tab.paramUI.param.paramcolor.grpName.cbvisible.value;
	exprCb.uiColorGrpName = w.grp.tab.paramUI.param.paramcolor.grpName.editNameet.text;
	exprCb.uiColor1Visible = w.grp.tab.paramUI.param.paramcolor.paramcolor1.cbvisible.value;
	exprCb.uiColor1Name = w.grp.tab.paramUI.param.paramcolor.paramcolor1.editNameet.text;
	exprCb.uiColor2Visible = w.grp.tab.paramUI.param.paramcolor.paramcolor2.cbvisible.value;
	exprCb.uiColor2Name = w.grp.tab.paramUI.param.paramcolor.paramcolor2.editNameet.text;
	exprCb.extLGrpVisible = w.grp.tab.paramUI.param.paramextL.grpName.cbvisible.value;
	exprCb.extLGrpName =w.grp.tab.paramUI.param.paramextL.grpName.editNameet.text;
	return exprCb ;
}

function createJson(exprCl, pluginVersion,  inEvalB){
	pluginVersionStr = pluginVersion.toString();
    ExprObj = {
        effectName   : "tlMath",
        parserModeB : exprCl.parserModeB, 
        category :  "Custom",
        presetName  : exprCl.presetName,
        description : exprCl.description, 
        pluginVesion : pluginVersionStr,
        minimalPluginVersion : "1.13",
        glslExpr  : exprCl.glslExpr,
		evalB :     inEvalB,
        redExpr   : exprCl.redExpr,
        greenExpr : exprCl.greenExpr,
        blueExpr  : exprCl.blueExpr,
        alphaExpr : exprCl.alphaExpr,
		uiSliderGrpVisible :	exprCl.uiSliderGrpVisible,
		uiSliderGrpName :  exprCl.uiSliderGrpName,
		uiSlider1Visible : exprCl.uiSlider1Visible,
		uiSlider1Name : exprCl.uiSlider1Name,
		uiSlider2Visible : exprCl.uiSlider2Visible,
		uiSlider2Name : exprCl.uiSlider2Name,
		uiSlider3Visible : exprCl.uiSlider3Visible,
		uiSlider3Name : exprCl.uiSlider3Name,
		uiSlider4Visible : exprCl.uiSlider4Visible,
		uiSlider4Name : exprCl.uiSlider4Name,
		uiPointGrpVisible : exprCl.uiPointGrpVisible,
		uiPointGrpName : exprCl.uiPointGrpName,
		uiPoint1Visible : exprCl.uiPoint1Visible,
		uiPoint1Name : exprCl.uiPoint1Name,
		uiPoint2Visible : exprCl.uiPoint2Visible,
		uiPoint2Name : exprCl.uiPoint2Name,
		uiColorGrpVisible : exprCl.uiColorGrpVisible,
		uiColorGrpName :  exprCl.uiColorGrpName,
		uiColor1Visible: exprCl.uiColor1Visible,
		uiColor1Name   : exprCl.uiColor1Name,
		uiColor2Visible: exprCl.uiColor2Visible,
		uiColor2Name   : exprCl.uiColor2Name,
		extLGrpVisible : exprCl.extLGrpVisible,
		extLGrpName: exprCl.extLGrpName
        };
        return ExprObj;
}

function saveAsJson(exprCl, pluginVersion){
    ExprObj = createJson(exprCl, pluginVersion,false);
    var presetFile =File.saveDialog('save your preset as a json');
    if (presetFile && presetFile.open('w')){
        presetFile.encoding ='UTF-8';
        presetFile.write(JSON.stringify(ExprObj, undefined, '\\r\\n'));
        presetFile.close();
    }
}

function readJson(pluginVersion){
    var ExprObj ={};
    var loadFile =File.openDialog('load your preset json');
    if (loadFile && loadFile.open('r')){
        loadFile.encoding ='UTF-8';
        var jsonFile = loadFile.read();
		var testObj = JSON.parse(jsonFile);
		ExprObj = analyseJson (testObj);
        loadFile.close();
        }
    return ExprObj;
}

function updateNumOfLines(textEditorStr) { 
    var numLines =1;
	var newStr = '';
	var splitStr = textEditorStr.split(/\r\n|\n/);
	for (var i=0; i<splitStr.length; i++)
	{
		newStr += numLines.toString() +'  -'+splitStr[i] +'\n';
		numLines +=1;
	}
	return newStr;
})=====";

std::string script_main_ui = R"=====(function exprScript( jsonInput, pluginMAJORV, pluginMINORV, pluginBUGV){ 
var pluginVersion = pluginMAJORV+'.'+pluginMINORV+pluginBUGV;
pluginVersion = parseFloat (pluginVersion); 
var exprCl;
exprCl= JSON.parse(jsonInput);
if (!exprCl.evalglslExp){
    exprCl.evalglslExp = 'no evalutation';
}
if (!exprCl.evalmathExp){
    exprCl.evalmathExp = 'no evalutation';
}
var w = new Window('dialog', 'Maths Expressions V'+pluginVersion, undefined, {resizeable:true} );
w.grp = w.add('group');
w.grp.orientation='column';
w.grp.alignment = ['fill', 'fill'];
w.grp.alignChildren = ['fill', 'fill'];
w.grp.btnGrp = w.grp.add('Group');
w.grp.btnGrp.orientation = 'row';
w.grp.btnGrp.alignChildren = ['fill', 'fill'];
w.grp.btnGrp.alignChildren = ['left', 'fill'];
w.grp.btnGrp.Ok =w.grp.btnGrp.add ('button', undefined, 'Apply');
w.grp.btnGrp.Cancel =w.grp.btnGrp.add ('button', undefined, 'Cancel');
w.grp.btnGrp.loadBtn = w.grp.btnGrp.add ('button', undefined, 'Load Preset');
w.grp.btnGrp.saveBtn =w.grp.btnGrp.add('button', undefined, 'Save Preset');

//PARSER MODE
w.grp.parserModeB = w.grp.add('group');
w.grp.parserModeB.orientation = 'row';
w.grp.parserModeB.alignChildren = ['fill', 'fill'];
w.grp.parserModeB.alignChildren = ['left', 'fill'];
w.grp.parserModeB.st =w.grp.parserModeB.add ('statictext', undefined, 'Parser Mode');
w.grp.parserModeB.ddl = w.grp.parserModeB.add ('dropdownlist', undefined, ['Math Expressions','Glsl'])
w.grp.parserModeB.ddl.selection =  exprCl.parserModeB;
// 
//PRESET NAME
w.grp.PresetN = w.grp.add('group');
w.grp.PresetN.orientation = 'row';
w.grp.PresetN.alignChildren = ['fill', 'fill'];
w.grp.PresetN.alignChildren = ['left', 'fill'];
w.grp.PresetN.stN =w.grp.PresetN.add ('statictext', undefined, 'Preset Name');
w.grp.PresetN.name = w.grp.PresetN.add ('edittext', undefined, exprCl.presetName, {characters :30});
// 
w.grp.tab = w.grp.add('tabbedpanel');
//Mode expr 
w.grp.tab.expr = w.grp.tab.add('tab', undefined, 'Math Expressions');
w.grp.tab.glsl= w.grp.tab.add('tab', undefined, 'Glsl');
w.grp.tab.paramUI= w.grp.tab.add('tab', undefined, 'UI Settings');
w.grp.tab.selection = w.grp.parserModeB.ddl.selection.index;
 //EXPR TAB 
    w.grp.tab.expr.orientation='column';
    w.grp.tab.expr.alignment  = ['fill', 'fill'];
    w.grp.tab.expr.alignChildren = ['fill', 'fill'];
    w.grp.tab.expr.btnGrp =w.grp.tab.expr.add ('group');
    w.grp.tab.expr.btnGrp.orientation='row';
    w.grp.tab.expr.btnGrp.exprEvalbtn = w.grp.tab.expr.btnGrp.add ('button', undefined, 'Evaluate Expression');
    w.grp.tab.expr.redst = w.grp.tab.expr.add ('statictext', undefined,'Red Channel Expression : ');
    w.grp.tab.expr.redC = w.grp.tab.expr.add('group');
    w.grp.tab.expr.redC.orientation = 'row';
    w.grp.tab.expr.redC.alignment = ['fill', 'fill'];
    w.grp.tab.expr.redC.alignChildren = ['fill', 'fill'];
    w.grp.tab.expr.redC.redet = w.grp.tab.expr.redC.add ('edittext', undefined, exprCl.redExpr,{multiline:true});
    w.grp.tab.expr.greenst = w.grp.tab.expr.add ('statictext', undefined,'Green Channel Expression :');
    w.grp.tab.expr.greenC = w.grp.tab.expr.add('group');
    w.grp.tab.expr.greenC.orientation = 'row';
	w.grp.tab.expr.greenC.alignment = ['fill', 'fill'];
    w.grp.tab.expr.greenC.alignChildren = ['fill', 'fill'];
    w.grp.tab.expr.greenC.greenet = w.grp.tab.expr.greenC.add ('edittext', undefined, exprCl.greenExpr,{multiline:true});
    w.grp.tab.expr.bluest = w.grp.tab.expr.add('statictext', undefined, 'Blue Channel Expression :'); 
    w.grp.tab.expr.blueC = w.grp.tab.expr.add('group');
    w.grp.tab.expr.blueC.orientation = 'row';
	w.grp.tab.expr.blueC.alignment = ['fill', 'fill'];
    w.grp.tab.expr.blueC.alignChildren = ['fill', 'fill'];
    w.grp.tab.expr.blueC.blueet = w.grp.tab.expr.blueC.add ('edittext', undefined, exprCl.blueExpr,{multiline:true});
    w.grp.tab.expr.alphast = w.grp.tab.expr.add ('statictext', undefined, 'Alpha Channel Expression :');
    w.grp.tab.expr.alphaC = w.grp.tab.expr.add('group');
    w.grp.tab.expr.alphaC.orientation = 'row';
	w.grp.tab.expr.alphaC.alignment = ['fill', 'fill'];
    w.grp.tab.expr.alphaC.alignChildren = ['fill', 'fill'];
    w.grp.tab.expr.alphaC.alphaet = w.grp.tab.expr.alphaC.add ('edittext', undefined, exprCl.alphaExpr,{multiline:true});
    w.grp.tab.expr.exprconsole =  w.grp.tab.expr.add ('edittext', undefined, 'math expression console ' +'\r-'+exprCl.evalmathExp,{readonly:true, multiline:true});
    

// GLSL TAB 
    w.grp.tab.glsl.orientation='column';
    w.grp.tab.glsl.alignment = ['fill', 'fill'];
    w.grp.tab.glsl.fragShst = w.grp.tab.glsl.add ('statictext', undefined,'GLSL Fragment Shader : ');
	w.grp.tab.glsl.btnGrp =w.grp.tab.glsl.add ('group');
	w.grp.tab.glsl.btnGrp.orientation='row';
	w.grp.tab.glsl.btnGrp.fragShevalbtn = w.grp.tab.glsl.btnGrp.add ('button', undefined, 'Evaluate Shader');
	w.grp.tab.glsl.btnGrp.fragShShpwBtn =  w.grp.tab.glsl.btnGrp.add ('button', undefined, 'Preview Code');
    w.grp.tab.glsl.fragSh = w.grp.tab.glsl.add('group');
    w.grp.tab.glsl.fragSh.orientation = 'column';
    w.grp.tab.glsl.fragSh.alignment = ['fill', 'center'];
	w.grp.tab.glsl.fragSh.alignChildren = ['fill', 'fill'];
	w.grp.tab.glsl.fragSh.fragShet = w.grp.tab.glsl.fragSh.add('edittext', undefined, exprCl.glslExpr, { multiline:true }); 
	w.grp.tab.glsl.fragSh.fragShet.maximumSize.height = w.maximumSize.height*0.65;
	w.grp.tab.glsl.fragSh.glslconsole = w.grp.tab.glsl.fragSh.add ('edittext', undefined, 'glsl console ' +'\r-'+exprCl.evalglslExp ,{readonly:true, multiline:true});
// 

//tab UI
	w.grp.tab.paramUI.orientation = 'column';
	w.grp.tab.paramUI.paramSt = w.grp.tab.paramUI.add ('statictext', undefined, 'Define Parameters Name');
	w.grp.tab.paramUI.param = w.grp.tab.paramUI.add('group');
	w.grp.tab.paramUI.param.orientation = 'row';
	w.grp.tab.paramUI.param.paramslider = w.grp.tab.paramUI.param.add('group');
	w.grp.tab.paramUI.param.paramslider.orientation = 'column';
	w.grp.tab.paramUI.param.paramslider.grpName =w.grp.tab.paramUI.param.paramslider.add('group');
	w.grp.tab.paramUI.param.paramslider.grpName.orientation = 'row';
	w.grp.tab.paramUI.param.paramslider.grpName.originalNameSt = w.grp.tab.paramUI.param.paramslider.grpName.add ('statictext', undefined, 'Slider group :');
	w.grp.tab.paramUI.param.paramslider.grpName.cbvisible = w.grp.tab.paramUI.param.paramslider.grpName.add ('checkbox', undefined, 'visible');
	w.grp.tab.paramUI.param.paramslider.grpName.cbvisible.value = exprCl.uiSliderGrpVisible;
	w.grp.tab.paramUI.param.paramslider.grpName.editNameet = w.grp.tab.paramUI.param.paramslider.grpName.add('edittext', undefined,exprCl.uiSliderGrpName, { characters: 30 }); 
	w.grp.tab.paramUI.param.paramslider.paramSlider1 = w.grp.tab.paramUI.param.paramslider.add('group'); 
	w.grp.tab.paramUI.param.paramslider.paramSlider1.orientation = 'row'; 
	w.grp.tab.paramUI.param.paramslider.paramSlider1.originalNameSt = w.grp.tab.paramUI.param.paramslider.paramSlider1.add('statictext', undefined, 'slider 1 :'); 
	w.grp.tab.paramUI.param.paramslider.paramSlider1.cbvisible = w.grp.tab.paramUI.param.paramslider.paramSlider1.add('checkbox', undefined, 'visible'); 
	w.grp.tab.paramUI.param.paramslider.paramSlider1.cbvisible.value =	exprCl.uiSlider1Visible;
	w.grp.tab.paramUI.param.paramslider.paramSlider1.editNameet = w.grp.tab.paramUI.param.paramslider.paramSlider1.add ('edittext', undefined, exprCl.uiSlider1Name, { characters: 30 });
	w.grp.tab.paramUI.param.paramslider.paramSlider2 = w.grp.tab.paramUI.param.paramslider.add('group');
    w.grp.tab.paramUI.param.paramslider.paramSlider2.orientation = 'row';
	w.grp.tab.paramUI.param.paramslider.paramSlider2.originalNameSt = w.grp.tab.paramUI.param.paramslider.paramSlider2.add ('statictext', undefined, 'slider 2 :');
	w.grp.tab.paramUI.param.paramslider.paramSlider2.cbvisible = w.grp.tab.paramUI.param.paramslider.paramSlider2.add ('checkbox', undefined, 'visible');
	w.grp.tab.paramUI.param.paramslider.paramSlider2.cbvisible.value = exprCl.uiSlider2Visible;
	w.grp.tab.paramUI.param.paramslider.paramSlider2.editNameet = w.grp.tab.paramUI.param.paramslider.paramSlider2.add ('edittext', undefined, exprCl.uiSlider2Name,{characters: 30});
	w.grp.tab.paramUI.param.paramslider.paramSlider3 = w.grp.tab.paramUI.param.paramslider.add('group');
    w.grp.tab.paramUI.param.paramslider.paramSlider3.orientation = 'row';
	w.grp.tab.paramUI.param.paramslider.paramSlider3.originalNameSt = w.grp.tab.paramUI.param.paramslider.paramSlider3.add ('statictext', undefined, 'slider 3 :');
	w.grp.tab.paramUI.param.paramslider.paramSlider3.cbvisible = w.grp.tab.paramUI.param.paramslider.paramSlider3.add ('checkbox', undefined, 'visible');
	w.grp.tab.paramUI.param.paramslider.paramSlider3.cbvisible.value = exprCl.uiSlider3Visible; 
	w.grp.tab.paramUI.param.paramslider.paramSlider3.editNameet = w.grp.tab.paramUI.param.paramslider.paramSlider3.add ('edittext', undefined, exprCl.uiSlider3Name,{characters: 30});
	w.grp.tab.paramUI.param.paramslider.paramSlider4 = w.grp.tab.paramUI.param.paramslider.add('group');
    w.grp.tab.paramUI.param.paramslider.paramSlider4.orientation = 'row';
	w.grp.tab.paramUI.param.paramslider.paramSlider4.originalNameSt = w.grp.tab.paramUI.param.paramslider.paramSlider4.add ('statictext', undefined, 'slider 4 :');
	w.grp.tab.paramUI.param.paramslider.paramSlider4.cbvisible = w.grp.tab.paramUI.param.paramslider.paramSlider4.add ('checkbox', undefined, 'visible');
	w.grp.tab.paramUI.param.paramslider.paramSlider4.cbvisible.value = exprCl.uiSlider4Visible;
	w.grp.tab.paramUI.param.paramslider.paramSlider4.editNameet = w.grp.tab.paramUI.param.paramslider.paramSlider4.add('edittext', undefined, exprCl.uiSlider4Name, { characters: 30 }); 
	w.grp.tab.paramUI.param.parampoint = w.grp.tab.paramUI.param.add('group'); 
	w.grp.tab.paramUI.param.parampoint.orientation = 'column'; 
	w.grp.tab.paramUI.param.parampoint.grpName = w.grp.tab.paramUI.param.parampoint.add('group'); 
	w.grp.tab.paramUI.param.parampoint.grpName.orientation = 'row'; 
	w.grp.tab.paramUI.param.parampoint.grpName.originalNameSt = w.grp.tab.paramUI.param.parampoint.grpName.add('statictext', undefined, '2d points group :'); 
	w.grp.tab.paramUI.param.parampoint.grpName.cbvisible = w.grp.tab.paramUI.param.parampoint.grpName.add('checkbox', undefined, 'visible'); 
	w.grp.tab.paramUI.param.parampoint.grpName.cbvisible.value = exprCl.uiPointGrpVisible;
	w.grp.tab.paramUI.param.parampoint.grpName.editNameet = w.grp.tab.paramUI.param.parampoint.grpName.add('edittext', undefined,exprCl.uiPointGrpName, { characters: 30 }); 
	w.grp.tab.paramUI.param.parampoint.parampoint1 = w.grp.tab.paramUI.param.parampoint.add('group'); 
	w.grp.tab.paramUI.param.parampoint.parampoint1.orientation = 'row'; 
	w.grp.tab.paramUI.param.parampoint.parampoint1.originalNameSt = w.grp.tab.paramUI.param.parampoint.parampoint1.add('statictext', undefined, 'point 1 :'); 
	w.grp.tab.paramUI.param.parampoint.parampoint1.cbvisible = w.grp.tab.paramUI.param.parampoint.parampoint1.add('checkbox', undefined, 'visible'); 
	w.grp.tab.paramUI.param.parampoint.parampoint1.cbvisible.value = exprCl.uiPoint1Visible;
	w.grp.tab.paramUI.param.parampoint.parampoint1.editNameet = w.grp.tab.paramUI.param.parampoint.parampoint1.add('edittext', undefined, exprCl.uiPoint1Name, { characters: 30 }); 
	w.grp.tab.paramUI.param.parampoint.parampoint2 = w.grp.tab.paramUI.param.parampoint.add('group'); 
	w.grp.tab.paramUI.param.parampoint.parampoint2.orientation = 'row'; 
	w.grp.tab.paramUI.param.parampoint.parampoint2.originalNameSt = w.grp.tab.paramUI.param.parampoint.parampoint2.add('statictext', undefined, 'point 2 :'); 
	w.grp.tab.paramUI.param.parampoint.parampoint2.cbvisible = w.grp.tab.paramUI.param.parampoint.parampoint2.add('checkbox', undefined, 'visible'); 
	w.grp.tab.paramUI.param.parampoint.parampoint2.cbvisible.value=	exprCl.uiPoint2Visible;
	w.grp.tab.paramUI.param.parampoint.parampoint2.editNameet = w.grp.tab.paramUI.param.parampoint.parampoint2.add('edittext', undefined, exprCl.uiPoint2Name, { characters: 30 }); 
	w.grp.tab.paramUI.param.paramcolor = w.grp.tab.paramUI.param.add('group'); 
	w.grp.tab.paramUI.param.paramcolor.orientation = 'column'; 
	w.grp.tab.paramUI.param.paramcolor.grpName = w.grp.tab.paramUI.param.paramcolor.add('group'); 
	w.grp.tab.paramUI.param.paramcolor.grpName.orientation = 'row'; 
	w.grp.tab.paramUI.param.paramcolor.grpName.originalNameSt = w.grp.tab.paramUI.param.paramcolor.grpName.add('statictext', undefined, 'colors group :'); 
	w.grp.tab.paramUI.param.paramcolor.grpName.cbvisible = w.grp.tab.paramUI.param.paramcolor.grpName.add('checkbox', undefined, 'visible'); 
	w.grp.tab.paramUI.param.paramcolor.grpName.cbvisible.value = exprCl.uiColorGrpVisible
	w.grp.tab.paramUI.param.paramcolor.grpName.editNameet = w.grp.tab.paramUI.param.paramcolor.grpName.add('edittext', undefined, exprCl.uiColorGrpName , { characters: 30 }); 
	w.grp.tab.paramUI.param.paramcolor.paramcolor1 = w.grp.tab.paramUI.param.paramcolor.add('group'); 
	w.grp.tab.paramUI.param.paramcolor.paramcolor1.orientation = 'row'; 
	w.grp.tab.paramUI.param.paramcolor.paramcolor1.originalNameSt = w.grp.tab.paramUI.param.paramcolor.paramcolor1.add('statictext', undefined, 'color 1 :'); 
	w.grp.tab.paramUI.param.paramcolor.paramcolor1.cbvisible = w.grp.tab.paramUI.param.paramcolor.paramcolor1.add('checkbox', undefined, 'visible'); 
	w.grp.tab.paramUI.param.paramcolor.paramcolor1.cbvisible.value = exprCl.uiColor1Visible;
	w.grp.tab.paramUI.param.paramcolor.paramcolor1.editNameet = w.grp.tab.paramUI.param.paramcolor.paramcolor1.add('edittext', undefined, exprCl.uiColor1Name, { characters: 30 }); 
	w.grp.tab.paramUI.param.paramcolor.paramcolor2 = w.grp.tab.paramUI.param.paramcolor.add('group'); 
	w.grp.tab.paramUI.param.paramcolor.paramcolor2.orientation = 'row'; 
	w.grp.tab.paramUI.param.paramcolor.paramcolor2.originalNameSt = w.grp.tab.paramUI.param.paramcolor.paramcolor2.add('statictext', undefined, 'color 2 :'); 
	w.grp.tab.paramUI.param.paramcolor.paramcolor2.cbvisible = w.grp.tab.paramUI.param.paramcolor.paramcolor2.add('checkbox', undefined, 'visible'); 
	w.grp.tab.paramUI.param.paramcolor.paramcolor2.cbvisible.value = exprCl.uiColor2Visible;
	w.grp.tab.paramUI.param.paramcolor.paramcolor2.editNameet = w.grp.tab.paramUI.param.paramcolor.paramcolor2.add('edittext', undefined, exprCl.uiColor2Name, { characters: 30 }); 
	w.grp.tab.paramUI.param.paramextL = w.grp.tab.paramUI.param.add('group'); 
	w.grp.tab.paramUI.param.paramextL.orientation = 'column';
	w.grp.tab.paramUI.param.paramextL.grpName = w.grp.tab.paramUI.param.paramextL.add('group'); 
	w.grp.tab.paramUI.param.paramextL.grpName.originalNameSt = w.grp.tab.paramUI.param.paramextL.grpName.add('statictext', undefined, 'external Layer group :'); 
	w.grp.tab.paramUI.param.paramextL.grpName.cbvisible = w.grp.tab.paramUI.param.paramextL.grpName.add('checkbox', undefined, 'visible'); 
	w.grp.tab.paramUI.param.paramextL.grpName.cbvisible.value = exprCl.extLGrpVisible;
	w.grp.tab.paramUI.param.paramextL.grpName.editNameet = w.grp.tab.paramUI.param.paramextL.grpName.add('edittext', undefined, exprCl.extLGrpName,{ characters: 30 }); 
	w.grp.tab.paramUI.descriptionGrp = w.grp.tab.paramUI.add('group');
	w.grp.tab.paramUI.descriptionGrp.orientation = 'row';
	w.grp.tab.paramUI.descriptionGrp.alignment = ['fill', 'fill'];
	w.grp.tab.paramUI.descriptionGrp.descrst = w.grp.tab.paramUI.descriptionGrp.add ('statictext', undefined, 'Description:');
	w.grp.tab.paramUI.descriptionGrp.description = w.grp.tab.paramUI.descriptionGrp.add ('edittext', undefined, exprCl.description,{multiline:true});
)=====";
std::string script_main_ui_callbacks = R"=====(
//CALLBACK 

var tempRet =getcallBacks (w);
var result_temp = createJson(tempRet, pluginVersion,false);
var result = JSON.stringify(result_temp);
w.grp.parserModeB.ddl.onChange = function (){ 
	if (w.grp.parserModeB.ddl.selection == 0){
		w.grp.tab.selection = 0;
		}
	else{
		w.grp.tab.selection =1;
		}
	}
w.grp.btnGrp.loadBtn.onClick = function (){
    var exprObj = readJson(pluginVersion);
    if (exprObj.error === "none"){
		w.grp.parserModeB.ddl.selection =  parseInt(exprObj.parserModeB); 
		w.grp.tab.glsl.fragSh.fragShet.text = exprObj.glslExpr;
        w.grp.tab.expr.redC.redet.text      = exprObj.redExpr;
        w.grp.tab.expr.greenC.greenet.text  = exprObj.greenExpr;
        w.grp.tab.expr.blueC.blueet.text    = exprObj.blueExpr;
        w.grp.tab.expr.alphaC.alphaet.text  =exprObj.alphaExpr;
        w.grp.PresetN.name.text= exprObj.presetName; 
		w.grp.tab.paramUI.param.paramslider.grpName.cbvisible= exprObj.uiSliderGrpVisible;
		w.grp.tab.paramUI.param.paramslider.grpName.editNameet.text= exprObj.uiSliderGrpName;
		w.grp.tab.paramUI.param.paramslider.paramSlider1.cbvisible.value= exprObj.uiSlider1Visible; 
		w.grp.tab.paramUI.param.paramslider.paramSlider1.editNameet.text = exprObj.uiSlider1Name; 
		w.grp.tab.paramUI.param.paramslider.paramSlider2.cbvisible.value = exprObj.uiSlider2Visible; 
		w.grp.tab.paramUI.param.paramslider.paramSlider2.editNameet.text = exprObj.uiSlider2Name; 
		w.grp.tab.paramUI.param.paramslider.paramSlider3.cbvisible.value = exprObj.uiSlider3Visible; 
		w.grp.tab.paramUI.param.paramslider.paramSlider3.editNameet.text = exprObj.uiSlider3Name; 
		w.grp.tab.paramUI.param.paramslider.paramSlider4.cbvisible.value = exprObj.uiSlider4Visible; 
		w.grp.tab.paramUI.param.paramslider.paramSlider4.editNameet.text = exprObj.uiSlider4Name; 
		w.grp.tab.paramUI.param.parampoint.grpName.cbvisible.value = exprObj.uiPointGrpVisible; 
		w.grp.tab.paramUI.param.parampoint.grpName.editNameet.text = exprObj.uiPointGrpName; 
		w.grp.tab.paramUI.param.parampoint.parampoint1.cbvisible.value = exprObj.uiPoint1Visible; 
		w.grp.tab.paramUI.param.parampoint.parampoint1.editNameet.text = exprObj.uiPoint1Name; 
		w.grp.tab.paramUI.param.parampoint.parampoint2.cbvisible.value = exprObj.uiPoint2Visible; 
		w.grp.tab.paramUI.param.parampoint.parampoint2.editNameet.text = exprObj.uiPoint2Name; 
		w.grp.tab.paramUI.param.paramcolor.grpName.cbvisible.value = exprObj.uiColorGrpVisible;  
		w.grp.tab.paramUI.param.paramcolor.grpName.editNameet.text = exprObj.uiColorGrpName;  
		w.grp.tab.paramUI.param.paramcolor.paramcolor1.cbvisible.value =exprObj.uiColor1Visible;
		w.grp.tab.paramUI.param.paramcolor.paramcolor1.editNameet.text=exprObj.uiColor1Name;
		w.grp.tab.paramUI.param.paramcolor.paramcolor2.cbvisible.value=exprObj.uiColor2Visible;
	    w.grp.tab.paramUI.param.paramcolor.paramcolor2.editNameet.text =exprObj.uiColor2Name;
		w.grp.tab.paramUI.param.paramextL.grpName.cbvisible.value = exprObj.extLGrpVisible;
		w.grp.tab.paramUI.param.paramextL.grpName.editNameet.text = exprObj.extLGrpName;
        w.grp.tab.paramUI.descriptionGrp.description.text   = exprObj.description; 
		}
    else { 
        alert (exprObj.error);
		}
    }

w.grp.btnGrp.saveBtn.onClick = function (){
    var exprRet =getcallBacks (w);
    saveAsJson (exprRet, pluginVersion);
    }
w.grp.tab.glsl.btnGrp.fragShevalbtn.onClick = function(){
	var exprRet = getcallBacks(w);
    var strExpr =createJson(exprRet, pluginVersion,true);
    w.close();
	result =JSON.stringify(strExpr);
    }
w.grp.tab.expr.btnGrp.exprEvalbtn.onClick = function(){
    var exprRet = getcallBacks(w);
    var strExpr =createJson(exprRet, pluginVersion,true);
    w.close();
    result =JSON.stringify(strExpr);
    }

w.grp.tab.glsl.btnGrp.fragShShpwBtn.onClick = function(){
	var floatbox = new Window('dialog', 'shader',undefined,{resizeable:true} ); 
	floatbox.txt = floatbox.add('edittext', undefined, updateNumOfLines(w.grp.tab.glsl.fragSh.fragShet.text), {multiline:true, readonly:true} )
	floatbox.txt.maximumSize.height = floatbox.maximumSize.height*0.8;
	floatbox.onResizing = w.onResize = function(){this.layout.resize();}
    floatbox.show();
    }
w.grp.btnGrp.Ok.onClick = function(){
    var exprRet = getcallBacks(w);
    var strExpr =createJson(exprRet, pluginVersion,false);
	w.close();
	result =JSON.stringify(strExpr);
	}
w.grp.btnGrp.Cancel.onClick = function(){
    var exprRet = getcallBacks (w);
    var ret = createJson(exprRet, pluginVersion,false);
	w.close();
	result = JSON.stringify(ret);
	}
w.onResizing = w.onResize = function(){this.layout.resize();}
w.show();
return result
}

exprScript(%s, %s, %s, %s);
)=====";



