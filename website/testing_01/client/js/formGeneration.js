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


/*
	Creates a slider connected value input box
	with name/min/max/default/step values
	and adds them to the input form
*/
function createSlider(name, inputForm, minVal, maxVal, defVal, step, isIntSlider)
{
    var div = document.createElement("div");
	div.className = "slider";
	
	var valbox = document.createElement("input");
	valbox.className = "slider-value";
	
	var slider = document.createElement("input");
	slider.className = "slider-input";
	
	var label = document.createElement("span");
	label.className = "slider-label";
	
	//Label:
	label.innerHTML  = name;
	
	//Value box:
    valbox.setAttribute("type", 	"number");
    valbox.setAttribute("min", 		minVal);
    valbox.setAttribute("max", 		maxVal);
    valbox.setAttribute("value",	defVal);
    valbox.setAttribute("step", 	step);
    valbox.setAttribute("onkeyup", 	"imposeMinMax(this)");
    valbox.id   = name + "-valbox";
	valbox.name = name + "-valbox";
            
	//Slider:
    slider.setAttribute("type", 	"range");
    slider.setAttribute("min", 		minVal);
    slider.setAttribute("max", 		maxVal);
    slider.setAttribute("value", 	defVal);
    slider.setAttribute("step", 	step);
    slider.id 	= name + "-slider";
	slider.name = name + "-slider";

//    valbox.setAttribute("onchange", "updateTextInput(this.value,\"" + valbox.id + "\");");
    	
	var precision = 2;
	if(isIntSlider === true){
		precision = 0;
	}
    valbox.addEventListener("input", function(e){
        var val = parseFloat(e.target.value).toFixed(precision);
        slider.value = val;
    })
    slider.addEventListener("input", function(e){				
        var val = parseFloat(e.target.value).toFixed(precision);
        valbox.value = val;
    })
    	
	inputForm.appendChild(div);
	div.appendChild(label);
    div.appendChild(slider);
    div.appendChild(valbox);
}