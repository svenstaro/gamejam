var App = new Game();
var clock = new THREE.Clock(true);
var debug = false;

var rainbow_color = [];
rainbow_color[0] = new THREE.Color("red");
rainbow_color[1] = new THREE.Color("orange");
rainbow_color[2] = new THREE.Color("yellow");
rainbow_color[3] = new THREE.Color("green");
rainbow_color[4] = new THREE.Color("blue");
rainbow_color[5] = new THREE.Color("indigo");
rainbow_color[6] = new THREE.Color("violet");

var city = new City(100, 100);
App.addEntity(city);
function mainloop () {
    requestAnimationFrame(mainloop);
    App.update(clock.getDelta());
    App.render();
}

document.addEventListener('mousemove', function(event) {
    App.mouse.set((event.clientX / window.innerWidth) * 2 - 1,
                  App.mouse.y = - (event.clientY / window.innerHeight) * 2 + 1);
}, false);

document.addEventListener('mousedown', function(event) {
    App.tank.shoot();
}, false);

document.addEventListener('keydown', function(event) {
    if(event.keyCode == 67)
        debug = !debug;
})
mainloop();
