var Game = Class.create({
    initialize: function() {
        this.width = window.innerWidth;
        this.height = window.innerHeight;
        this.entities = [];

        // set some camera attributes
        this.view_angle = 80;
        this.aspect = this.width / this.height;
        this.near = 0.1;
        this.far = 10000;

        // create a WebGL renderer, camera
        // and a scene
        this.renderer = new THREE.WebGLRenderer();
        this.scene = new THREE.Scene();
        this.camera = new THREE.PerspectiveCamera(this.view_angle, this.aspect, this.near, this.far);
        this.camera.position.y = 10;
        this.camera.rotation.x = -Math.PI/2;

        // start the renderer
        this.renderer.setSize(this.width, this.height);

        // attach the render-supplied DOM element
        document.body.appendChild(this.renderer.domElement);


        this.tank = new Tank(this);
        this.addEntity(this.tank);

        // setup grid
        this.grid = new THREE.GridHelper(100, 1);
        this.grid.position.x = 0.5;
        this.grid.position.z = 0.5;
        this.scene.add(this.grid);

        // and the camera
        this.scene.add(this.camera);

        this.keyboard = new THREEx.KeyboardState();

        this.sun = new THREE.DirectionalLight( 0xCCDDFF, 0.3 );
        this.sun.position.set(0, 1, 0);
        this.scene.add(this.sun);
    },

    update: function(dt) {
        this.entities.forEach(function(entity) {
            entity.update(dt);
        });
    },

    addEntity: function(entity) {
        entity.game = this;
        this.entities.push(entity);
        entity.onAdd(this.scene);
    },

    render: function() {
        this.renderer.render(this.scene, this.camera);
    }
});
