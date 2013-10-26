var Shot = Class.create(Entity, {
    initialize: function(tank) {
        this.velocity = tank.getBarrelDirection().multiplyScalar(5);
        this.node = new THREE.Object3D();
        this.node.position = tank.node.position.clone();
        this.node.position.y = 0.2;

        this.timeLeft = 5;
    },

    onAdd: function(scene) {
        console.log("hi");
        // create root node
        scene.add(this.node);

        var material = new THREE.MeshLambertMaterial({color: 0xFFFF00});
        this.body = new THREE.Mesh(new THREE.SphereGeometry(0.02), material);
        this.node.add(this.body);

    },

    onRemove: function(scene) {
        scene.remove(this.node);
    },

    update: function(dt) {
        this.node.position.add(this.velocity.clone().multiplyScalar(dt));
        this.timeLeft -= dt;
        if(this.timeLeft <= 0) {
            this.game.removeEntity(this);
        }

    }
});
