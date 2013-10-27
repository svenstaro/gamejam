var Sound = Class.create(Entity, {
    className: "Sound",
    initialize: function(name) {
    	this.name = name;
    	this.audio = document.createElement('audio');
    	this.source = document.createElement('source');
    	this.source.src = 'data/' + name + '.ogg';
	    this.audio.appendChild(this.source);
	    this.audio.volume = 1.0;
    },

    onAdd: function(scene) {
        scene.add(this.mesh);
    },

    update: function(dt) {

    },

    play: function() {
    	this.audio.play();
    },

});