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
	Get the input form DOM element (<form/>)
*/
function getInputForm(){
	return document.getElementById("input_form");
}


/*
	Clear the input form DOM element (<form/>)
*/
function clearInputForm(){
  var inputForm = getInputForm();
  inputForm.innerHTML = "";
}

/*
	Generate input form function for each type
*/
function generateIbeamInputForm(){		generateInputForm("I_BEAM"); }
function generateCbeamInputForm(){		generateInputForm("C_BEAM"); }
function generatePipeInputForm(){ 		generateInputForm("PIPE");	 }
function generateGenericInputForm(){ 	generateInputForm("GENERIC");}

/*
	Main function for creating the input form
	- clears the input form
	- get data from data.js (contains information of each type of generation)
	- creates the sliders/checkboxes/text boxes etc for the form
	- TODO: for certain types, create a 3D view that show the shape
	- TODO: create the ajax call to server, and add as handler to submit button
*/
function generateInputForm(modelType){
	clearInputForm();
	var inputForm = getInputForm();
    
	var model_types      = data.model_types;
	var model_input_spec = model_types[modelType];
	var title            = model_input_spec.title;
	var inputs           = model_input_spec.inputs;
	
	var titleP = document.createElement("H2");
	titleP.appendChild(document.createTextNode(title));
	inputForm.appendChild(titleP);

	inputs.forEach(function(item, index){
		var name = item.name;
		var p = document.createElement("H4");
		var n = document.createTextNode(name);
		p.appendChild(n);
		inputForm.appendChild(p);
		
		if(item.type === "slider"){		
			var minVal      = 1.0; var maxVal      = 10.0;
			var defaultVal  = 5.0; var step        = 0.25;
			createSlider(name, inputForm, minVal, maxVal, defaultVal, step);
		}
		else if(item.type === "check"){
            createCheckBox(name, inputForm);
		}	
		else if(item.type === "text"){
            createTextBox(name, inputForm);
		}		
		
		var submitButton = document.createElement("input");
		submitButton.setAttribute('type',"submit");
		submitButton.setAttribute('value',"Submit");
		inputForm.appendChild(submitButton);
		
	});
	
	createIbeamModel(inputForm);
	createAJAXsubmitHandler(inputForm);
}

/*
*/
function createAJAXsubmitHandler(inputForm){
	
	let $form = $('#input_form');
	console.log(inputForm);
	console.log($form);
	console.log($form.serialize());
	
	$form.on('submit', submitHandler);
	
	var type = "GENERIC";
	function submitHandler (e) {
	  e.preventDefault()
		
	  $.ajax({
		url 		: '/generate',
		type		: 'POST',
		method		: 'POST',
		data		: $form.serialize() + "&type=" + type,
		beforeSend	: function(){
			//document.getElementById("loader").style.display = "block";
		},
		complete	: function(data){
			//document.getElementById("loader").style.display = "none";
		}
		//timeout: 600000
	  }).done(response => {
		console.log(response);
		//document.location.assign(response);
		
		var a = document.createElement('a');
		a.href = response;
		// Give filename you wish to download
		a.download = "result.txt"; //document.getElementById("input3").value;
		a.style.display = 'none';
		document.body.appendChild(a);
		a.click();
	  })

	}		
	
}




/*
	Creates a slider connected value input box
	with name/min/max/default/step values
	and adds them to the input form
*/
function createSlider(name, inputForm, minVal, maxVal, defVal, step)
{
    var valbox = document.createElement("INPUT");
    valbox.setAttribute("type", 	"number");
    valbox.setAttribute("min", 		minVal);
    valbox.setAttribute("max", 		maxVal);
    valbox.setAttribute("value",	defVal);
    valbox.setAttribute("step", 	step);
    valbox.setAttribute("onkeyup", 	"imposeMinMax(this)");
    valbox.id   = name + "-valbox";
	valbox.name = name + "-valbox";
            
    var slider = document.createElement("INPUT");
    slider.setAttribute("type", 	"range");
    slider.setAttribute("min", 		minVal);
    slider.setAttribute("max", 		maxVal);
    slider.setAttribute("value", 	defVal);
    slider.setAttribute("step", 	step);
    slider.id 	= name + "-slider";
	slider.name = name + "-slider";
    slider.classList.add("slider");

    //valbox.setAttribute("onchange", "updateTextInput(this.value,\"" + valbox.id + "\");");
    
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

/*
	Creates a checkbox adds it to the input form
*/
function createCheckBox(name, inputForm)
{
    var checkBox = document.createElement("INPUT");
    checkBox.setAttribute("type", "checkbox");
    checkBox.setAttribute("checked", "checked");
    checkBox.id   = name;
	checkBox.name = name;
    inputForm.appendChild(checkBox);
}

/*
	Creates a textbox adds it to the input form
*/
function createTextBox(name, inputForm)
{
    var textBox = document.createElement("INPUT");
    textBox.setAttribute("type", "text");
    textBox.setAttribute("checked", "checked");
	textBox.setAttribute("size", "150");
    textBox.id   = name;
	textBox.name = name;
    inputForm.appendChild(textBox);
}


