let scene,
    camera,
    renderer,
    controls,
	numPoints,
	points,
    fungus;

function init () {
    scene = new THREE.Scene();
    camera = new THREE.PerspectiveCamera( 75, window.innerWidth/window.innerHeight, 0.1, 1000 );
    controls = new THREE.OrbitControls( camera );

    renderer = new THREE.WebGLRenderer();
    renderer.setSize( window.innerWidth, window.innerHeight );
    document.body.appendChild( renderer.domElement );

    fungus = new Module.Fungus();

	var positions = new Float32Array( 1000999 * 3 );
	var geometry = new THREE.BufferGeometry();
	geometry.addAttribute( 'position', new THREE.BufferAttribute( positions, 3 ) );
	var material = new THREE.PointsMaterial( { color: 0xff0000, sizeAttenuation: false } );

	points = new THREE.Points( geometry, material );
	scene.add( points );

	numPoints = 0;

    camera.position.z = 50;

    animate();
}

function animate() {
    controls.update();
    requestAnimationFrame( animate );

    fungus.update();
    var lastPoints = fungus.getNewPoints();
	var positions = points.geometry.attributes.position;
	for (let i = 0; i < lastPoints.size(); ++i) {
		var lastPoint = lastPoints.get(i);
		positions.array[ numPoints * 3 + 0 ] = lastPoint[0];
		positions.array[ numPoints * 3 + 1 ] = lastPoint[1];
		positions.array[ numPoints * 3 + 2 ] = lastPoint[2];
		numPoints++;
	}
	console.log(positions);

	positions.needsUpdate = true;

    renderer.render( scene, camera );
};

// to deal with asynchronous bs from emscripten @ runtime
var Module = {
    onRuntimeInitialized: function() {
        init();
    }
};
