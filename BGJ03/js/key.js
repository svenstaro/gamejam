function getKeyAnimation() {
    return new gamvas.Animation("sparkle", 
                gamvas.state.getCurrentState().resource.getImage('gfx/key.png'), 32, 32, 2, 3)
}

Key = TriggerTile.extend({
    create: function(name, x, y) {
        this._super(name, x, y, 1, function(tile) {
            gamvas.state.getCurrentState().findKey(this);
        });

        this.addAnimation(getKeyAnimation());
        this.setAnimation("sparkle");
    },
});

Bubble = gamvas.Actor.extend({
    create: function(name, parent, animation) {
        this._super(name, 0, 0);
        this.parent = parent;

        this.image = new gamvas.Image(gamvas.state.getCurrentState().resource.getImage('gfx/speechbubble.png'));
        this.image.center.x = 32;
        this.image.center.y = 28;

        this.addAnimation(animation);
        this.setAnimation(animation.name);
        this.center.x = animation.width / 2;
        this.center.y = animation.height / 2;
        this.lifetime = 0;
    },

    draw: function(t) {
        this.lifetime += t;
        var st = gamvas.state.getCurrentState();

        if(this.lifetime >= 5) {
            st.removeActor(this);
            return;
        }

        this.position = new gamvas.Vector2D(
            this.parent.position.x + 30 , 
            this.parent.position.y - 50);
        this.image.position = this.position;

        this.layer = -10000;
        this.image.layer = -10000;


        if(this.lifetime > 4.5)
            st.c.globalAlpha = 1 - (this.lifetime - 4.5) * 2;
        this.image.draw(t);
        this._super(t);
        st.c.globalAlpha = 1;
    }
});
