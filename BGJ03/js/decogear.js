DecoGear = gamvas.Actor.extend({
    create: function(name, x, y, startRotation, rotationSpeed)
	{
        this._super(name, x, y);

        var st = gamvas.state.getCurrentState();
		this.setFile(st.resource.getImage('gfx/biggear.png'));
		this.setCenter(64,64);
		
		this.layer = 1;

        this.rotationSpeed = rotationSpeed;
		this.rotate(startRotation);
		
		this.getCurrentState().update = function(t)
		{
			this.actor.rotate(this.actor.rotationSpeed * t);
		}
	}
});
