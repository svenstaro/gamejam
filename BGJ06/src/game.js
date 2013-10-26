var Game = Class.create({
    initialize: function() {
        // set the scene size
        this.width = window.innerWidth;
        this.height = window.innerHeight;

        // set some camera attributes
        this.view_angle = 80;
        this.aspect = this.width / this.height;
        this.near = 0.1;
        this.far = 10000;

        // create a WebGL renderer, camera
        // and a scene
        this.renderer = new THREE.WebGLRenderer();
        this.camera = new THREE.PerspectiveCamera(this.view_angle, this.aspect, this.near, this.far);
        this.scene = new THREE.Scene();

        // the camera starts at 0,0,0 so pull it back
        this.camera.position.z = 300;

        // start the renderer
        this.renderer.setSize(this.width, this.height);

        // attach the render-supplied DOM element
        document.body.appendChild(this.renderer.domElement);

        // create the sphere's material
        var sphereMaterial = new THREE.MeshLambertMaterial(
        {
            color: 0xCC0000
        });

        // set up the sphere vars
        var radius = 50, segments = 16, rings = 16;

        // create a new mesh with sphere geometry -
        // we will cover the sphereMaterial next!
        this.sphere = new THREE.Mesh(
           new THREE.SphereGeometry(radius, segments, rings),
           sphereMaterial);

        // add the sphere to the scene
        this.scene.add(this.sphere);

        // and the camera
        this.scene.add(this.camera);

        // create a point light
        this.pointLight = new THREE.PointLight(0xFFFFFF);

        // set its position
        this.pointLight.position.x = 10;
        this.pointLight.position.y = 50;
        this.pointLight.position.z = 130;

        // add to the scene
        this.scene.add(this.pointLight);
    },

    update: function(delta) {
        this.sphere.rotation.x += 1 * delta;
        this.sphere.rotation.y += 1 * delta;
    },

    render: function() {
        this.renderer.render(this.scene, this.camera);
    }
});
