MainState = gamvas.State.extend({
    init: function() {
        gamvas.physics.pixelsPerMeter = 32;

        // disable object sleeping (third parameter is false)
        var w = gamvas.physics.resetWorld(0, 20, false);

        this.camera.setPosition(this.dimension.w / 2, this.dimension.h / 2);
        this.clearColor = "#222";

        this.player = new Player("player", this.dimension.w / 2, 100);
        this.addActor(this.player);
        this.keysPressed = {};
        
        loadLevel(this, "levels/test.json");

        this.addActor(new DecoGear("gear1", 200, 200, 0, 0.1));
        this.addActor(new DecoGear("gear2", 323, 200, 0, -0.1));
        this.addActor(new DecoGear("gear3", 365, 315, 0.08, 0.1));
        this.addActor(new DecoGear("gear4", 488, 315, 0.08, -0.1));
        this.addActor(new LevelName("Hard as hell"));

        gamvas.config.preventKeyEvents = false;
        gamvas.config.preventMouseEvents = false;
    },

    draw: function(t) {
        //gamvas.physics.drawDebug();
    },

    onMouseDown: function(b, x, y) {
        if (b == gamvas.mouse.LEFT || b == gamvas.mouse.RIGHT) {
            var fireMode = "primary";
            if(b == gamvas.mouse.RIGHT) fireMode = "secondary";
            this.player.gun.shoot(fireMode);
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
        } else if(isKey(k, JUMP_KEYS)) {
            this.player.jump();
        }    
    },

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
