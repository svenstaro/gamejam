
Player = gamvas.Actor.extend({
    moveByWind: true,

    create: function(name, x, y) {
        this._super(name, x, y);
        this.type = "player";

        var st = gamvas.state.getCurrentState();
        this.gun = new Gun("gun", this);
        st.addActor(this.gun);
        this.gun.setActive(false);

        this.addAnimation(new gamvas.Animation("start-idle", st.resource.getImage('gfx/idle.png'), 64, 64, 2, 3));
        this.addAnimation(new gamvas.Animation("walk-right", st.resource.getImage('gfx/playerright.png'), 64, 64, 10, 20));
        this.addAnimation(new gamvas.Animation("walk-left", st.resource.getImage('gfx/playerleft.png'), 64, 64, 10, 20));
        this.addAnimation(new gamvas.Animation("walk-left-big", st.resource.getImage('gfx/playerleft_big.png'), 64, 64, 10, 20));
        /*var idleLeft = new gamvas.Animation("idle-left", st.resource.getImage('gfx/playerleft.png'), 64, 64, 10, 10);
        idleLeft.setFrameList([2, 2, 2, 3, 3, 3]);
        this.addAnimation(idleLeft);
        var idleLeft = new gamvas.Animation("idle-right", st.resource.getImage('gfx/playerright.png'), 64, 64, 10, 10);
        idleLeft.setFrameList([2, 2, 2, 3, 3, 3]);
        this.addAnimation(idleLeft);*/
        var idleLeft = new gamvas.Animation("idle-left", st.resource.getImage('gfx/idleleft.png'), 64, 64, 2, 3);
        this.addAnimation(idleLeft);
        var idleLeft = new gamvas.Animation("idle-right", st.resource.getImage('gfx/idleright.png'), 64, 64, 2, 3);
        this.addAnimation(idleLeft);
        
        this.setAnimation("start-idle");
        
        //this.addAnimation(new gamvas.Animation("idle", st.resource.getImage('gfx/idle.png'), 64, 64, 2, 6));
        
        this.lookDirectionRight = true;
        this.walkDirectionRight = true;

        //activatePhysics is the thing now
        
        this.center.x = 32;
        this.center.y = 32;

        this.contacts = new Array();
        this.inAirJump = false;
        this.jumpTimeout = 0;

        this.getCurrentState().update = function(t) {
            this.actor.jumpTimeout -= t;

            var f = 6;

            if (isKeyDown(LEFT_KEYS)) { 
                this.actor.body.m_linearVelocity.x = -f;
                this.actor.walkDirectionRight = false;
                this.actor.gun.layer = 0.1;
                this.actor.gun.image.setScaleXY(1,-1);
                this.actor.gun.setActive(true);
            } else if (isKeyDown(RIGHT_KEYS)) {
                this.actor.body.m_linearVelocity.x = f;
                this.actor.walkDirectionRight = true;
                this.actor.gun.layer = -0.1;
                this.actor.gun.image.setScaleXY(1,1);
                this.actor.gun.setActive(true);
            } else {
                this.actor.body.m_linearVelocity.x *= (1 - t * 8);
            }


            this.actor.lookDirectionRight = (this.actor.gun.rotation > -Math.PI / 2 && this.actor.gun.rotation < Math.PI / 2);

            if(this.actor.gun.isActive()) {
                var a = (Math.abs(this.actor.body.m_linearVelocity.x) <= 0.3 ? "idle" : "walk");
                this.actor.setAnimation(
                        a + "-" + (this.actor.lookDirectionRight ? "right" : "left"));

                if(a == "walk") {
                    var frames = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9];
                    if(this.actor.walkDirectionRight != this.actor.lookDirectionRight)
                        frames.reverse();

                    this.actor.setFrameList(frames);
                }
            }

            if (this.actor.isOnGround() && this.actor.inAirJump && isKeyDown(JUMP_KEYS)) {
                this.actor.jump();
                this.actor.gun.setActive(true);
            }
        };
        
        this.getCurrentState().onCollisionEnter = function(collider) {
            if(collider.type == "collisiontile" || collider.type == "box") {
                this.actor.contacts.push(collider);
            }
        };
        
        this.getCurrentState().onCollisionLeave = function(collider) {
            deleteFromArray(this.actor.contacts, collider);
        };
    },

    reset: function(pos) {
        this.body.m_linearVelocity = new b2Vec2(0,0);
        this.setPosition(pos.x, pos.y);
        this.gun.setActive(false);
        this.setAnimation("start-idle");
    },
    
    activatePhysics: function() {
        //create body
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
        
        this.setFixedRotation(true);
        this.fixture.SetFriction(0);
        this.fixture.SetRestitution(0.01);
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
        if(this.jumpTimeout > 0) 
            return;

        if(!this.isOnGround()) {
            this.inAirJump = true;
            return;
        }
        this.inAirJump = false;
        this.jumpTimeout = 0.2;

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
