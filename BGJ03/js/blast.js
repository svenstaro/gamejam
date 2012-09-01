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

        this.accumulator = 0;
        this.winds = 0;

        this.getCurrentState().update = function(t) {
            this.actor.accumulator += t;
            var dt = 1 / WINDS_PER_SECOND;
            while(this.actor.accumulator > dt && this.actor.winds < WINDS_PER_BLAST) {
                // spawn a wind particle
                var w = new Wind(nextId("wind"), this.actor.position.x, this.actor.position.y, this.actor.rotation);
                gamvas.state.getCurrentState().addActor(w);
                this.actor.winds++;
                this.actor.accumulator -= dt;
            }


        };
    }
});

