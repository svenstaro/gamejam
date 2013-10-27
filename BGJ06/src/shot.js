var Shot = Class.create(Entity, {
    className: "Shot",
    initialize: function($super, tank) {
        $super();

        this.velocity = tank.getBarrelDirection().multiplyScalar(5).add(tank.velocity);
        this.node = new THREE.Object3D();
        this.node.position = tank.node.position.clone();
        this.node.position.y = 0.01;

        this.timeLeft = 5;

        //sound of shooting
        this.shootingSound = new Sound("simpleShot");
        this.shootingSound.play();
        this.shootingSound.volume(0.2);
    },

    onAdd: function(scene) {
        // create root node
        scene.add(this.node);

        var material = new THREE.MeshLambertMaterial({color: 0xFFFF00});
        this.body = new THREE.Mesh(new THREE.SphereGeometry(0.02), material);
        this.node.add(this.body);

        // this.engine = new ParticleEngine();
        var smoke =
        {
            positionStyle    : Type.CUBE,
            positionBase     : this.node.position,
            positionSpread   : new THREE.Vector3(0, 0, 0),

            velocityStyle    : Type.CUBE,
            velocityBase     : this.velocity,
            velocitySpread   : new THREE.Vector3(0, 0, 0), 
            accelerationBase : new THREE.Vector3(0, 0, 0),

            particleTexture : THREE.ImageUtils.loadTexture('data/smokeparticle.png'),

            angleBase               : 0,
            angleSpread             : 720,
            angleVelocityBase       : 0,
            angleVelocitySpread     : 720,
            
            sizeTween    : new Tween([0.0, 0.0], [0.1, 0.1]),
            opacityTween : new Tween([0.8, 2], [0.5, 0]),
            colorTween   : new Tween([0.4, 1], [new THREE.Vector3(0,0,0.2), new THREE.Vector3(0, 0, 0.5)]),

            particlesPerSecond : 100,
            particleDeathAge   : 2,
            emitterDeathAge    : 20
        }
        // this.engine.setValues(smoke);
        // this.engine.initialize(scene);
    },

    onRemove: function(scene) {
        scene.remove(this.node);
    },

    update: function(dt) {
        this.node.position.add(this.velocity.clone().multiplyScalar(dt));
        this.timeLeft -= dt;
        if(this.timeLeft <= 0) {
            this.die();
            return;
        }

        // update box
        this.setBox(this.node.position, new THREE.Vector3(0.01, 0.01, 0.01));

        // check collision
        var self = this;
        this.game.entities.forEach(function(e) {
            if(e == self) return;
            if(!self.collidesWith(e)) return;
            if(e.className == "Building") {
                self.die();
            }
            if(e.className == "Enemy") {
                self.die();
                e.die();
            }
        });

        // this.engine.update(dt);
    }
});
