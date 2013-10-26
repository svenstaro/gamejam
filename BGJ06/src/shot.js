var Shot = Class.create(Entity, {
    className: "Shot",
    initialize: function($super, tank) {
        $super();

        this.velocity = tank.getBarrelDirection().multiplyScalar(5).add(tank.velocity);
        this.node = new THREE.Object3D();
        this.node.position = tank.node.position.clone();
        this.node.position.y = 0.01;

        this.timeLeft = 5;
    },

    onAdd: function(scene) {
        // create root node
        scene.add(this.node);

        var material = new THREE.MeshLambertMaterial({color: 0xFFFF00});
        this.body = new THREE.Mesh(new THREE.SphereGeometry(0.02), material);
        this.body.castShadow = true;
        this.node.add(this.body);

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
            console.log(e.className);
            if(e.className == "Enemy") {
                self.die();
                e.die();
            }
        });
        console.log();
    }
});
