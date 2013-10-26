var Entity = Class.create({
    className: "Entity",
    initialize: function() {
        this.game = null;
        this.box = new THREE.Box3();
    },

    setBox: function(position, size) {
        var halfsize = size.clone().multiplyScalar(0.5);
        this.box.set(position.clone().sub(halfsize), position.clone().add(halfsize));
    },

    collidesWith: function(entity) {
        return this.box && entity.box && this.box.isIntersectionBox(entity.box);
    },

    die: function() {
        this.game.removeEntity(this);
    },

    onAdd: function(scene) {},
    onRemove: function(scene) {},
    update: function(dt) {}
});
