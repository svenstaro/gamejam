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
			
			//not so hacky anymore
			this.setFixedRotation(true);
			
            this.getCurrentState().update = function(t) {
                var f = 1;

                if (gamvas.key.isPressed(gamvas.key.LEFT) 
                    || gamvas.key.isPressed(gamvas.key.A)) {
                        this.actor.body.m_linearVelocity.x = -f;
                }
                if (gamvas.key.isPressed(gamvas.key.RIGHT)
                    || gamvas.key.isPressed(gamvas.key.D)) {
                        // this.actor.body.m_linearVelocity.x = f;
                        this.actor.body.m_force.x = f;
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
        }
});
