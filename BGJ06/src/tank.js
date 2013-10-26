var Tank = Class.create(Entity, {
    initialize: function($super, game) {
        $super(game);
        var material = new THREE.MeshLambertMaterial({color: 0x00FF00});
        this.mesh = new THREE.Mesh(new THREE.SphereGeometry(0.5, 16, 16), material);
        game.scene.add(this.mesh);

        this.speed = 5;
    },
    update: function(dt) {
        if(this.game.keyboard.pressed("d"))
            this.mesh.position.x += dt * this.speed;
        if(this.game.keyboard.pressed("a"))
            this.mesh.position.x -= dt * this.speed;
        if(this.game.keyboard.pressed("w"))
            this.mesh.position.z -= dt * this.speed;
        if(this.game.keyboard.pressed("s"))
            this.mesh.position.z += dt * this.speed;
    }
});
