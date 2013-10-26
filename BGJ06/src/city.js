var City = Class.create(Entity, {
    initialize: function(width, height) {
        this.width = width - width/2;
        this.height = height - height/2;
        this.listOfEnemies = [];

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

    update: function(dt) {
        var tank = this.game.scene.getObjectByName("tank");
    	var newListOfEnemies = [];

        var chance = THREE.Math.randInt(0, 100);
        if (chance == 0 && this.listOfEnemies.length < 10) {
            var enemy = new Enemy(tank.position.x + THREE.Math.randInt(-3, 3),
                                  tank.position.z + THREE.Math.randInt(-3, 3));
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

        /*}
        for (var item = 0; item < this.listOfEnemies.length; item++) {
            for (var a = 0; a < newListOfEnemies.length; a++) {
                this.listOfEnemies.splice(item, 1);
                this.game.removeEntity(item);
                newListOfEnemies.splice(item,1);

            }
        }*/
            
        //var distance = Math.sqrt(Math.pow( (tank.position.x - enemy.mesh.position.x), 2) 
          ///                  + Math.pow( (tank.position.z - enemy.mesh.position.z), 2));
        
    }
});
