var App = new Game();
var clock = new THREE.Clock(true);
var building = new Building(THREE.Math.randInt(-10, 10), THREE.Math.randInt(-10, 10));
App.addEntity(building);
function mainloop () {
    requestAnimationFrame(mainloop);
    App.update(clock.getDelta());
    App.render();
}

mainloop();