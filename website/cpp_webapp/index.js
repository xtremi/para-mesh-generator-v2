var express    = require('express');
const path 	   = require('path');
var bodyParser = require('body-parser');
var app = express();
var router = express.Router();

app.use(express.static('public'));
//app.set('view engine', 'jade');

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

app.get('/media/reload.gif', function (req, res) {
  	res.sendFile(path.join(__dirname + '/client/media/reload.gif'));
});
app.get('/index.html', function (req, res) {
  	res.sendFile(path.join(__dirname + '/client/index.html'));
});
app.get('/js/data.js', function (req, res) {
  	res.sendFile(path.join(__dirname + '/client/js/data.js'));
});
app.get('/js/formGeneration.js', function (req, res) {
  	res.sendFile(path.join(__dirname + '/client/js/formGeneration.js'));
});
app.get('/js/modelGeneration.js', function (req, res) {
  	res.sendFile(path.join(__dirname + '/client/js/modelGeneration.js'));
});
app.get('/js/OrbitControls.js', function (req, res) {
  	res.sendFile(path.join(__dirname + '/client/js/OrbitControls.js'));
});
app.get('/js/three.js', function (req, res) {
  	res.sendFile(path.join(__dirname + '/client/js/three.js'));
});
app.get('/styles.css', function(req, res){	
	res.sendFile(path.join(__dirname + '/client/styles.css'));	
});
app.get('/', function(req, res){
	res.redirect('index.html');
});

//D:\nodejs\cpp_webapp\cpp\para-mesh-generator\$(Platform)\$(Configuration)\

function parseInputForm(formData){
	console.log("\t> parseInputForm()");	
	console.log("\t\tType: " + formData.type);
	console.log(formData);
	
	var arguments = [];
	if(formData.type == "GENERIC"){
		var argumentsString = formData["arguments"];
		arguments = argumentsString.split(" ");
	}
	
	return arguments;
}

var program = "cpp/para-mesh-generator/x64/Debug/para-mesh-generator";

app.post('/generate', async function(req, res)
{
	
	console.log(" > app.post generate");
	const filename = "out.txt";
	
	var args = parseInputForm(req.body);		
	args.push("--out", filename);	
	console.log("arguments : " + args);
	
	var execFile = require('child_process').execFile;
	
	var child = execFile(program, args,
		function (error, stdout, stderr){
			
			
			console.log("\n#File  : " + filename);
			console.log("\terr    : " + error);
			console.log("\tstderr : " + stderr);
			console.log("\n\tstdout : ");
			console.log("----------------------");
			console.log(stdout);
			console.log("----------------------\n");
										
			const file = __dirname + "/" + filename;
			app.get("/" + filename, function (req, res) {
				res.sendFile(path.join(file));
			});

			//res.download(file);
			res.send(filename);
		}
	)	

})

const port = process.env.PORT || 1337;
var server = app.listen(port, function () {
	const host = server.address().address;
	const port = server.address().port;
	console.log(`Example app listening at http://${host}:${port}`);

});
