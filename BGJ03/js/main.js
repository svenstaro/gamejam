MainState = gamvas.State.extend({
    init: function() {
        gamvas.physics.pixelsPerMeter = 32;

        // disable object sleeping (third parameter is false)
        var w = gamvas.physics.resetWorld(0, 9.81, false);

        this.camera.setPosition(this.dimension.x / 2, this.dimension.y / 2);

        this.player = new Player("player", 0, 0);
        this.addActor(this.player);

        for(var x = 0; x < 25; x++) {
            for(var y = 0; y < 18; y++) {
                if(x == 0 || y > 5) {
                    this.addActor(new Tile("tile-" + x + "-" + y, x, y));
                }
            }
        }

        gamvas.config.preventKeyEvents = false;
        gamvas.config.preventMouseEvents = false;
    },

    draw: function(t) {
        //gamvas.physics.drawDebug();
    },

    onMouseDown: function(b, x, y) {
        if (b == gamvas.mouse.LEFT) {
            var worldMouse = this.camera.toWorld(x, y);
        }
    },

    onKeyDown: function(k, c, e) {
        if(k == gamvas.key.R) {
            document.location.reload(true);
        } else if(k == gamvas.key.UP || k == gamvas.key.W) {
            this.player.jump();
        }
    }
});

gamvas.event.addOnLoad(function() {
    gamvas.state.addState(new MainState('MainState'));
    gamvas.start('gameCanvas', true);
});
