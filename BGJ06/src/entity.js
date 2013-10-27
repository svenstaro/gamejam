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
        this.onDeath();
        this.game.removeEntity(this);
    },

    takeHit: function() {
        if(this.health > 0) {
            this.health -= 1;
            this.onTakeHit();
        }
        else {
            if(this.className != "Tank")
                this.die();
        }
    },

    randomColor: function() {},
    
    onAdd: function(scene) {},
    onRemove: function(scene) {},
    onTakeHit: function() {},
    onDeath: function() {},
    update: function(dt) {}
});
