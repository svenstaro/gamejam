function tileVisible(tile) {
    var s = gamvas.state.getCurrentState();
    var c = s.camera.position;
    var d = s.dimension;
    var b = TILESIZE;

    return (
        tile.position.x > c.x - d.w / 2 - TILESIZE * 3 &&
        tile.position.x < c.x + d.w / 2 + TILESIZE * 3 &&
        tile.position.y > c.y - d.h / 2 - TILESIZE * 3 &&
        tile.position.y < c.y + d.h / 2 + TILESIZE * 3)
}

/*
 *     COLLISION=1  DEATH=2
 * 1 = Square       Square
 * 2 = Bottom Left  Bottom
 * 3 = Bottom Right Top
 * 4 = Top Left     Left
 * 5 = Top Right    Right
 */

function makeCollisionShape(id, x, y) {
    var v = [];
    var c = 0.5;

    x /= TILESIZE;
    y /= TILESIZE;

    if(id != 3)
        v.push(new b2Vec2(x - c, y - c));
    if(id != 2)
        v.push(new b2Vec2(x + c, y - c));
    if(id != 4)
        v.push(new b2Vec2(x + c, y + c));
    if(id != 5)
        v.push(new b2Vec2(x - c, y + c));

    return v;
}

function makeDeathShape(id, x, y) {
    var v = [];
    var c = 0.5;

    x /= TILESIZE;
    y /= TILESIZE;

    if(id == 1 || id == 3 || id == 4)
        v.push(new b2Vec2(x - c, y - c));
    if(id == 4 || id == 5)
        v.push(new b2Vec2(x + 0, y - c));
    if(id == 1 || id == 3 || id == 5)
        v.push(new b2Vec2(x + c, y - c));
    if(id == 2 || id == 3)
        v.push(new b2Vec2(x + c, y + 0));
    if(id == 1 || id == 2 || id == 5)
        v.push(new b2Vec2(x + c, y + c));
    if(id == 4 || id == 5)
        v.push(new b2Vec2(x + 0, y + c));
    if(id == 1 || id == 2 || id == 3)
        v.push(new b2Vec2(x - c, y + c));
    if(id == 2 || id == 3)
        v.push(new b2Vec2(x - c, y + 0));
    return v;
}


Tile = gamvas.Actor.extend({
    create: function(name, x, y, xOffset, yOffset, tileset, layer)
    {
        this._super(name, (x) * TILESIZE, (y) * TILESIZE);

        var st = gamvas.state.getCurrentState();

        this.image = new gamvas.Image(st.resource.getImage(tileset));
        //this.image.position = new gamvas.Vector2D(this.position.x, this.position.y);
        this.image.position = this.position;
        this.image.move(-xOffset  * TILESIZE, -yOffset * TILESIZE);
        this.image.setClipRect(xOffset * TILESIZE, yOffset * TILESIZE, TILESIZE, TILESIZE);

        this.layer = layer;
    },
    
    draw: function(t)
    {
        // and you thought YOU were hacking
        if(tileVisible(this)) 
            this.image.draw();
    }
});

CollisionTile = gamvas.Actor.extend({
    blockWind: true,
    create: function(name, x, y, id) {
        this._super(name, 0, 0);
        this.type = "tile";

        var polygon = new Box2D.Collision.Shapes.b2PolygonShape;
        var vertices = makeCollisionShape(id, this.position.x, this.position.y);
        polygon.SetAsArray(vertices, vertices.length);
        this.createBody(gamvas.physics.STATIC, polygon);
        this.fixture.SetRestitution(0);
        this.setPosition((x + 0.5) * TILESIZE, (y + 0.5) * TILESIZE);
    },
    draw: function(t) {
        this.body.SetActive(tileVisible(this));
    }
});

DeathTile = gamvas.Actor.extend({
    create: function(name, x, y, id) {
        this._super(name, 0, 0);

        var polygon = new Box2D.Collision.Shapes.b2PolygonShape;
        var vertices = makeDeathShape(id, 0, 0);
        polygon.SetAsArray(vertices, vertices.length);
        this.createBody(gamvas.physics.STATIC, polygon);
        this.setSensor(true);

        this.setPosition((x + 0.5) * TILESIZE, (y + 0.5) * TILESIZE);
    },
    draw: function(t) {
        this.body.SetActive(tileVisible(this));
    }
});
