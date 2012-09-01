// create our collision objects
Wind = gamvas.ParticleEmitter.extend({
    create: function(x, y, dir) {
        var img = new gamvas.Image(gamvas.state.getCurrentState().resource.getImage('gfx/steam.png'))
        img.setCenter(32, 32);

        this._super(nextId("steam-"), x, y, img);
        this.setPosition(x, y);
        this.setParticleRate(200);
        this.setRotation(dir - Math.PI / 2);
        this.setParticleSpeed(500);
        this.setParticleSpeedRange(100);
        this.setRotationRange(BLAST_ANGLE / 2);
        // this.setParticleStartPositionRange(new gamvas.Vector2D(10, 10));
        // this.setGravity(0);
        this.setSpeedScaleTable([ [0, 1, 1], [300, 0.02, 2] ]);
        this.alignParticleToPath(true);
        this.setAlphaTable([ [0, 0], [0.1, 0.1], [1, 0] ]);
        //this.setScaleTable([ [0, 0], [1, 1] ]);
        this.setParticleLifeTime(2);
        this.setParticleLifeTimeRange(0.4);
        this.setParticleVelocityDamping(2);
    },


    draw: function(t) {
        if(this.lifeTime > 0.2)
            this.setParticleRate(0);
        this._super(t);
    }
});
