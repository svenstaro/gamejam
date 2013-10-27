var Building = Class.create(Entity, {
    className: "Building",
    initialize: function($super, posX, posZ) {
        $super();

        // shape
        this.height = THREE.Math.randInt(1,4);
        this.geometry = new THREE.CubeGeometry(1, this.height, 1);
        
        // color
        var material = new THREE.MeshPhongMaterial({color: "white"});
        this.mesh = new THREE.Mesh(this.geometry, material);
        this.mesh.position.x = posX;
        this.mesh.position.y += this.height / 2
        this.mesh.position.z = posZ;
        this.mesh.castShadow = true;
        this.mesh.receiveShadow = true;

        // update box
        this.setBox(this.mesh.position, new THREE.Vector3(1, this.height, 1));
    },

    onAdd: function(scene) {
        scene.add(this.mesh);
    },

    update: function(dt) {
    }
});
