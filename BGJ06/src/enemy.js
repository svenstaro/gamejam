var Enemy = Class.create(Entity, {
    className: "Enemy",
    initialize: function($super, posX, posZ) {
        $super();
        this.speed = 1;

        // create the mesh
        var material = new THREE.MeshLambertMaterial({color: 0x0FF0000});
        this.mesh = new THREE.Mesh(new THREE.SphereGeometry(0.5, 16, 16), material);
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

    update: function(dt) {
        this.setBox(this.mesh.position, new THREE.Vector3(1, 1, 1));
    }
});
