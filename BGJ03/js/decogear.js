DecoGear = gamvas.Actor.extend({
    create: function(name, x, y, startRotation, rotationSpeed)
    {
        this._super(name, x, y);

        var st = gamvas.state.getCurrentState();
        this.setFile(st.resource.getImage('gfx/biggear.png'));
        this.setCenter(64,64);
        
        this.layer = 5;

        this.rotationSpeed = rotationSpeed;
        this.rotate(startRotation);
        
        this.getCurrentState().update = function(t)
        {
            this.actor.rotate(this.actor.rotationSpeed * t);
        }
    }
});

MegaGear = gamvas.Actor.extend({
    create: function(name, x, y, startRotation, rotationSpeed, scale)
    {
        this._super(name, x, y);

        var st = gamvas.state.getCurrentState();
        this.setFile(st.resource.getImage('gfx/megagear.png'));
        this.setScale(scale);
        this.setCenter(250,250);
        
        this.layer = 5;
        this.tickAccu = 0.05;

        this.rotationSpeed = rotationSpeed;
        this.rotate(startRotation);
        
        this.getCurrentState().update = function(t)
        {
            if(this.actor.tickAccu >= 0.74) {
                this.actor.tickAccu = 0.0;
                this.actor.rotate(this.actor.rotationSpeed * t * 50);
            }
            this.actor.tickAccu = this.actor.tickAccu + t;
        }
    }
});
