var Enemy = Class.create(Entity, {
    initialize: function(posX, posZ) {
        this.speed = 1;

        // create the mesh
        var material = new THREE.MeshLambertMaterial({color: 0x0FF0000});
        this.mesh = new THREE.Mesh(new THREE.SphereGeometry(0.5, 16, 16), material);
        this.mesh.position.x = posX;
        this.mesh.position.z = posZ;

        // create a point light
        this.light = new THREE.PointLight(0xFF0000);
        this.light.position.set(posX, 2, posZ);
    },

    onAdd: function(scene) {
        scene.add(this.mesh);
        scene.add(this.light);
    },

    update: function(dt) {}
});
