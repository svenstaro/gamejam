
Tile = gamvas.Actor.extend({
    create: function(name, x, y, xOffset, yOffset, tileset)
    {
        this._super(name, (x + 0.5) * TILESIZE, (y + 0.5) * TILESIZE);
        this.type = "tile";

        var st = gamvas.state.getCurrentState();

        this.image = new gamvas.Image(st.resource.getImage(tileset));
        this.image.position = new gamvas.Vector2D(this.position.x, this.position.y);
        this.image.move(-(xOffset + 0.5) * TILESIZE, -(yOffset + 0.5) * TILESIZE);
        this.image.setClipRect(xOffset * TILESIZE, yOffset * TILESIZE, TILESIZE, TILESIZE);

        // create a static (non moving) rectangle
        //this.bodyRect(this.position.x, this.position.y, TILESIZE, TILESIZE, gamvas.physics.STATIC);
        //this.fixture.SetRestitution(0);
    },
    
    draw: function(t)
    {
        // and you thought YOU were hacking
        this.image.draw();
    }
});

CollisionTile = gamvas.Actor.extend({
    create: function(name, x, y, xOffset, yOffset)
    {
        this._super(name, (x + 0.5) * TILESIZE, (y + 0.5) * TILESIZE);

        // create a static (non moving) rectangle
        this.bodyRect(this.position.x, this.position.y, TILESIZE, TILESIZE, gamvas.physics.STATIC);
        this.fixture.SetRestitution(0);
    }
});
