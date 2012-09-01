MainState = gamvas.State.extend({
        init: function() {
            gamvas.physics.pixelsPerMeter = 32;

            // disable object sleeping (third parameter is false)
            var w = gamvas.physics.resetWorld(0, 0, false);
            gamvas.physics.setGravity(new gamvas.Vector2D(0, 1));

            this.addActor(new Player("player", 0, 0));
        },

        draw: function(t) {
            gamvas.physics.drawDebug();
        },

        onMouseDown: function(b, x, y) {
            if (b == gamvas.mouse.LEFT) {
                var worldMouse = this.camera.toWorld(x, y);
            }
        },

        onKeyDown: function(k, c, e) {
            if(k == gamvas.key.R) {
                document.location.reload(true);
            }
        }
});
//
// fire up our game
gamvas.event.addOnLoad(function() {
    gamvas.state.addState(new MainState('MainState'));
    gamvas.start('gameCanvas', true);
});
