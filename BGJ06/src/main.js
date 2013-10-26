var App = new Game();
var clock = new THREE.Clock(true);
function mainloop () {
    requestAnimationFrame(mainloop);
    App.update(clock.getDelta());
    App.render();
}

mainloop();
