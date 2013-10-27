var Enemy = Class.create(Entity, {
    className: "Enemy",
    initialize: function($super, posX, posZ) {
        $super();
        this.velocity = new THREE.Vector3(0, 0, 0);
        this.acceleration = 20;
        this.damping = 20;
        this.health = 3;
        this.health_blink = 0.0;

        // create the mesh
        var color = rainbow_color[THREE.Math.randInt(0, 6)];
        this.color = color;
        var material = new THREE.MeshLambertMaterial({color: this.color});
        this.mesh = new THREE.Mesh(new THREE.CubeGeometry(0.2, 0.3, 0.2), material);
        this.mesh.position.x = posX;
        this.mesh.position.z = posZ;
        this.mesh.castShadow = true;
        this.mesh.receiveShadow = true;
    },

    onAdd: function(scene) {
        scene.add(this.mesh);
    },

    onRemove: function(scene) {
        scene.remove(this.mesh);
    },

    onTakeHit: function() {
        this.mesh.material.color = new THREE.Color("white");
    },

    update: function(dt) {
        this.setBox(this.mesh.position, new THREE.Vector3(0.2, 1, 0.2));
        this.mesh.material.color.lerp(this.color, this.health_blink)

        if(!this.color.equals(this.mesh.material.color) && this.health_blink < 1.0) {
            this.health_blink += dt;
        }
        else {
            this.health_blink = 0.0;
            this.mesh.material.color = this.color;
        }

        var tank = this.game.scene.getObjectByName("tank");

        var vec = tank.position.clone().sub(this.mesh.position);
        var angle = -Math.atan2(vec.z, vec.x);

        vec.normalize();

        this.velocity.add(vec.multiplyScalar(dt * this.acceleration));

        this.velocity.multiplyScalar(Math.max(0, 1 - this.damping * dt));
        this.mesh.position.add(this.velocity.clone().multiplyScalar(dt));
    }
});
