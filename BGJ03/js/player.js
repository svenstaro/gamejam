// create our collision objects
Player = gamvas.Actor.extend({
    moveByWind: true,
    create: function(name, x, y) {
        this._super(name, x, y);

        var st = gamvas.state.getCurrentState();
        this.gun = new Gun("gun", this);

        this.addAnimation(new gamvas.Animation("anim1", st.resource.getImage('gfx/player.png'), 32, 32, 1, 10));
        this.addAnimation(new gamvas.Animation("anim2", st.resource.getImage('gfx/player.png'), 32, 32, 1, 40));
        this.setAnimation("anim1");

        // create a static (non moving) rectangle
        //this.bodyCircle(this.position.x, this.position.y, 16, gamvas.physics.DYNAMIC);
        this.bodyRect(this.position.x, this.position.y, 16, 32, gamvas.physics.DYNAMIC);
        this.center.x = 16;
        this.center.y = 16;
        this.setFixedRotation(true);
        this.fixture.SetFriction(0);
        this.fixture.SetRestitution(0);

        this.groundContacts = new Array();
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
        
        this.getCurrentState().onCollisionEnter = function(collider, contact)
        {
            print("type:"+collider.type);
            if(collider.position.y > this.actor.position.y + TILESIZE - 1)
            {
                this.actor.groundContacts.push(collider);
            }
        };
        
        this.getCurrentState().onCollisionLeave = function(collider, contact)
        {
            deleteFromArray(this.actor.groundContacts, collider);
        };
    },

    isOnGround: function() {
        return this.groundContacts.length > 0;
    },

    jump: function() {
        if(!this.isOnGround()) {
            this.inAirJump = true;
            return;
        }
        this.inAirJump = false;

        this.body.ApplyImpulse(new b2Vec2(0, -4.7), new b2Vec2(0, 0));
        // this.actor.body.m_linearVelocity.y = -8;
    }
});
