var scene; 	
var camera; 	
var renderer;
var mesh = [];
var geom = [];

var width = 350.0;
var height = 250.0;
aspect = width / height;


function createThreeJSview(inputForm)
{
    scene       = new THREE.Scene();
    camera      = new THREE.OrthographicCamera(-5.0*aspect, 5.0*aspect, 5.0, -5.0, 0.1, 1000);
    renderer 	= new THREE.WebGLRenderer();
    renderer.setSize( width, height );
    inputForm.appendChild(renderer.domElement);   
}

function createIbeamModel(inputForm)
{
    createThreeJSview(inputForm);
    
    var material = new THREE.MeshNormalMaterial();
	var L = 1;
			
	var g1 = new THREE.BoxGeometry(L,L,L);	
	var m1 = new THREE.Mesh(g1, material);	

	var g2 = new THREE.BoxGeometry(L,L,L);	
	var m2 = new THREE.Mesh(g2, material);		
	
	var g3 = new THREE.BoxGeometry(L,L,L);	
	var m3 = new THREE.Mesh(g3, material);	
    
    scene.add(m1);
	scene.add(m2);
	scene.add(m3);
	
	geom.push(g1);
	mesh.push(m1);
	geom.push(g2);
	mesh.push(m2);
	geom.push(g3);
	mesh.push(m3);
    
    var H  = 8.0;
	var B  = 6.0; 
	var t1 = 1.0;
	var t2 = 2.0;
	var L  = 1; 
	
	mesh[0].scale.set(B, t1, L); //top
	mesh[1].scale.set(B, t1, L); //bottom
	mesh[2].scale.set(t2, H - 2*t1, L); //bottom
		
	mesh[0].translateY( (H - t1)/2);
	mesh[1].translateY(-(H - t1)/2);
	
	camera.position.z = 100;
	
	var controls = new THREE.OrbitControls(camera, renderer.domElement); 
	
	function animate() {
		requestAnimationFrame(animate);
		//controls.update();
		renderer.render(scene, camera);
	}
		
	animate();
}