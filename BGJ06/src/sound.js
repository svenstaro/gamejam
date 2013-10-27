var Sound = Class.create(Entity, {
    className: "Sound",
    initialize: function(name) {
    	this.name = name;
    	this.audio = document.createElement('audio');
    	this.source = document.createElement('source');
    	this.source.src = 'data/' + name + '.ogg';
	    this.audio.appendChild(this.source);
	    
	},

    onAdd: function(scene) {
        scene.add(this.mesh);
    },

    update: function(dt) {

    },

    play: function() {
    	this.audio.play();
    },

    volume: function(volume) {
    	this.audio.volume = volume;
    },

    loop: function() {
    	if (typeof this.audio.loop == 'boolean') {
    		this.audio.loop = true;
    	} 
    	else {
    		this.audio.addEventListener('ended', function() {
    			this.currentTime = 0;
    			this.play();
    		}, false);
		}
		this.audio.play();
    }

});