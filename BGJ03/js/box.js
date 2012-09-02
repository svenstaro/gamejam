Box = gamvas.Actor.extend({
    moveByWind: true,
    
    create: function(name, x, y) {
        this._super(name, x * TILESIZE, y * TILESIZE);
        this.type = "box";

        var st = gamvas.state.getCurrentState();
        this.setFile(st.resource.getImage('gfx/crate.png'));

        this.bodyRect(this.position.x, this.position.y, TILESIZE, TILESIZE, gamvas.physics.DYNAMIC);
        this.body.SetMassData({mass: 2, center: { x: 0, y: 0}});
    },
});
