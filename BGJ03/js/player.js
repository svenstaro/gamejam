// create our collision objects
Player = gamvas.Actor.extend({
        create: function(name, x, y) {
            this._super(name, x, y);

            var st = gamvas.state.getCurrentState();
			this.addAnimation(new gamvas.Animation("anim1", st.resource.getImage('gfx/anim.png'), 32, 32, 4, 10));
			this.addAnimation(new gamvas.Animation("anim2", st.resource.getImage('gfx/anim.png'), 32, 32, 4, 40));
			this.setAnimation("anim1");

            // create a static (non moving) rectangle
            this.bodyCircle(this.position.x, this.position.y, 16, gamvas.physics.DYNAMIC);
			this.setFixedRotation(true);
            this.fixture.SetFriction(0);
			
            this.getCurrentState().update = function(t) {
                var f = 4;

                if (gamvas.key.isPressed(gamvas.key.LEFT) 
                    || gamvas.key.isPressed(gamvas.key.A)) {
                        this.actor.body.m_linearVelocity.x = -f;
                } else if (gamvas.key.isPressed(gamvas.key.RIGHT)
                    || gamvas.key.isPressed(gamvas.key.D)) {
                        this.actor.body.m_linearVelocity.x = f;
                } else {
                        this.actor.body.m_linearVelocity.x = 0;
                }

                if (gamvas.key.isPressed(gamvas.key.UP)
                    || gamvas.key.isPressed(gamvas.key.W)) {
					this.actor.setAnimation("anim2");
                }

                if (gamvas.key.isPressed(gamvas.key.DOWN)
                    || gamvas.key.isPressed(gamvas.key.S)) {
					this.actor.setAnimation("anim1");
                }
            };
        },

        isOnGround: function() {
            for (var ce = this.body.m_contactList; ce; ce = ce.next) {
                var other = ce.contact.m_nodeA.other.GetUserData().data;
                if(other == this)
                    var other = ce.contact.m_nodeB.other.GetUserData().data;

                if(ce.contact.IsTouching() && other.position.y > this.position.y + TILESIZE / 2)
                    return true;
            }
            return false;
        },

        jump: function() {
            if(!this.isOnGround()) return;

            this.body.ApplyImpulse(new b2Vec2(0, -4), new b2Vec2(0, 0));
            // this.actor.body.m_linearVelocity.y = -8;
            this.body.position.y -= 1;
        }
});
