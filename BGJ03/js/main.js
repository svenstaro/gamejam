mainState = gamvas.State.extend({
        init: function() {
            gamvas.physics.pixelsPerMeter = 32;

            // disable object sleeping (third parameter is false)
            var w = gamvas.physics.resetWorld(0, 9.8, false);
        },

        draw: function(t) {
            if (gamvas.key.isPressed(gamvas.key.LEFT)) {
            }
            if (gamvas.key.isPressed(gamvas.key.RIGHT)) {
            }
            if (gamvas.key.isPressed(gamvas.key.UP)) {
            }
            if (gamvas.key.isPressed(gamvas.key.DOWN)) {
            }
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
    gamvas.state.addState(new mainState('mainState'));
    gamvas.start('gameCanvas', true);
});
