var City = Class.create(Entity, {
    initialize: function(width, height) {
        this.width = width - width/2;
        this.height = height - height/2;
        this.listOfEnemies = [];

        //background sound
        this.backgroundMusic = new Sound("backgroundSound");
        this.backgroundMusic.loop(); 
        this.backgroundMusic.play();
        this.backgroundMusic.volume(0.45);
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
    	   		}
    	    }
    	}
    },

    update: function(dt) {
        var tank = this.game.scene.getObjectByName("tank");
    	var newListOfEnemies = [];

        var chance = THREE.Math.randInt(0, 50);
        if (chance == 0) {
            var enemy = new Enemy(tank.position.x + THREE.Math.randInt(-5, 5),
                                  tank.position.z + THREE.Math.randInt(-5, 5));
        	this.game.addEntity(enemy);
            this.listOfEnemies.push(enemy);
        }

        for (var i = 0; i < this.listOfEnemies.length; i++) {
            if (Math.sqrt(Math.pow((tank.position.x - this.listOfEnemies[i].mesh.position.x), 2)
                + Math.pow( (tank.position.z - this.listOfEnemies[i].mesh.position.z), 2)) < 10) {
                newListOfEnemies.push(this.listOfEnemies[i]);
            }
            else {
                this.game.removeEntity(this.listOfEnemies[i]);
            }
        }

        this.listOfEnemies = newListOfEnemies;
    }
});
