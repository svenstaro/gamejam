// create our collision objects
Wind = gamvas.Actor.extend({
    create: function(name, x, y, direction) {
        this._super(name, x, y);
        this.direction = direction;
        this.dir = new b2Vec2(
            Math.cos(direction),
            Math.sin(direction));

        var radius = 5;
        this.bodyCircle(
            x + this.dir.x * radius * 2, 
            y + this.dir.y * radius * 2, 
            radius, gamvas.physics.DYNAMIC);
        this.setSensor(true);
        this.fixture.SetDensity();

        this.dir.Multiply(2);
        this.body.ApplyImpulse(this.dir, new b2Vec2());

        this.getCurrentState().update = function(t) {
            for (var ce = this.actor.body.m_contactList; ce; ce = ce.next) {
                var other = ce.contact.m_nodeA.other.GetUserData().data;
                if(other == this.actor) {
                    var other = ce.contact.m_nodeB.other.GetUserData().data;
                }

                if(other.type == "tile" && ce.contact.IsTouching()) {
                    println("BAAAM");
                    gamvas.state.getCurrentState().removeActor(this.actor);
                }

                if(other.moveByWind) {
                    println("Pfft.");
                    this.actor.dir.Normalize();
                    this.actor.dir.Multiply(0.5);
                    other.body.ApplyImpulse(this.actor.dir, new b2Vec2());
                    gamvas.state.getCurrentState().removeActor(this.actor);
                }
            }
        };
    }
});

