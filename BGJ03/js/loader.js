function loadLevel(state, level) {
    $.getJSON(level, function(json) {
        var width = json.width;
        var height = json.height;
        state.levelWidth = width * TILESIZE;
        state.levelHeight = height * TILESIZE;

        
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
                            var tile =  new Tile("tile-" + x + "-" + y + "-onlayer-"+layerindex, 
                                                x, y,
                                                tileX, tileY,
                                                'levels/'+json.tilesets[tilesetindex].image,
                                                tryParseInt(json.layers[layerindex].name));
                            state.addActor(tile);
                        } else {
                            if(tileindex >= 1 && tileindex <= 5) {
                                state.addActor(new CollisionTile(nextId("collisiontile-"), x, y, tileindex));
                            } else if((tileindex >= 5 && tileindex <= 10) || tileindex == 12) {
                                var id = (tileindex == 12 ? 1 : tileindex - 5);
                                var callback = (tileindex == 12 ? state.playerWins : state.playerDied);
                                state.addActor(new TriggerTile(nextId("triggertile-"), x, y, id, callback));
                            } else if(tileindex == 11) {
                                state.resetPosition.x = (x + 0.5) * TILESIZE;
                                state.resetPosition.y = y * TILESIZE;
                            } else if(tileindex == 14) {
                                state.addActor(new Box(nextId("box-"), x, y));
                            }
                        }
                    }
                }
            }
        }

        for(var x = 0; x < width; x++) {
            state.addActor(new CollisionTile(nextId("walls-"), x, -1, 1));
            state.addActor(new CollisionTile(nextId("walls-"), x, height + 1, 1));
        }

        for(var y = 0; y < height; y++) {
            state.addActor(new CollisionTile(nextId("walls-"), -1, y, 1));
            state.addActor(new CollisionTile(nextId("walls-"), width + 1, y, 1));
        }

        state.resetPlayer();
    });
}
