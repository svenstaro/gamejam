var Tank = Class.create(Entity, {
    initialize: function(game) {
        this.velocity = new THREE.Vector3(0, 0, 0);
        this.acceleration = 50;
        this.damping = 15;

        // create root node
        this.node = new THREE.Object3D();
        this.node.name = "tank";
        game.scene.add(this.node);

        var material = new THREE.MeshLambertMaterial({color: 0x555555});
        // create the body
        this.body = new THREE.Mesh(new THREE.CubeGeometry(0.3, 0.1, 0.2), material);
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
        this.light.position.set(0, 2, 0);
        this.node.add(this.light);
    },

    update: function(dt) {
        var dir = new THREE.Vector3(0, 0, 0);
        if(this.game.keyboard.pressed("d")) dir.x += 1;
        if(this.game.keyboard.pressed("a")) dir.x -= 1;
        if(this.game.keyboard.pressed("w")) dir.z -= 1;
        if(this.game.keyboard.pressed("s")) dir.z += 1;
        this.velocity.add(dir.multiplyScalar(dt * this.acceleration));

        this.velocity.multiplyScalar(Math.max(0, 1 - this.damping * dt));
        this.node.position.add(this.velocity.clone().multiplyScalar(dt));

        //console.log(this.game.worldMouse());
        var mouseDiff = this.game.worldMouse().sub(this.node.position);
        this.barrelRoot.rotation.y = -Math.atan2(mouseDiff.z, mouseDiff.x);
    }
});
