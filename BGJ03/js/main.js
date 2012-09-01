MainState = gamvas.State.extend({
    init: function() {
        gamvas.physics.pixelsPerMeter = 32;

        // disable object sleeping (third parameter is false)
        var w = gamvas.physics.resetWorld(0, 20, false);

        this.camera.setPosition(this.dimension.w / 2, this.dimension.h / 2);

        this.player = new Player("player", this.dimension.w / 2, 100);
        this.addActor(this.player);
        this.keysPressed = {};
        
        var temp_state = this;
        $.getJSON('levels/test.json', function(json) {
            var width = json.width;
            var height = json.height;
            var data = json.layers[0].data;
            for(var y = 0; y < height; y++) {
                for(var x = 0; x < width; x++) {
                    print(data[x+width*y], "true");
                    temp_state.addActor(new Tile("tile-" + x + "-" + y, x, y, 1, 1));
                }
                print("");
            }
        });

        //for(var x = 0; x < 25; x++) {
        //    for(var y = 0; y < 18; y++) {
        //        if(x == 0 || x == 24 || y == 0 || y == 17 ||
        //            (x > 20 && y > 15) ||
        //            (x + 18 - y < 10) ||
        //            (x >= 16 && x < 20 && y == 13)
        //            ) {
        //            this.addActor(new Tile("tile-" + x + "-" + y, x, y, 1, 1));
        //        }
        //    }
        //}

        gamvas.config.preventKeyEvents = false;
        gamvas.config.preventMouseEvents = false;
    },

    draw: function(t) {
        //gamvas.physics.drawDebug();
        //this.c.fillStyle = '#fff';
        //this.c.font = 'bold 20px sans-serif';
		//this.c.fillText("This level is hard as hell.", 0, 0);
    },

    onMouseDown: function(b, x, y) {
        if (b == gamvas.mouse.LEFT) {
            var worldMouse = this.camera.toWorld(x, y);
        }
    },

    onKeyUp: function(k, c, e) {
        if (k in this.keysPressed) {
            this.keysPressed[k] = false;
        }
    },

    onKeyPushedDown: function(k, c, e) {
        if(k == gamvas.key.R) {
            document.location.reload(true);
        } else if(k == gamvas.key.UP || k == gamvas.key.W) {
            this.player.jump();
        }    },

    onKeyDown: function(k, c, e) {
        if (!(k in this.keysPressed) || !this.keysPressed[k]) {
            this.keysPressed[k] = true;
            this.onKeyPushedDown(k, c, e);
        }
    }
});

gamvas.event.addOnLoad(function() {
    gamvas.state.addState(new MainState('MainState'));
    gamvas.start('gameCanvas', true);
});
