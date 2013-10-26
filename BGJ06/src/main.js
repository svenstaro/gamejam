var App = new Game();
var clock = new THREE.Clock(true);
var building = new Building(THREE.Math.randInt(0, 100), THREE.Math.randInt(0, 100));
App.addEntity(building);
function mainloop () {
    requestAnimationFrame(mainloop);
    App.update(clock.getDelta());
    App.render();
}

mainloop();