var Tank = Class.create(Entity, {
    className: "Tank",
    initialize: function($super) {
        $super();
        this.velocity = new THREE.Vector3(0, 0, 0);
        this.acceleration = 50;
        this.damping = 15;
        this.health = 7;
        this.score = 0;
    },

    onAdd: function(scene) {
        // create root node
        this.node = new THREE.Object3D();
        this.node.name = "tank";
        scene.add(this.node);

        var material = new THREE.MeshLambertMaterial({color: 0x555555});

        // create the body
        this.body = new THREE.Mesh(new THREE.CubeGeometry(0.3, 0.1, 0.2), material);
        this.body.castShadow = true;
        this.node.add(this.body);

        // tower
        var tower = new THREE.Mesh(new THREE.SphereGeometry(0.08), material);
        tower.position.y = 0.1;
        this.node.add(tower);

        // create the barrel
         this.barrelRoot = new THREE.Object3D();
        this.node.add(this.barrelRoot);
        this.barrel = new THREE.Mesh(new THREE.CylinderGeometry(0.03, 0.03, 0.2), material);
        this.barrel.rotation.z = Math.PI/2;
        this.barrel.position.set(0.1, 0.12, 0);
        this.barrelRoot.add(this.barrel);

        // create a point light
        this.light = new THREE.PointLight(0xFFFFFF);
        this.light.position.set(0, 1, 0);
        this.light.distance = 3;
        this.node.add(this.light);

        // create barrel light
        this.barrellight = new THREE.SpotLight(0xFFFFFF, 1, 5);
        this.barrellight.angle = Math.PI/8;
        this.barrellight.castShadow = true;
        this.barrellight.shadowCameraNear = 0.1;
        this.barrellight.shadowCameraFov = 45;
        this.node.add(this.barrellight);
    },

    update: function(dt) {
        var dir = new THREE.Vector3(0, 0, 0);
        if(this.game.keyboard.pressed("d")) dir.x += 1;
        if(this.game.keyboard.pressed("a")) dir.x -= 1;
        if(this.game.keyboard.pressed("w")) dir.z -= 1;
        if(this.game.keyboard.pressed("s")) dir.z += 1;
        dir.normalize();
        this.velocity.add(dir.multiplyScalar(dt * this.acceleration));

        this.body.rotation.y = -Math.atan2(this.velocity.z, this.velocity.x);

        this.velocity.multiplyScalar(Math.max(0, 1 - this.damping * dt));
        this.node.position.add(this.velocity.clone().multiplyScalar(dt));

        var bd = this.getBarrelDirection();
        var br = -Math.atan2(bd.z, bd.x);
        this.barrelRoot.rotation.y = br;

        // update box
        this.setBox(this.node.position, new THREE.Vector3(0.2, 0.1, 0.2));

        //going through buildings prohibited
        var self = this;
        this.game.entities.forEach(function(e) {
            if(e.className == "Building") {
                if(self.collidesWith(e)) {
                    self.node.position.sub(self.velocity.clone().multiplyScalar(dt));
                }
            } 
            if(e.className == "Enemy") { 
                if(self.collidesWith(e)) {
                    if(self.health >= 1) {
                        self.health -= 1;
                        e.die();
                        if(self.health == 0) {
                            self.die();
                        }
                    }
                }
            }
        });

        // barrel light
        var light_vec = new THREE.Vector3(1, 0, 0);
        light_vec.applyAxisAngle(new THREE.Vector3(0, 1, 0), br);
        this.barrellight.target.position.addVectors(this.node.position, light_vec);
        this.barrellight.position.y = 0.2;

        // move camera
        var target = this.node.position.clone();
        target.y = this.game.camera.position.y;
        this.game.camera.position.lerp(target, dt*6);

        this.barrellight.shadowCameraVisible = debug;
    },

    getBarrelDirection: function() {
        return this.game.worldMouse().sub(this.node.position).normalize();
    },

    shoot: function(dt) {
        this.game.addEntity(new Shot(this));
    }
});
