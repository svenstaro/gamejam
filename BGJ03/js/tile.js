// create our collision objects
Tile = gamvas.Actor.extend({
        create: function(name, x, y) {
            this._super(name, x * TILESIZE, y * TILESIZE);

            var st = gamvas.state.getCurrentState();
            this.setFile(st.resource.getImage('gfx/wall.png'));

            // create a static (non moving) rectangle
            this.bodyRect(this.position.x, this.position.y, TILESIZE, TILESIZE, gamvas.physics.STATIC);
        }
});
