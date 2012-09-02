var teatime = false;

MainState = gamvas.State.extend({
    levelWidth: 0,
    levelHeight: 0,
    resetPosition: new gamvas.Vector2D(),
    died: false,
    flashAlpha: 1,
    noclip: 0,
    keys: 0,
    keysLeft: 0,
    gunMode: 0, // 0 = none, 1 = blow, 2 = pipe

    init: function() {
        MUSIC = new Audio("snd/One-eyed Maestro.ogg");

        gamvas.physics.pixelsPerMeter = 32;

        var w = gamvas.physics.resetWorld(0, 30, false);

        //this.camera.setPosition(this.dimension.w / 2, this.dimension.h / 2);
        this.clearColor = "#222";

        this.keysPressed = {};

        gamvas.config.preventKeyEvents = false;
        gamvas.config.preventMouseEvents = false;
        
        this.levelname = new LevelName("Wait");
    },
    
    enter: function() {
        if(DEBUG === true) {
            this.level = 10;
        } else {
            this.level = 0;
        }
        this.scheduleChangeLevel = true;
    },

    resetPlayer: function() {
        this.scheduleChangeLevel = true;
    },

    draw: function(t) {        
        if(this.scheduleChangeLevel) {
            this.changeLevel(this.level);
            this.scheduleChangeLevel = false;
            return;
        }

        if(this.player) {
            var d = this.dimension;
            var h = d.h;
            if(this.levelHeight <= 19 * TILESIZE)
            {
                h = this.levelHeight;
            }
            this.camera.position.x = Math.min(this.levelWidth  - d.w / 2, Math.max(this.player.position.x, d.w / 2));
            this.camera.position.y = Math.min(this.levelHeight - h / 2, Math.max(this.player.position.y, h / 2));
        }

        if(gamvas.mouse.isPressed(gamvas.mouse.LEFT) && this.gunMode >= 1) {
            this.player.gun.shoot("secondary");
        }
        
        if(DEBUG_DRAWS === true) {
            gamvas.physics.drawDebug();
        }

        if(DEBUG !== true) {
            MUSIC.play();
        }

        if(this.died) {
            this.resetPlayer();
            this.died = false;
        }

        this.flashAlpha -= t * 2;

        if(this.flashAlpha > 0) {
            this.c.fillStyle = 'rgba(255, 255, 255, ' + (1 - Math.pow(1 - this.flashAlpha, 2)) + ')';
            this.c.fillRect(this.camera.position.x - this.dimension.w / 2, this.camera.position.y - this.dimension.h / 2, this.dimension.w, this.dimension.h);
        }
    },

    changeLevel: function(level) {
        // this.flashAlpha = 1;
        for(var actor in this.actors) {
            this.removeActor(this.actors[actor]);
        }

        var additionalActors = [];
        
        this.removeCollisions = true;
        
        additionalActors.push(new Background("background", -100, 0));
        
        if(level === 0) {
            this.levelname.text = "The level that makes you understand the basics of running and such. Press WASD or arrow keys to do it.";

            additionalActors.push(new DecoGear("gear1", 700, 450, 0, 0.3));
            additionalActors.push(new DecoGear("gear2", 823, 450, 0, -0.3));
            additionalActors.push(new DecoGear("gear3", 865, 565, 0.08, 0.3));
            additionalActors.push(new DecoGear("gear4", 988, 565, 0.08, -0.3));
            
            loadLevel(this, "levels/level0.json", additionalActors);
        }
        if(level === 1) {
            this.levelname.text = "The level that wants you to think about what you have learned and gives you time for thoughts about tea.";

            additionalActors.push(new Prop("prop1", 500, 100, Math.PI / 2));
            additionalActors.push(new Prop("prop2", 200, 100, -Math.PI / 2));

            loadLevel(this, "levels/level1.json", additionalActors);
        }
        if(level === 2) {
            this.levelname.text = "The level that adds pointy spikes to enhance your personal jumping experience.";

            additionalActors.push(new MegaGear("gear2", 50, -50, 0, -0.1, 1, true));

            loadLevel(this, "levels/level2.json", additionalActors);
        }
        if(level === 3) {
            this.levelname.text = "The third level which ultimatly shows you the importance of key possession.";

            loadLevel(this, "levels/level3.json", additionalActors);
        }
        if(level === 4) {
            this.levelname.text = "You have been granted the use of a terrificly practical device. Use the left mouse button to push things.";

            additionalActors.push(new MegaGear("gear1", 850, 100, 1, 0.1, 2, true));
            additionalActors.push(new MegaGear("gear2", 50, 200, 0, -0.1, 2, true));

            loadLevel(this, "levels/level4.json", additionalActors);
        }
        if(level === 5) {
            this.levelname.text = "Press R to restart the very level. That is of greatest importance!";
            
            additionalActors.push(new Prop("prop1", 530, 100, Math.PI+0.1));
            additionalActors.push(new Prop("prop2", 130, 100, Math.PI-0.1));
            additionalActors.push(new DecoGear("gear1", 700, 200, 0, 0.3));
            additionalActors.push(new DecoGear("gear2", 723, 160, 0, -0.5, 1.2));
            additionalActors.push(new DecoGear("gear3", 650, 115, 0.08, 0.9));
            additionalActors.push(new DecoGear("gear4", 700, 79, 0.08, -2.3));

            loadLevel(this, "levels/level5.json", additionalActors);
        }
        if(level === 6) {
            this.levelname.text = "You may now use your right mouse button well to perform an environmental action!";

            loadLevel(this, "levels/level6.json", additionalActors);
        }
        if(level === 7) {
            this.levelname.text = "Blowing boxes is one of the most gentlemanly sports ever invented, don't you think?";

            loadLevel(this, "levels/level7.json", additionalActors);
        }
        if(level === 8) {
            this.levelname.text = "What a brilliant circumstance that you now have unlocked the full potential of this device!";

            loadLevel(this, "levels/level8.json", additionalActors);
        }
        if(level === 9) {
            this.levelname.text = "A gentleman without a pile of crates is still a gentleman. Just without crates.";

            loadLevel(this, "levels/above.json", additionalActors);
        }
        
        if(level === 10) {
            this.levelname.text = "Even a door which has to be opened with two keys is not challenging a true gentleman.";

            this.removeCollisions = false;
            loadLevel(this, "levels/secondkey.json", additionalActors);
        }
        
        if(level === 11) {
            this.levelname.text = "The smell of tea gets stronger every step the gentleman takes. And it's almost time, isn't it?";

            additionalActors.push(new MegaGear("gear1", 850, 100, 1, 0.1, 2, true));
            additionalActors.push(new MegaGear("gear2", 50, 200, 0, -0.1, 2, true));
            additionalActors.push(new MegaGear("gear3", 600, 2200, 1, 0.2, 0.7));
            additionalActors.push(new MegaGear("gear4", 400, 2150, 0, -0.1, 0.5));
            additionalActors.push(new DecoGear("gear5", 400, 2150, 0, 0.3));
            additionalActors.push(new DecoGear("gear6", 523, 2100, 0, -0.5, 1.2));
            additionalActors.push(new DecoGear("gear7", 550, 2200, 0.08, 0.9));
            additionalActors.push(new DecoGear("gear8", 490, 2150, 0.1, -2.3));

            this.removeCollisions = false;
            loadLevel(this, "levels/final.json", additionalActors);
        }
        
        if(level === 12) {
            teatime = true;
            gamvas.state.setState('MenuState');
        }
        
        /*if(level === xxx) {
            this.levelname.text = "This room seems strange, but with a smell most refreshing coming from that door!";
            loadLevel(this, "levels/tube.json");
        }*/
    },
    
    spawnPlayer: function()
    {
        this.player = new Player("player", this.resetPosition.x, this.resetPosition.y);
        this.addActor(this.player);
    },

    onMouseDown: function(b, x, y) {
        if (b == gamvas.mouse.RIGHT && this.gunMode >= 2) {
            this.player.gun.shoot("primary");
        }
    },

    onKeyUp: function(k, c, e) {
        if (k in this.keysPressed) {
            this.keysPressed[k] = false;
        }
    },

    onKeyPushedDown: function(k, c, e) {
        if(k == gamvas.key.R) {
            this.resetPlayer();
        } else if(k == gamvas.key.F) {
            if(this.noclip === 1) {
                this.noclip = 0;
                gamvas.physics.setGravity(new gamvas.Vector2D(0, 30));
            } else {
                this.noclip = 1;
                gamvas.physics.setGravity(new gamvas.Vector2D(0, 0));
            }
        } else if(isKey(k, JUMP_KEYS)) {
            this.player.jump();
        } else if(DEBUG)
        {
            if(k == gamvas.key.PAGE_UP) {
                if(this.level < 12) {
                    this.level += 1;
                    this.scheduleChangeLevel = true;
                }
            } else if(k == gamvas.key.PAGE_DOWN) {
                if(this.level > 0) {
                    this.level -= 1;
                    this.scheduleChangeLevel = true;
                }
            }
        }
    },

    onKeyDown: function(k, c, e) {
        if (!(k in this.keysPressed) || !this.keysPressed[k]) {
            this.keysPressed[k] = true;
            this.onKeyPushedDown(k, c, e);
        }
    },

    playerWins: function(tile) {
        if(this.keysLeft > 0) {
            this.addActor(new Bubble("keybubble", this.player, getKeyAnimation()));
            println("Get the missing " + this.keysLeft + " keys!");
        } else {
            ++this.level;
            this.scheduleChangeLevel = true;
        }
    },

    playerDied: function(tile) {
        this.died = true;
    },

    findKey: function(key) {
        this.removeActor(key);
        this.keysLeft--;
    }
});

MenuState = gamvas.State.extend({
    init: function() {
        MUSIC = new Audio("snd/clocksong1.ogg");

        this.addActor(new MegaGear("gear3", 300, -150, 0, -0.1, 3, true));
        this.addActor(new MegaGear("gear1", 250, 250, 0, 0.1, 1.5));
        this.addActor(new MegaGear("gear2", 300, -150, 0, -0.1, 1));
        this.addActor(new TextActor("title", "Airy Viktor", -350, -200, 100, "#999"));
        this.addActor(new Viktor("viktor", 80, -50));

        // this is shown at pseudoState === 0
        this.addActor(new TextActor("start", "start", -350, -100, 40, "#999"));
        this.addActor(new TextActor("instructions", "instructions", -350, -60, 40, "#999"));
        this.addActor(new TextActor("credits", "credits", -350, -20, 40, "#999"));
        this.addActor(new TextActor("quit", "quit", -350, 20, 40, "#999"));

        // this is shown at pseudoState === 1
        this.addActor(new TextActor("instructions_title", "Instructions", -350, -100, 40, "#FFF"));
        this.addActor(new TextActor("instructions_line1", "You are Viktor, a dapper gentleman.", -350, -40, 30, "#999"));
        this.addActor(new TextActor("instructions_line2", "You are trapped in a hideous steam machine.", -350, 0, 30, "#999"));
        this.addActor(new TextActor("instructions_line3", "It is of utmost importance that you get home in time for tea.", -350, 40, 30, "#999"));
        this.addActor(new TextActor("instructions_line4", "You are equipped with a steam weapon.", -350, 80, 30, "#999"));

        // this is shown at pseudoState === 2
        this.addActor(new TextActor("credits_title", "Credits", -350, -100, 40, "#FFF"));
        this.addActor(new TextActor("credits_line2", "Paul Bienkowski", -350, -40, 30, "#999"));
        this.addActor(new TextActor("credits_line1", "Sascha Graeff", -350, 0, 30, "#999"));
        this.addActor(new TextActor("credits_line3", "Sven-Hendrik Haase", -350, 40, 30, "#999"));
        this.addActor(new TextActor("credits_line4", "Janina Matz", -350, 80, 30, "#999"));

        // this is shown at pseudoState === 3
        this.addActor(new TextActor("quit_line1", "This is a web game, good chap.", -350, 100, 30, "#999"));
        this.addActor(new TextActor("quit_line2", "Why not just close the tab?", -350, 140, 30, "#999"));

        // this is shown at pseudoState === 4
        this.addActor(new TextActor("teatime_line1", "You managed to get home just right for teatime.", -350, 100, 30, "#999"));
        this.addActor(new TextActor("teatime_line2", "Indubitably.", -350, 140, 30, "#999"));
        this.addActor(new Viktor("teatime", 80, -50));

        
        this.currentMenuEntry = 0;
        this.pseudoState = 0;
    },
    
    enter: function() {
        if(teatime)
        {
            this.pseudoState = 4;
            teatime = false;
        }
    },

    draw: function() {
        for(var actor in this.actors) {
            this.actors[actor].setActive(false);
        }

        this.actors.gear1._isActive = true;
        this.actors.gear2._isActive = true;
        this.actors.gear3._isActive = true;
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
                if(this.actors[actor]._isActive)
                    this.actors[actor].color = "#999";
            }
            if(this.currentMenuEntry === 0) {
                this.actors.start.color = "#fff";
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
        } else if(this.pseudoState === 4) {
            this.actors.teatime_line1._isActive = true;
            this.actors.teatime_line2._isActive = true;
            this.actors.teatime._isActive = true;
            this.actors.viktor._isActive = false;
        }


        if(DEBUG !== true) {
            MUSIC.play();
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
            if(this.pseudoState === 0)
            {
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
            else
            {
                this.pseudoState = 0;
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
