let scene,
    camera,
    renderer,
    cube,
    fungus;

function init () {
    scene = new THREE.Scene();
    camera = new THREE.PerspectiveCamera( 75, window.innerWidth/window.innerHeight, 0.1, 1000 );
    renderer = new THREE.WebGLRenderer();
    renderer.setSize( window.innerWidth, window.innerHeight );
    document.body.appendChild( renderer.domElement );

    fungus = new Module.Fungus();

    const geometry = new THREE.BoxGeometry( 1, 1, 1 );
    const material = new THREE.MeshBasicMaterial( { color: 0x00ff00 } );
    cube = new THREE.Mesh( geometry, material );
    scene.add( cube );

    camera.position.z = 5;

    animate();
}

function animate() {
    requestAnimationFrame( animate );

    fungus.update();

    cube.rotation.x += 0.1;
    cube.rotation.y += 0.1;

    renderer.render( scene, camera );
};

// to deal with asynchronous bs from emscripten @ runtime
var Module = {
    onRuntimeInitialized: function() {
        init();
    }
};
