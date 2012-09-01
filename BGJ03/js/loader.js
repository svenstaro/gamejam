function loadLevel(state, level) {
    $.getJSON(level, function(json) {
        var width = json.width;
        var height = json.height;  
        
        for(var layerindex = 0; layerindex < json.layers.length; ++layerindex) {
            var data = json.layers[layerindex].data;

            for(var y = 0; y < height; y++) {
                for(var x = 0; x < width; x++) {
                    var tileindex = data[x+width*y];

                    if(tileindex !== 0) {
                        var tilesetindex = 0;
                        while(tilesetindex < json.tilesets.length && json.tilesets[tilesetindex].firstgid > tileindex) {
                            ++tilesetindex;
                        }

                        var tilesetLineWidth = json.tilesets[tilesetindex].imagewidth / TILESIZE;

                        var tileX = (tileindex-1) % tilesetLineWidth;
                        var tileY = Math.floor((tileindex-1) / tilesetLineWidth);

                        if(json.layers[layerindex].name != "collision") {
                            var img = 'levels/'+json.tilesets[tilesetindex].image;
                            var z = tryParseInt(json.layers[layerindex].name);
                            state.addActor(new Tile(nextId("tile-"), x, y, tileX, tileY, img, z));
                        } else {
                            state.addActor(new CollisionTile(nextId("collisiontile-"), x, y, tileindex));
                        }
                    }
                }
            }
        }
    });
}
