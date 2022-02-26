var express    = require('express');
const path 	   = require('path');
var bodyParser = require('body-parser');
var bodyParser = require('body-parser');
const { v4: uuidv4 } = require('uuid');

var app = express();
var router = express.Router();

var clientPath = path.join(__dirname, 'client');

app.use(express.static(clientPath));

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));


//app.get('/css/styles.css', function(req, res){	
//	res.sendFile(path.join(__dirname + '/client/css/styles.css'));	
//});
//app.get('/css/input_form_style.css', function(req, res){	
//	res.sendFile(path.join(__dirname + '/client/css/input_form_style.css'));	
//})
//app.get('/js/formGeneration.js', function (req, res) {
//  	res.sendFile(path.join(__dirname + '/client/js/formGeneration.js'));
//});
//app.get('/media/reload.gif', function (req, res) {
//  	res.sendFile(path.join(__dirname + '/client/media/reload.gif'));
//});
//app.get('/js/data.js', function (req, res) {
//  	res.sendFile(path.join(__dirname + '/client/js/data.js'));
//});
//app.get('/js/modelGeneration.js', function (req, res) {
//  	res.sendFile(path.join(__dirname + '/client/js/modelGeneration.js'));
//});
//app.get('/js/OrbitControls.js', function (req, res) {
//  	res.sendFile(path.join(__dirname + '/client/js/OrbitControls.js'));
//});
//app.get('/js/three.js', function (req, res) {
//  	res.sendFile(path.join(__dirname + '/client/js/three.js'));
//});


app.get('/', function(req, res){
	res.redirect('index.html');
});

function parseInputForm(formData){
	console.log("\t> parseInputForm()");	
	console.log("\t\tType    : " + formData.type);
	console.log("\t\targs_d : " + formData["args_d"]);
	
	var arguments = [];
	if(formData.type == "DEBUG"){
		var argumentsString = formData["args_d"];
		arguments = argumentsString.split(" ");
	}
	
	return arguments;
}


app.post('/generate', async function(req, res)
{
	
	console.log(" > app.post generate");
	const filename = "generated_models/" + uuidv4() + ".txt";
	
	var args = parseInputForm(req.body);		
	args.push("-o", filename);	
	console.log("\targuments : " + args);
	
	var execFile = require('child_process').execFile;
	
	var child = execFile("../../output/x64/Release/para-mesh-generator-v2.exe", args,
		function (error, stdout, stderr){
			
			
			console.log("\tFile  : " + filename);
			console.log("\terr    : " + error);
			console.log("\tstderr : " + stderr);
			console.log("\n\tstdout : ");
			console.log("----------------------");
			console.log(stdout);
			console.log("----------------------\n");
										
			const file = __dirname + "/" + filename;
			app.get("/" + filename, function (req, res) {
				res.sendFile(path.join(file));
				res.status(200);
			});

			//res.download(file);
			console.log("res.send start");
			res.send(filename);
			console.log("res.send end");
		}
	)	

})

const port = process.env.PORT || 1337;
var server = app.listen(port, function () {
	const host = server.address().address;
	const port = server.address().port;
	console.log(`Example app listening at http://${host}:${port}`);

});
