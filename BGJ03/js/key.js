Key = TriggerTile.extend({
    create: function(name, x, y) {
        this._super(name, x, y, 1, function(tile) {
            gamvas.state.getCurrentState().findKey(this);
        });

        this.addAnimation(new gamvas.Animation("sparkle", 
                gamvas.state.getCurrentState().resource.getImage('gfx/key.png'), 32, 32, 2, 3));
        this.setAnimation("sparkle");
    },
});
