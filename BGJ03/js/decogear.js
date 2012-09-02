DecoGear = gamvas.Actor.extend({
    create: function(name, x, y, startRotation, rotationSpeed, scale)
    {
        this._super(name, x, y);

        var st = gamvas.state.getCurrentState();
        this.setFile(st.resource.getImage('gfx/biggear.png'));
        this.setCenter(64,64);
        this.originalX = x;
        
        this.layer = 500;

        scale = typeof scale !== 'undefined' ? scale : 1;
        this.setScale(scale);

        this.rotationSpeed = rotationSpeed;
        this.rotate(startRotation);
        
        this.getCurrentState().update = function(t)
        {
            this.actor.rotate(this.actor.rotationSpeed * t);
            var campos = gamvas.state.getCurrentState().camera.position.x;
            this.actor.setPosition(campos * 1/5 + this.actor.originalX, this.actor.position.y);
        }
    }
});

MegaGear = gamvas.Actor.extend({
    create: function(name, x, y, startRotation, rotationSpeed, scale, blurry)
    {
        this._super(name, x, y);

        var st = gamvas.state.getCurrentState();
        if(blurry === true) {
            this.setFile(st.resource.getImage('gfx/megagear-blurry.png'));
        } else {
            this.setFile(st.resource.getImage('gfx/megagear.png'));
        }

        this.setScale(scale);
        this.setCenter(250,250);
        
        this.layer = 150;
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

Background = gamvas.Actor.extend({
    create: function(name, x, y)
    {
        this._super(name, x, y);

        var st = gamvas.state.getCurrentState();
        this.setFile(st.resource.getImage('gfx/metall005-new-tileable.png'));
        this.setScale(2);
        
        this.layer = 1000;
        
        this.getCurrentState().update = function(t)
        {
            var campos = gamvas.state.getCurrentState().camera.position;
            this.actor.setPosition(campos.x * 1/3 - 300, campos.y * 1/3 - 300);
        }
    }
});

Prop = gamvas.Actor.extend({
    create: function(name, x, y, rotation)
    {
        this._super(name, x, y, rotation);

        var st = gamvas.state.getCurrentState();
        this.setFile(st.resource.getImage('gfx/prop1.png'));
        this.setScale(2);
        this.setCenter(100, 100);
        this.originalX = x;
        this.setRotation(rotation);
        
        this.layer = 500;
        
        this.getCurrentState().update = function(t)
        {
            var campos = gamvas.state.getCurrentState().camera.position.x;
            this.actor.setPosition(campos * 1/5 + this.actor.originalX, this.actor.position.y);
        }
    }
});
