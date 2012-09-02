Box = gamvas.Actor.extend({
    moveByWind: true,
    
    create: function(name, x, y) {
        this._super(name, (0.45 + x) * TILESIZE, (0.45 + y) * TILESIZE);
        this.type = "box";

        var st = gamvas.state.getCurrentState();
        this.setFile(st.resource.getImage('gfx/crate.png'));
        
        this.setScale(0.8, 0.8);
    },
    
    activatePhysics: function() {
        var polygon = new Box2D.Collision.Shapes.b2PolygonShape;
        var vertices = [];
        vertices.push(new b2Vec2(- 0.4,   0.4));
        vertices.push(new b2Vec2(- 0.4, - 0.4));
        vertices.push(new b2Vec2(  0.4, - 0.4));
        vertices.push(new b2Vec2(  0.4,   0.4));
        polygon.SetAsArray(vertices, vertices.length);

        this.createBody(gamvas.physics.DYNAMIC, polygon);
        
        this.setPosition(this.position.x, this.position.y);

        this.center.x = TILESIZE * 0.5;
        this.center.y = TILESIZE * 0.5;
        this.fixture.SetRestitution(0.1);
        this.body.SetSleepingAllowed(false);
        this.setBullet(true);
        this.setFixedRotation(false);
        // this.body.SetMassData({mass: 2, center: { x: 0, y: 0}});

        var md = {center: new b2Vec2()};
        polygon.ComputeMass(md, 3);
        this.body.SetMassData(md);
        this.body.SetAngularDamping(100);
    }
});
