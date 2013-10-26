var App = new Game();
var clock = new THREE.Clock(true);

var city = new City(100, 100); 
App.addEntity(city);
function mainloop () {
    requestAnimationFrame(mainloop);
    App.update(clock.getDelta());
    App.render();
}

mainloop();