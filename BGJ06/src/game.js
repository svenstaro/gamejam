var Game = Class.create({
    className: "Game",
    initialize: function() {
        this.width = window.innerWidth;
        this.height = window.innerHeight;
        this.entities = [];
        this.mouse = new THREE.Vector2();
        
        this.score = 0;

        this.lastParty = 0.0;

        // set some camera attributes
        this.view_angle = 80;
        this.aspect = this.width / this.height;
        this.near = 0.001;
        this.far = 10;

        // create a WebGL renderer, camera
        // and a scene
        this.renderer = new THREE.WebGLRenderer({antialias: true});
        this.scene = new THREE.Scene();
        this.scene.fog = new THREE.Fog(0x222222, 1, 10);
        this.camera = new THREE.PerspectiveCamera(this.view_angle, this.aspect, this.near, this.far);
        this.camera.position.y = 3.5;
        this.camera.rotation.x = -Math.PI/2;

        // start the renderer
        this.renderer.setSize(this.width, this.height);

        this.renderer.shadowMapEnabled = true;
        this.renderer.shadowMapSoft = true;

        // attach the render-supplied DOM element
        document.body.appendChild(this.renderer.domElement);

        this.tank = new Tank();
        this.addEntity(this.tank);

        // setup ground
        var material = new THREE.MeshPhongMaterial({color: 0x333333});
        this.ground = new THREE.Mesh(new THREE.PlaneGeometry(100, 100), material);
        this.ground.rotation.x = -Math.PI/2;
        this.ground.receiveShadow = true;
        this.scene.add(this.ground);

        // setup grid
        this.grid = new THREE.GridHelper(100, 1);
        this.grid.name = "grid";
        this.grid.position.y = 0.01;
        this.grid.position.x = 0.5;
        this.grid.position.z = 0.5;

        // and the camera
        this.scene.add(this.camera);

        this.keyboard = new THREEx.KeyboardState();

        this.sun = new THREE.DirectionalLight(0xDDAAAA, 1);
        this.sun.position.set(300, 500, 100);
        this.sun.target.position.set(0, 0, 0);
        this.sun.castShadow = true;
        this.sun.shadowCameraNear = 0.1;
        this.scene.add(this.sun);

        //health sphere
        this.radius = 0.1;
        this.segments = 16;
        this.rings = 16;
        this.sphereMaterial = new THREE.MeshLambertMaterial({color: rainbow_color[6]});
        this.geometry = new THREE.SphereGeometry(this.radius, this.segments, this.rings);
        this.healthSphere = new THREE.Mesh(this.geometry, this.sphereMaterial);
        this.healthSphere.position.y = 1;
        this.scene.add(this.healthSphere);
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
        this.lastParty += dt;
        var self = this;
        this.entities.forEach(function(entity) {
            entity.update(dt);

            // party mode on death
            if(self.tank.health <= 0) {
                if(self.lastParty > 0.2) {                
                    var color = rainbow_color[THREE.Math.randInt(0, 6)];
                    entity.randomColor();
                    self.healthSphere.material.color = color;
                    self.sun.color = color;
                    self.ground.material.color = color;
                    self.lastParty = 0;
                }
            }
        });

        this.sun.shadowCameraVisible = debug;

        if (debug)
            this.scene.add(this.grid);
        else
            this.removeEntity(this.scene.getObjectByName("grid"));
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
