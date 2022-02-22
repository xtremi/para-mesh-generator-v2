function imposeMinMax(el){
  if(el.value != ""){
    if(parseInt(el.value) < parseInt(el.min)){
      el.value = el.min;
    }
    if(parseInt(el.value) > parseInt(el.max)){
      el.value = el.max;
    }
  }
}

function getInputForm(){
	return document.getElementById("input_form");
}

function clearInputForm(){
  var inputForm = getInputForm();
  inputForm.innerHTML = "";
}

function generateIbeamInputForm(){
	generateInputForm("I_BEAM");
}
function generateCbeamInputForm(){
	generateInputForm("C_BEAM");
}

function createSlider(name, inputForm, minVal, maxVal, defVal, step)
{
    var valbox = document.createElement("INPUT");
    valbox.setAttribute("type", 	"number");
    valbox.setAttribute("min", 		minVal);
    valbox.setAttribute("max", 		maxVal);
    valbox.setAttribute("value",	defVal);
    valbox.setAttribute("step", 	step);
    valbox.setAttribute("onkeyup", 	"imposeMinMax(this)");
    valbox.id = name + "-valbox";
            
    var slider = document.createElement("INPUT");
    slider.setAttribute("type", 	"range");
    slider.setAttribute("min", 		minVal);
    slider.setAttribute("max", 		maxVal);
    slider.setAttribute("value", 	defVal);
    slider.setAttribute("step", 	step);
    slider.id = name + "-slider";
    slider.classList.add("slider");

    valbox.setAttribute("onchange", "updateTextInput(this.value,\"" + valbox.id + "\");");
    
    valbox.addEventListener("input", function(e){
        var val = parseFloat(e.target.value).toFixed(2);
        slider.value = val;
    })
    slider.addEventListener("input", function(e){				
        var val = parseFloat(e.target.value).toFixed(2);
        valbox.value = val;
    })
    
    inputForm.appendChild(slider);
    inputForm.appendChild(valbox);
}

function createCheckBox(name, inputForm)
{
    var checkBox = document.createElement("INPUT");
    checkBox.setAttribute("type", "checkbox");
    checkBox.setAttribute("checked", "checked");
    checkBox.id = name + "-valbox";
    inputForm.appendChild(checkBox);
}

function generateInputForm(modelType){
	clearInputForm();
	var inputForm = getInputForm();
    
	var model_types      = data.model_types;
	var model_input_spec = model_types[modelType];
	var title            = model_input_spec.title;
	var inputs           = model_input_spec.inputs;
	
	var titleP = document.createElement("p");
	titleP.appendChild(document.createTextNode(title));
	inputForm.appendChild(titleP);

	inputs.forEach(function(item, index){
		var p = document.createElement("p");
		var n = document.createTextNode(item.name);
		p.appendChild(n);
		inputForm.appendChild(p);
		
		if(item.type === "slider"){		
			var minVal      = 1.0;
			var maxVal      = 10.0;
			var defaultVal  = 5.0;
			var step        = 0.25;
			createSlider(name, inputForm, minVal, maxVal, defaultVal, step);
		}
		else if(item.type === "check"){
            createCheckBox(name, inputForm);
		}		
		
	});
	
	createIbeamModel(inputForm);
	
}
