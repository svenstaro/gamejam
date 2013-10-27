var Entity = Class.create({
    className: "Entity",
    initialize: function() {
        this.game = null;
        this.box = new THREE.Box3();
        this.health = 0;
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

    takeHit: function() {
        if(this.health >= 1) {
            this.health -= 1;
            this.onTakeHit();
        }
        else {
            this.die();
        }
    },

    onAdd: function(scene) {},
    onRemove: function(scene) {},
    onTakeHit: function() {},
    update: function(dt) {}
});
