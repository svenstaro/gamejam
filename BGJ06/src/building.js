var Building = Class.create(Entity, {
    initialize: function(posX, posZ) {
        this.height = THREE.Math.randInt(1,5);
        //shape
        this.geometry = new THREE.CubeGeometry(1, this.height, 1);
        //color
        this.material = new THREE.MeshLambertMaterial(0x999966);
        //this.mesh = new THREE.Mesh(this.geometry, new THREE.MeshLambertMaterial( { envMap: THREE.ImageUtils.loadTexture( 'textures/metal.jpg', new THREE.SphericalReflectionMapping() ), overdraw: true } ));
        this.mesh = new THREE.Mesh(this.geometry, this.material);
        this.mesh.position.x = posX;
        this.mesh.position.y += this.height / 2
        this.mesh.position.z = posZ;
        this.mesh.castShadow = true;
        this.mesh.receiveShadow = true;
    },

    onAdd: function(scene) {
        scene.add(this.mesh);
    },

    update: function(dt) {
        //this.sphere.rotation.x += 1 * delta;
        //this.sphere.rotation.y += 1 * delta;
    }
});