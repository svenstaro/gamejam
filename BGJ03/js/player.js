// create our collision objects
Player = gamvas.Actor.extend({
    create: function(name, x, y) {
        this._super(name, x, y);

        var st = gamvas.state.getCurrentState();
        this.gun = new Gun("gun", x, y);

        this.addAnimation(new gamvas.Animation("anim1", st.resource.getImage('gfx/player.png'), 32, 32, 1, 10));
        this.addAnimation(new gamvas.Animation("anim2", st.resource.getImage('gfx/player.png'), 32, 32, 1, 40));
        this.setAnimation("anim1");

        // create a static (non moving) rectangle
        this.bodyCircle(this.position.x, this.position.y, 16, gamvas.physics.DYNAMIC);
        this.setFixedRotation(true);
        this.fixture.SetFriction(0);
        this.fixture.SetRestitution(0);

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
    },

    isOnGround: function() {
        for (var ce = this.body.m_contactList; ce; ce = ce.next) {
            var other = ce.contact.m_nodeA.other.GetUserData().data;
            if(other == this)
                var other = ce.contact.m_nodeB.other.GetUserData().data;

            if(ce.contact.IsTouching() && other.position.y > this.position.y + TILESIZE - 1)
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

        this.body.ApplyImpulse(new b2Vec2(0, -9), new b2Vec2(0, 0));
        // this.actor.body.m_linearVelocity.y = -8;
    }
});
