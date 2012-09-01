// create our collision objects
Player = gamvas.Actor.extend({
    moveByWind: true,
    create: function(name, x, y) {
        this._super(name, x, y);

        var st = gamvas.state.getCurrentState();
        this.gun = new Gun("gun", this);
        st.addActor(this.gun);

        this.addAnimation(new gamvas.Animation("anim1", st.resource.getImage('gfx/playerleft.png'), 64, 64, 10, 10));
        this.addAnimation(new gamvas.Animation("anim2", st.resource.getImage('gfx/playerleft.png'), 64, 64, 10, 10));
        this.setAnimation("anim1");

        // create a static (non moving) rectangle
        //this.bodyCircle(this.position.x, this.position.y, 16, gamvas.physics.DYNAMIC);
        //this.bodyRect(this.position.x, this.position.y, 32, 64, gamvas.physics.DYNAMIC);
        var vertices = Array();
        vertices.push(new b2Vec2(0.4, 0.9));
        vertices.push(new b2Vec2(0.3, 1.0));
        vertices.push(new b2Vec2(-0.3, 1.0));
        vertices.push(new b2Vec2(-0.4, 0.9));
        vertices.push(new b2Vec2(-0.4, -0.9));
        vertices.push(new b2Vec2(-0.3, -1.0));
        vertices.push(new b2Vec2(0.3, -1.0));
        vertices.push(new b2Vec2(0.4, -0.9));
        var polygon = new Box2D.Collision.Shapes.b2PolygonShape;
        polygon.SetAsArray(vertices, vertices.length);
        this.createBody(gamvas.physics.DYNAMIC, polygon);
        
        this.center.x = 32;
        this.center.y = 32;
        this.setFixedRotation(true);
        this.fixture.SetFriction(0);
        this.fixture.SetRestitution(0);

        this.contacts = new Array();
        this.inAirJump = false;

        this.getCurrentState().update = function(t) {
            var f = 6;

            this.actor.gun.position = this.actor.position;

            if (isKeyDown(LEFT_KEYS)) { 
                this.actor.body.m_linearVelocity.x = -f;
            } else if (isKeyDown(RIGHT_KEYS)) {
                this.actor.body.m_linearVelocity.x = f;
            } else {
                this.actor.body.m_linearVelocity.x *= (1 - t * 8);
            }

            if (this.actor.isOnGround() && this.actor.inAirJump && isKeyDown(JUMP_KEYS)) {
                this.actor.jump();
            }
        };
        
        this.getCurrentState().onCollisionEnter = function(collider)
        {
            if(collider.type == "tile" && collider.position.y - this.actor.position.y > Math.abs(collider.position.x - this.actor.position.x))
            //if(collider.type == "tile")
            {
                this.actor.contacts.push(collider);
            }
            println((collider.position.y - this.actor.position.y)+" > "+Math.abs(collider.position.x - this.actor.position.x));
        };
        
        this.getCurrentState().onCollisionLeave = function(collider)
        {
            deleteFromArray(this.actor.contacts, collider);
        };
    },

    isOnGround: function() {
        if(this.contacts.length > 0)
        {
            return true;
        }
        return false;
    },

    jump: function() {
        if(!this.isOnGround()) {
            this.inAirJump = true;
            return;
        }
        this.inAirJump = false;

        this.body.ApplyImpulse(new b2Vec2(0, -4.7 * 4), new b2Vec2(0, 0));
        // this.actor.body.m_linearVelocity.y = -8;
    }
});
