Box = gamvas.Actor.extend({
    moveByWind: true,
    
    create: function(name, x, y) {
        this._super(name, x * TILESIZE, y * TILESIZE);
        this.type = "box";

        var st = gamvas.state.getCurrentState();
        this.setFile(st.resource.getImage('gfx/crate.png'));
    },
    
    activatePhysics: function() {
        var halfskin = 1;
        this.bodyRect(this.position.x + halfskin,
                      this.position.y + halfskin,
                      TILESIZE - halfskin*2,
                      TILESIZE - halfskin*2, gamvas.physics.DYNAMIC);
        this.body.SetMassData({mass: 2, center: { x: 0, y: 0}});
    }
});
