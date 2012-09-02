MainState = gamvas.State.extend({
    levelWidth: 0,
    levelHeight: 0,

    init: function() {
        MUSIC = new Audio("snd/One-eyed Maestro.ogg");

        gamvas.physics.pixelsPerMeter = 32;

        // disable object sleeping (third parameter is false)
        var w = gamvas.physics.resetWorld(0, 30, false);

        this.camera.setPosition(this.dimension.w / 2, this.dimension.h / 2);
        this.clearColor = "#222";

        this.player = new Player("player", this.dimension.w / 2, 100);
        this.addActor(this.player);
        this.keysPressed = {};
        
        loadLevel(this, "levels/level2.json");
        this.levelname = new LevelName("The level that makes you understand the basics of running and such.");
        this.addActor(this.levelname);

        this.addActor(new DecoGear("gear1", 200, 200, 0, 0.3));
        this.addActor(new DecoGear("gear2", 323, 200, 0, -0.3));
        this.addActor(new DecoGear("gear3", 365, 315, 0.08, 0.3));
        this.addActor(new DecoGear("gear4", 488, 315, 0.08, -0.3));

        gamvas.config.preventKeyEvents = false;
        gamvas.config.preventMouseEvents = false;
    },

    draw: function(t) {        
        var d = this.dimension;
        this.camera.position.x = Math.min(this.levelWidth  - d.w / 2, Math.max(this.player.position.x, d.w / 2));
        this.camera.position.y = Math.min(this.levelHeight - d.h / 2, Math.max(this.player.position.y, d.h / 2));
        
        // gamvas.physics.drawDebug();

        if(DEBUG !== true) {
            MUSIC.play();
        }
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

MenuState = gamvas.State.extend({
    init: function() {
        MUSIC = new Audio("snd/clocksong1.ogg");

        this.addActor(new MegaGear("gear1", 250, 250, 0, 0.1, 1.5));
        this.addActor(new MegaGear("gear2", 300, -150, 0, -0.1, 1));
        this.addActor(new TextActor("title", "Airy Viktor", -300, -200, 32, "#999"));
        this.addActor(new Viktor("viktor", 80, -50));

        // this is shown at pseudoState === 0
        this.addActor(new TextActor("start", "start", -300, -100, 16, "#999"));
        this.addActor(new TextActor("instructions", "instructions", -300, 0, 16, "#999"));
        this.addActor(new TextActor("credits", "credits", -300, 100, 16, "#999"));
        this.addActor(new TextActor("quit", "quit", -300, 200, 16, "#999"));

        // this is shown at pseudoState === 1
        this.addActor(new TextActor("instructions_title", "Instructions", -300, -150, 24, "#bbb"));
        this.addActor(new TextActor("instructions_line1", "You are Viktor, a dapper gentleman.", -300, -100, 16, "#999"));
        this.addActor(new TextActor("instructions_line2", "You are trapped in a hideous steam machine.", -300, -50, 16, "#999"));
        this.addActor(new TextActor("instructions_line3", "It is of utmost importance that you get home in time for tea.", -300, 0, 16, "#999"));
        this.addActor(new TextActor("instructions_line4", "You are equipped with a steam weapon.", -300, 50, 16, "#999"));

        // this is shown at pseudoState === 2
        this.addActor(new TextActor("credits_title", "Credits", -300, -150, 24, "#bbb"));
        this.addActor(new TextActor("credits_line2", "Paul Bienkowski", -300, -100, 24, "#999"));
        this.addActor(new TextActor("credits_line1", "Sascha Graeff", -300, -50, 24, "#999"));
        this.addActor(new TextActor("credits_line3", "Sven-Hendrik Haase", -300, 0, 24, "#999"));
        this.addActor(new TextActor("credits_line4", "Janina Matz", -300, 50, 24, "#999"));

        // this is shown at pseudoState === 3
        this.addActor(new TextActor("quit_line1", "This is a web game, good chap.", -300, -100, 16, "#999"));
        this.addActor(new TextActor("quit_line2", "Why not just close the tab?", -300, 0, 16, "#999"));

        this.currentMenuEntry = 0;
        this.pseudoState = 0;
    },

    draw: function() {
        for(var actor in this.actors) {
            this.actors[actor].setActive(false);
        }

        this.actors.gear1._isActive = true;
        this.actors.gear2._isActive = true;
        this.actors.title._isActive = true;
        this.actors.viktor._isActive = true;

        // pseudoState 0 is normal main menu
        // pseudoState 1 is instructions screen
        // pseudoState 2 is credits screen
        // pseudoState 3 is quit screen
        if(this.pseudoState === 0) {
            this.actors.start._isActive = true;
            this.actors.instructions._isActive = true;
            this.actors.credits._isActive = true;
            this.actors.quit._isActive = true;

            for(var actor in this.actors) {
                this.actors[actor].color = "#999";

                if(this.currentMenuEntry === 0) {
                    this.actors.start.color = "#fff";
                }
            }
            if(this.currentMenuEntry === 1) {
                this.actors.instructions.color = "#fff";
            }
            if(this.currentMenuEntry === 2) {
                this.actors.credits.color = "#fff";
            }
            if(this.currentMenuEntry === 3) {
                this.actors.quit.color = "#fff";
            }

        } else if(this.pseudoState === 1) {
            this.actors.instructions_title._isActive = true;
            this.actors.instructions_line1._isActive = true;
            this.actors.instructions_line2._isActive = true;
            this.actors.instructions_line3._isActive = true;
            this.actors.instructions_line4._isActive = true;
        } else if(this.pseudoState === 2) {
            this.actors.credits_title._isActive = true;
            this.actors.credits_line1._isActive = true;
            this.actors.credits_line2._isActive = true;
            this.actors.credits_line3._isActive = true;
            this.actors.credits_line4._isActive = true;
        } else if(this.pseudoState === 3) {
            this.actors.quit_line1._isActive = true;
            this.actors.quit_line2._isActive = true;
        }


        if(DEBUG !== true) {
            MUSIC.play();
        }
    },

    onKeyUp: function(k, c, e) {
    },

    onKeyPushedDown: function(k, c, e) {
        if(k == gamvas.key.R) {
            document.location.reload(true);
        }
    },

    onKeyDown: function(k, c, e) {
        if(k == gamvas.key.UP) {
            if(this.currentMenuEntry > 0) {
                this.currentMenuEntry -= 1;
            }
        }
        if(k == gamvas.key.DOWN) {
            if(this.currentMenuEntry < 3) {
                this.currentMenuEntry += 1;
            }
        }

        if(k == gamvas.key.ESCAPE) {
            this.pseudoState = 0;
        }

        if(k == gamvas.key.RETURN) {
            if(this.currentMenuEntry === 0) {
                MUSIC.pause();
                gamvas.state.setState('MainState');
            } else if(this.currentMenuEntry === 1) {
                this.pseudoState = 1;
            } else if(this.currentMenuEntry === 2) {
                this.pseudoState = 2;
            } else if(this.currentMenuEntry === 3) {
                this.pseudoState = 3;
            }
        }
    }
});

gamvas.event.addOnLoad(function() {
    gamvas.state.addState(new MenuState('MenuState'));
    gamvas.state.addState(new MainState('MainState'));
    gamvas.start('gameCanvas', true);
    if(DEBUG === true) {
        gamvas.state.setState('MainState');
    } else {
        gamvas.state.setState('MenuState');
    }
});
