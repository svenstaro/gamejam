BlastEmissionState = gamvas.ActorState.extend({
    create: function() {
        this._super("emission");
        this.lifetime = 0;
    },

    update: function(t) {
        this.lifetime += t;

        if(!this.actor.freeMode) {
            this.actor.center.x = (1 - Math.min(this.lifetime, 0.2) * 5) * TILESIZE;
        }

        var t = this.lifetime / 2.0;
        var min = -0.8 * t * t + 1.6 * t;
        min = t - 0.2;
        var max = min + 0.2;
        
        var targets = this.actor.castRays();
        for(var i = 0; i < targets.length; ++i) {
            var actor = targets[i][0];
            var d = targets[i][1];

            // over lifetime, the range of d's varies
            // p   -> min - max
            // 0.0 -> 0 - 0.2
            // 1.0 -> 0.8 - 1
            var p = d / 2.0;
            if(p < min || p > max) continue;

            var diff = new b2Vec2(
                gamvas.physics.toWorld(actor.position.x - this.actor.position.x),
                gamvas.physics.toWorld(actor.position.y - this.actor.position.y));

            diff.Normalize();
            diff.Multiply(Math.pow(1 - d, 2) * this.actor.windForce);
            actor.body.ApplyForce(diff, new b2Vec2(1, 1));
        }

        if(this.lifetime > 2) {
            this.actor.setState("die");
        }
    }
});

DyingState = gamvas.ActorState.extend({
    create: function(dieTime) {
        this._super("die");
        this.dieTime = dieTime || 2;
        this.timeLeft = this.dieTime;
    },

    enter: function() {
        this.actor.dyingProgress = 0;
    },

    update: function(t) {
        this.actor.dyingProgress = 1 - this.timeLeft / this.dieTime;
        this.timeLeft -= t;

        if(!this.actor.freeMode) {
            this.actor.center.x = this.actor.dyingProgress * TILESIZE;
        }

        if(this.timeLeft < 0) {
            gamvas.state.getCurrentState().removeActor(this.actor.particles);
            gamvas.state.getCurrentState().removeActor(this.actor);
        }
    }
});

function isBlastOnTile(x, y, nx, ny) {
    var actors = gamvas.state.getCurrentState().actors;
    for(var i in actors) {
        var actor = actors[i];
        if(actor.type == "blast" && !actor.freeMode) {
            if(actor.tile.x == x && actor.tile.y == y && actor.normal.x == nx && actor.normal.y == ny)
                return true;
        }
    }
    return false;
}

Blast = gamvas.Actor.extend({
    create: function(name, parent, normal, freeMode) {
        this._super(name, 
            parent.position.x, parent.position.y);
            //parent.position.x + normal.x * TILESIZE / 2, 
            //parent.position.y + normal.y * TILESIZE / 2);
        this.type = "blast";
        this.rotation = Math.atan2(normal.y, normal.x);
        this.normal = normal;
        this.freeMode = freeMode;
        if(!freeMode) this.tile = parent;
        this.layer = 120;

        if(freeMode) {
            this.windForce = WIND_FORCE_PLAYER;
        } else {
            if(Math.abs(normal.x) > Math.abs(normal.y)) {
                this.windForce = WIND_FORCE_HORIZONTAL;
            } else {
                this.windForce = WIND_FORCE_VERTICAL;
            }
        }

        var st = gamvas.state.getCurrentState();
        if(!freeMode) {
            this.addAnimation(new gamvas.Animation("idle", st.resource.getImage('gfx/pipe.png'), 32, 32, 1, 40));
            this.setAnimation("idle");
        }
        this.center = new gamvas.Vector2D(0, 16);

        this.winds = 0;
        this.addState(new BlastEmissionState());
        this.addState(new DyingState(0.2));
        this.setState("emission");

        this.particles = new Wind(this.position.x, this.position.y, this.rotation, this.normal);
        gamvas.state.getCurrentState().addActor(this.particles);
    },

    // returns a list of pairs like [actor, fraction]
    castRays: function() {
        var list = [];
        var blast = this;

        var start = new b2Vec2(
                gamvas.physics.toWorld(this.position.x), 
                gamvas.physics.toWorld(this.position.y));

        for(var angle = this.rotation - BLAST_ANGLE / 2; 
                angle <= this.rotation + BLAST_ANGLE / 2; 
                angle += BLAST_ANGLE / BLAST_RAYS) {

            var wrapper = function(fixture, point, normal, fraction) {
                var actor = fixture.GetBody().GetUserData().data;

                if(actor.moveByWind && !(blast.freeMode && actor.type == "player")) {
                    var inList = false;
                    for(var i in list) {
                        if(list[i][0] == actor) {
                            inList = true;
                            if(list[i][1] > fraction) list[i][1] = fraction;
                        }
                    }
                    if(!inList) {
                        list.push([actor, fraction]);
                    }
                }
                return true; // !actor.blockWind;
            };

            var end = new b2Vec2(
                    start.x + Math.cos(angle) * BLAST_DISTANCE, 
                    start.y + Math.sin(angle) * BLAST_DISTANCE);

            gamvas.physics.getWorld().RayCast(wrapper, start, end);
        }
        return list;
    }
});

