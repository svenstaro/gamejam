var App = new Game();
var clock = new THREE.Clock(true);
var building = new Building(THREE.Math.randInt(0, 100), THREE.Math.randInt(0, 100));
App.addEntity(building);
function mainloop () {
    requestAnimationFrame(mainloop);
    App.update(clock.getDelta());
    App.render();
}

document.addEventListener('mousemove', onDocumentMouseMove, false);

function onDocumentMouseMove(event) {
    // event.preventDefault();
    App.mouse.set(( event.clientX / window.innerWidth ) * 2 - 1, App.mouse.y = - ( event.clientY / window.innerHeight ) * 2 + 1);
}

mainloop();
