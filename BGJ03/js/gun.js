PipeGhost = gamvas.Actor.extend({
    create: function(name, tile, normal) {
        this._super(name, tile.position.x, tile.position.y);
        this.tile != tile;
        this.rotation = Math.atan2(normal.y, normal.x);
        this.layer = 119; // a bit above pipes

        var st = gamvas.state.getCurrentState();
        this.setFile(st.resource.getImage('gfx/pipe-outline.png'));
        this.center = new gamvas.Vector2D(0, 16);
    }
});


Gun = gamvas.Actor.extend({
    create: function(name, player) {
        this._super(name, player.x, player.y);
        this.player = player;
        this.cooldown = 0;

        var st = gamvas.state.getCurrentState();
        //this.addAnimation(new gamvas.Animation("idle", st.resource.getImage('gfx/gun.png'), 32, 32, 1, 40));
        //this.setAnimation("idle");
        
        this.image = new gamvas.Image(st.resource.getImage('gfx/gun.png'));
        //this.setFile(st.resource.getImage('gfx/gun.png'));
        
        this.center = new gamvas.Vector2D(3, 15);
        this.image.center = new gamvas.Vector2D(3, 15);
        
        this.layer = -0.1;
        this.ghost = null;

        /*this.getCurrentState().update = function(t) {
            this.actor.aim();
        };*/
    },
    
    draw: function(t)
    {
        this.cooldown -= t;
        var newPosition = new gamvas.Vector2D(this.player.position.x - 1, this.player.position.y+10);
        var newRotation = this.aim();

        if(this.position != newPosition || this.rotation != newRotation) {
            var target = this.castShot();
            var tile = target[0];
            var normal = target[1];

            var isGhostTile = tile && this.ghost && this.ghost.tile == tile;
            if(tile && !isGhostTile && !isBlastOnTile(tile.x, tile.y, normal.x, normal.y)) {
                if(this.ghost) gamvas.state.getCurrentState().removeActor(this.ghost);

                this.ghost = new PipeGhost(nextId("pipeghost-"), tile, normal);
                gamvas.state.getCurrentState().addActor(this.ghost);
            }

            this.position = newPosition;
            this.rotation = newRotation;
            this.image.setPosition(newPosition.x, newPosition.y);
            this.image.setRotation(newRotation);
        }

        this.image.draw();
    },

    aim: function() {
        var m = mousePosition();
        var p = this.position;
        return Math.atan2(m.y - p.y, m.x - p.x);
    },

    shoot: function(mode) {
        if(mode == "primary" && this.isActive()) {
            var target = this.castShot();
            var tile = target[0];
            var normal = target[1];

            if(tile && !isBlastOnTile(tile.x, tile.y, normal.x, normal.y))
                gamvas.state.getCurrentState().addActor(new Blast(nextId("blast-"), tile, normal, false));
        } else if(mode == "secondary" && this.cooldown <= 0) {
            var norm = new b2Vec2(
                    Math.cos(this.rotation),
                    Math.sin(this.rotation));
            gamvas.state.getCurrentState().addActor(new Blast(nextId("user-blast-"), this.player, norm, true));

            norm.Multiply(-1 * RECOIL);
            this.player.body.ApplyImpulse(norm, new b2Vec2());
            this.cooldown = 0.5;
        }
    },

    // returns [tile, normal]
    castShot: function () {
        // cast a ray to find tile
        // var ray = gamvas.physics.getWorld().RayCastOne(
        var first = []; // dist, actor, normal

        var start = new b2Vec2(
                gamvas.physics.toWorld(this.position.x), 
                gamvas.physics.toWorld(this.position.y));


        var wrapper = function(fixture, point, normal, fraction) {
            var actor = fixture.GetBody().GetUserData().data;

            if(actor.type == "collisiontile") {
                var dist = new b2Vec2(point.x - start.x, point.y - start.y);
                var d = dist.Length();

                if(!first[0] || first[0] > d) {
                    // calculate better normal
                    var diff = new b2Vec2(
                            gamvas.physics.toWorld(actor.position.x) - point.x,
                            gamvas.physics.toWorld(actor.position.y) - point.y);
                    var angle = Math.atan2(diff.y, diff.x);
                    angle /= (Math.PI / 2);
                    angle = Math.round(angle);
                    var n = new b2Vec2(0, 0);
                    if(angle == 0) n.x = -1;
                    else if(angle == 1) n.y = -1;
                    else if(angle == 2 || angle == -2) n.x = 1;
                    else if(angle == -1) n.y = 1;
                    first = [d, actor, n];
                }
            }
            return true;
        };

        var mouse = mousePosition();
        mouse.x = gamvas.physics.toWorld(mouse.x);
        mouse.y = gamvas.physics.toWorld(mouse.y);

        var end = new b2Vec2(mouse.x, mouse.y);
        end.Subtract(start);
        end.Normalize();
        end.Multiply(100);
        end.Add(start);

        gamvas.physics.getWorld().RayCast(wrapper, start, end);

        return [ first[1], first[2] ];
    }
});

