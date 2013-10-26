var Tank = Class.create(Entity, {
    initialize: function($super, game) {
        $super(game);
        var material = new THREE.MeshLambertMaterial({color: 0x00FF00});
        this.mesh = new THREE.Mesh(new THREE.SphereGeometry(0.5, 16, 16), material);
        game.scene.add(this.mesh);
    },
    update: function(dt) {
        if(this.game.keyboard.pressed("w")) {
            this.mesh.position.x += dt;
        }
    }
});
