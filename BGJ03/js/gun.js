// create our collision objects
Gun = gamvas.Actor.extend({
    create: function(name, player) {
        this._super(name, player.x, player.y);
        this.player = player;

        var st = gamvas.state.getCurrentState();
        this.addAnimation(new gamvas.Animation("idle", st.resource.getImage('gfx/gun.png'), 32, 32, 1, 40));
        this.setAnimation("idle");
        this.center = new gamvas.Vector2D(16, 16);

        this.getCurrentState().update = function(t) {
            this.actor.aim();
        };
    },

    aim: function() {
        var m = gamvas.mouse.getPosition();
        var p = this.position;
        this.rotation = Math.atan2(m.y - p.y, m.x - p.x);
    },

    shoot: function(mode) {
        if(mode == "primary") {
            // cast a ray to find tile
            // var ray = gamvas.physics.getWorld().RayCastOne(
            var first = []; // dist, actor, normal

            var start = new b2Vec2(
                    gamvas.physics.toWorld(this.position.x), 
                    gamvas.physics.toWorld(this.position.y));


            var wrapper = function(fixture, point, normal, fraction) {
                var actor = fixture.GetBody().GetUserData().data;

                if(actor.type == "tile") {
                    var dist = new b2Vec2(point.x, point.y);
                    dist.Subtract(start);

                    if(!first[0] || first[0] > dist) {
                        first = [dist, actor, normal];
                    }
                }
                return true;
            };

            var mouse = new b2Vec2(
                    gamvas.physics.toWorld(gamvas.mouse.getX()),
                    gamvas.physics.toWorld(gamvas.mouse.getY()));

            var end = new b2Vec2(mouse.x, mouse.y);
            end.Subtract(start);
            end.Normalize();
            end.Multiply(100);
            end.Add(start);

            gamvas.physics.getWorld().RayCast(wrapper, start, end);
            if(first[1]) 
                gamvas.state.getCurrentState().addActor(new Blast("blast", first[1], first[2]));
        } else if(mode == "secondary") {

        }
    }
});
