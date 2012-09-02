Box = gamvas.Actor.extend({
    moveByWind: true,
    
    create: function(name, x, y) {
        this._super(name, 0, 0);
        this.type = "box";

        var st = gamvas.state.getCurrentState();
        this.setFile(st.resource.getImage('gfx/crate.png'));
        this.setScale(0.8, 0.8);

        var polygon = new Box2D.Collision.Shapes.b2PolygonShape;
        var vertices = [];
        vertices.push(new b2Vec2(- 0.4,   0.4));
        vertices.push(new b2Vec2(- 0.4, - 0.4));
        vertices.push(new b2Vec2(  0.4, - 0.4));
        vertices.push(new b2Vec2(  0.4,   0.4));
        polygon.SetAsArray(vertices, vertices.length);

        this.createBody(gamvas.physics.DYNAMIC, polygon);
        this.setPosition((0.45 + x) * TILESIZE, (0.45 + y) * TILESIZE);

        this.body.SetMassData({mass: 3, center: { x: 0, y: 0}});
        this.center.x = TILESIZE * 0.5;
        this.center.y = TILESIZE * 0.5;
        this.fixture.SetRestitution(0.1);
        this.body.SetSleepingAllowed(false);
    },
});
