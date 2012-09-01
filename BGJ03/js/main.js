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
        
        var temp_state = this;
        $.getJSON('levels/test.json', function(json) {
            var width = json.width;
            var height = json.height;
            
            for(var layerindex = 0; layerindex < json.layers.length; ++layerindex) {
                var data = json.layers[layerindex].data;

                for(var y = 0; y < height; y++) {
                    for(var x = 0; x < width; x++) {
                        var tileindex = data[x+width*y];

                        if(tileindex !== 0) {
                            var tilesetindex = 0;
                            
                            while(tilesetindex < json.tilesets.length-1 && json.tilesets[tilesetindex+1].firstgid <= tileindex) {
                                ++tilesetindex;
                            }

                            var tilesetLineWidth = json.tilesets[tilesetindex].imagewidth / TILESIZE;
                            
                            tileindex -= json.tilesets[tilesetindex].firstgid-1;

                            var tileX = (tileindex-1) % tilesetLineWidth;
                            var tileY = Math.floor((tileindex-1) / tilesetLineWidth);

                            if(json.layers[layerindex].name != "collision") {
                                temp_state.addActor(new Tile("tile-" + x + "-" + y + "-onlayer-"+layerindex, 
                                                                x, y,
                                                                tileX, tileY,
                                                                'levels/'+json.tilesets[tilesetindex].image,
                                                                tryParseInt(json.layers[layerindex].name)));
                            } else {
                                switch(tileindex)
                                {
                                    case 1:
                                        temp_state.addActor(new CollisionTile("collisiontile-" + x + "-" + y, x, y, tileindex));
                                    break;
                                    case 6: case 7:
                                        temp_state.addActor(new DeathTile("deathtile-" + x + "-" + y, x, y, tileindex));
                                    break;
                                    case 11:
                                        temp_state.player.setPosition((x + 0.5) * TILESIZE, y * TILESIZE);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        });

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
