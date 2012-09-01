// create our collision objects
Tile = gamvas.Actor.extend({
    create: function(name, x, y) {
        this._super(name, x * TILESIZE, y * TILESIZE);

        var st = gamvas.state.getCurrentState();
        //this.setFile(st.resource.getImage('gfx/wall9.png'));
		this.image = new gamvas.Image(st.resource.getImage('gfx/wall9.png'));
		this.image.position = new gamvas.Vector2D(this.position.x, this.position.y);
		var xOffset = 1;
		var yOffset = 1;
		this.image.move(-(xOffset + 0.5) * TILESIZE, -(yOffset + 0.5) * TILESIZE);
		this.image.setClipRect(xOffset * TILESIZE, yOffset * TILESIZE, TILESIZE, TILESIZE);

        // create a static (non moving) rectangle
        this.bodyRect(this.position.x, this.position.y, TILESIZE, TILESIZE, gamvas.physics.STATIC);
		
		this.fixture.SetRestitution(0);
    },
	
	draw: function(t)
	{
		// and you thought YOU were hacking
		this.image.draw();
	}
});
