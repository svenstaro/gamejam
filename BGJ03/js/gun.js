// create our collision objects
Gun = gamvas.Actor.extend({
    create: function(name, x, y) {
        this._super(name, x, y);

        var st = gamvas.state.getCurrentState();
        this.addAnimation(new gamvas.Animation("idle", st.resource.getImage('gfx/gun.png'), 32, 32, 1, 40));
        this.setAnimation("idle");
        this.center = new gamvas.Vector2D(16, 16);

        this.getCurrentState().update = function(t) {
            var m = gamvas.mouse.getPosition();
            var p = this.actor.position;
            this.actor.rotation = Math.atan2(m.y - p.y, m.x - p.x);
        };
    }
});
