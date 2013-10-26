var Tank = Class.create(Entity, {
    initialize: function(game) {
        this.speed = 5;

        // create root node
        this.node = new THREE.Object3D();
        this.node.name = "tank";
        game.scene.add(this.node);

        // create the mesh
        var material = new THREE.MeshLambertMaterial({color: 0x00FF00});
        this.mesh = new THREE.Mesh(new THREE.SphereGeometry(0.5, 16, 16), material);
        this.node.add(this.mesh);

        // create a point light
        this.light = new THREE.PointLight(0xFFFFFF);
        this.light.position.set(0, 2, 0);
        this.node.add(this.light);
    },

    update: function(dt) {
        if(this.game.keyboard.pressed("d"))
            this.node.position.x += dt * this.speed;
        if(this.game.keyboard.pressed("a"))
            this.node.position.x -= dt * this.speed;
        if(this.game.keyboard.pressed("w"))
            this.node.position.z -= dt * this.speed;
        if(this.game.keyboard.pressed("s"))
            this.node.position.z += dt * this.speed;
    }
});
