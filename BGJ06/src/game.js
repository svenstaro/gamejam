var Game = Class.create({
    initialize: function() {
        this.width = window.innerWidth;
        this.height = window.innerHeight;
        this.entities = [];
        this.mouse = new THREE.Vector2();

        // set some camera attributes
        this.view_angle = 80;
        this.aspect = this.width / this.height;
        this.near = 0.001;
        this.far = 10;

        // create a WebGL renderer, camera
        // and a scene
        this.renderer = new THREE.WebGLRenderer({antialias: true});
        this.scene = new THREE.Scene();
        this.camera = new THREE.PerspectiveCamera(this.view_angle, this.aspect, this.near, this.far);
        this.camera.position.y = 3.5;
        this.camera.rotation.x = -Math.PI/2;

        // start the renderer
        this.renderer.setSize(this.width, this.height);

        // attach the render-supplied DOM element
        document.body.appendChild(this.renderer.domElement);


        this.tank = new Tank();
        this.addEntity(this.tank);

        // setup ground
        var material = new THREE.MeshLambertMaterial({color: 0xFF0000});
        this.ground = new THREE.Mesh(new THREE.PlaneGeometry(100, 100), material);
        this.ground.rotation.x = -Math.PI/2;
        this.scene.add(this.ground);

        // setup grid
        this.grid = new THREE.GridHelper(100, 1);
        this.grid.position.y = 0.01;
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

    worldMouse: function() {
        // var projector = new THREE.Projector();
        // var vector = new THREE.Vector3( this.mouse.x, this.mouse.y, 1 );
        // projector.unprojectVector(vector, this.camera);

        // var ray = new THREE.Raycaster(this.camera.position, vector.sub(this.camera.position).normalize());
        // var intersects = ray.intersectObject(this.grid);

        var i = new THREE.Projector().pickingRay(new THREE.Vector3(this.mouse.x, this.mouse.y, 0), this.camera).intersectObject(this.ground);
        return i.length ? i[0].point : new THREE.Vector3();
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

    removeEntity: function(entity) {
        var index = this.entities.indexOf(entity);
        if (index < 0) return;
        entity.onRemove(this.scene);
        this.entities.splice(index, 1);
        entity.game = null;
    },

    render: function() {
        this.renderer.render(this.scene, this.camera);
    }
});
