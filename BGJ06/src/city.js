var City = Class.create(Entity, {
    initialize: function(width, height) {
        this.width = width - width/2;
        this.height = height - height/2;

    },

    onAdd: function(scene) {
    	for (var x = -this.width; x < this.width; x++) {
    		for (var y = -this.height; y < this.height; y++) {
    			if (x % 2 == 0 && y % 2 == 0) {
    				var chance = THREE.Math.randInt(0, 1);
    				if (chance == 0) {
    					var building = new Building(x, y);
    	   				this.game.addEntity(building);
    				}
    				else {}
    	   		}
    	    }
    	}
    },

    update: function(dt) {}
});
