// create our collision objects
Player = gamvas.Actor.extend({
    moveByWind: true,
    create: function(name, x, y) {
        this._super(name, x, y);

        var st = gamvas.state.getCurrentState();
        this.gun = new Gun("gun", this);
        st.addActor(this.gun);

        this.addAnimation(new gamvas.Animation("walk-right", st.resource.getImage('gfx/playerright.png'), 64, 64, 10, 20));
        this.addAnimation(new gamvas.Animation("walk-left", st.resource.getImage('gfx/playerleft.png'), 64, 64, 10, 20));
        this.addAnimation(new gamvas.Animation("walk-left-big", st.resource.getImage('gfx/playerleft_big.png'), 64, 64, 10, 20));
        var idleLeft = new gamvas.Animation("idle-left", st.resource.getImage('gfx/playerleft.png'), 64, 64, 10, 10);
        idleLeft.setFrameList([2, 2, 2, 3, 3, 3]);
        this.addAnimation(idleLeft);
        var idleLeft = new gamvas.Animation("idle-right", st.resource.getImage('gfx/playerright.png'), 64, 64, 10, 10);
        idleLeft.setFrameList([2, 2, 2, 3, 3, 3]);
        this.addAnimation(idleLeft);
        
        this.setAnimation("idle-right");
        
        //this.addAnimation(new gamvas.Animation("idle", st.resource.getImage('gfx/idle.png'), 64, 64, 2, 6));
        
        this.lookDirectionRight = true;

        // create a static (non moving) rectangle
        //this.bodyCircle(this.position.x, this.position.y, 16, gamvas.physics.DYNAMIC);
        //this.bodyRect(this.position.x, this.position.y, 32, 64, gamvas.physics.DYNAMIC);
        var vertices = Array();
        vertices.push(new b2Vec2(0.4, 0.5));
        vertices.push(new b2Vec2(0.3, 1.0));
        vertices.push(new b2Vec2(-0.3, 1.0));
        vertices.push(new b2Vec2(-0.4, 0.5));
        vertices.push(new b2Vec2(-0.4, -0.5));
        vertices.push(new b2Vec2(-0.3, -0.8));
        vertices.push(new b2Vec2(0.3, -0.8));
        vertices.push(new b2Vec2(0.4, -0.5));
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

            if (isKeyDown(LEFT_KEYS)) { 
                this.actor.body.m_linearVelocity.x = -f;
                this.actor.lookDirectionRight = false;
                this.actor.gun.layer = 0.1;
                this.actor.gun.image.setScaleXY(1,-1);
                //this.actor.setAnimation("walk-left");
            } else if (isKeyDown(RIGHT_KEYS)) {
                this.actor.body.m_linearVelocity.x = f;
                this.actor.lookDirectionRight = true;
                this.actor.gun.layer = -0.1;
                this.actor.gun.image.setScaleXY(1,1);
                //this.actor.setAnimation("walk-right");
            } else {
                this.actor.body.m_linearVelocity.x *= (1 - t * 8);
            }

            this.actor.setAnimation(
                    (Math.abs(this.actor.body.m_linearVelocity.x) <= 0.3 ? "idle" : "walk")
                    + "-" +
                    (this.actor.lookDirectionRight ? "right" : "left"));

            if (this.actor.isOnGround() && this.actor.inAirJump && isKeyDown(JUMP_KEYS)) {
                this.actor.jump();
            }
        };
        
        this.getCurrentState().onCollisionEnter = function(collider)
        {
            if(collider.type == "tile")
            {
                this.actor.contacts.push(collider);
            }
        };
        
        this.getCurrentState().onCollisionLeave = function(collider)
        {
            deleteFromArray(this.actor.contacts, collider);
        };
    },

    isOnGround: function() {
        if(this.contacts.length > 0)
        {
            for(var i = 0; i < this.contacts.length; ++i)
            {
                if(this.contacts[i].position.y - this.position.y - TILESIZE > Math.abs(this.contacts[i].position.x - this.position.x))
                {
                    return true;
                }
            }
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

Viktor = gamvas.Actor.extend({
    create: function(name, x, y) {
        this._super(name, x, y);
        var st = gamvas.state.getCurrentState();
        this.addAnimation(new gamvas.Animation("walk-left", st.resource.getImage('gfx/playerleft_medium.png'), 341, 341, 10, 8));
        this.setAnimation("walk-left");
    }
});
