// create our collision objects
Blast = gamvas.Actor.extend({
    create: function(name, tile, normal) {
        this._super(name, 
            tile.position.x + normal.x * TILESIZE / 2, 
            tile.position.y + normal.y * TILESIZE / 2);
            //gamvas.physics.toScreen(tile.x), 
            //gamvas.physics.toScreen(tile.y));
        this.rotation = Math.atan2(normal.y, normal.x);

        var st = gamvas.state.getCurrentState();
        this.addAnimation(new gamvas.Animation("idle", st.resource.getImage('gfx/blaster.png'), 32, 32, 1, 40));
        this.setAnimation("idle");
        this.center = new gamvas.Vector2D(0, 16);

        this.getCurrentState().update = function(t) {
        };
    }
});

