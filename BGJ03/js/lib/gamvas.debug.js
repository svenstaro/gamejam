/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas
 *
 * Description:
 *
 * Basic functions for handling the gamvas environment: http://gamvas.com
 *
 * Gamvas is a HTML5 game development framework/engine that simplifies the task
 * of developing games. It keeps the developer away from the boring tasks and
 * allows you to concentrate on the gameplay instead of spending weeks coding
 * just to get you a useful base framework for your game.
 *
 * The framework uses upper case words for classes, lowercase words for static
 * functions. For example, see <gamvas.State> vs <gamvas.state>
 *
 * While the use of static functions won't win Gamvas a award in object
 * oriented software design, it will in certain situations improve performance
 * and reduce necessary amount of source code and therfor further speed up
 * your development process.
 *
 * At the moment Gamvas is more like a framework for developing games with the
 * HTML5 canvas element and you'll need quite a bit of JavaScript knowledge to use it, although
 * i hope that the documentation and examples make it even possible for JavaScript
 * beginners to get their dream game to life.
 *
 * Gamvas is planned to evolve into a full game engine though with native graphical editors
 * on Linux, Mac OSX and Windows. I want to make this system as useful as it can get
 * for the developers, so future features depend on the requirements. Feel free
 * to contact me with feature requests at hi@93-interactive.com
 *
 */
gamvas = {
    _canvas: null,
    _context2D: null,
    _world: null,
    _usePhys: false,
    /*
     * Variable: baseURL
     *
     * The place there the gamvas scripts live
     * Will be filled on startup
     */
    baseURL: '',

    /*
     * Function: start
     *
     * Description:
     *
     * Starts the gamvas system
     *
     * Parameters:
     *
     * canvasID - The id of the canvas element to start gamvas on
     * usePhysics - Use physics (requires box2dweb JavaScript)
     * doSleep - If using physics, allow objects to 'fall asleep' when not active anymore
     *
     * Example:
     *
     * A typical gamvas start, add a onload handler that adds your main game state and starts
     * gamvas on the canvas with id "myCanvas" and enables physics support
     *
     * > gamvas.event.addOnLoad(function() {
     * >    gamvas.state.addState(myState);
     * >    gamvas.start('myCanvas', true);
     * > });
     */
    start: function(canvasid, usePhysics, doSleep) {
        if (gamvas.isSet(usePhysics)) {
            gamvas._usePhys = usePhysics;
            var slp = doSleep;
            if (!gamvas.isSet(doSleep)) {
                slp = true;
            }
            gamvas.physics.resetWorld(0, 9.8, slp);
        }

        gamvas._canvas = document.getElementById(canvasid);
        if (!gamvas.isSet(gamvas._canvas)) {
            console.log('canvas #'+canvasid+' not found, aboring gamvas');
            return;
        }

        gamvas._canvas.oncontextmenu=function() {return false;};
        gamvas._context2D = gamvas._canvas.getContext("2d");

        window.requestAnimationFrame = (function(cb) {
            return window.requestAnimationFrame ||
            window.webkitRequestAnimationFrame ||
            window.mozRequestAnimationFrame ||
            window.oRequestAnimationFrame ||
            window.msRequestAnimationFrame ||
            function(cb){
                window.setTimeout(cb, 1000 / 60);
            };
        })();

        gamvas.state.setup();
        gamvas.event.onReady();

        window.requestAnimationFrame(gamvas.screen.redraw);
    },

    /*
     * Function: getCanvasPosition
     *
     * Description:
     *
     * Get the position in DOM of the current canvas
     *
     * Returns:
     *
     * <gamvas.Vector2D>
     *
     * See:
     *
     * <gamvas.dom.getPosition>
     */
    getCanvasPosition: function() {
        return gamvas.dom.getPosition(gamvas._canvas);
    },

    /*
     * Function: getCanvasDimension
     *
     * Description:
     *
     * Get the width and height of the canvas
     *
     * Returns:
     *
     * Object {w, h} with with and height of the canvas
     *
     * Example:
     *
     * > var d = gamvas.getCanvasDimension();
     * > alert('canvas has a width of '+d.w+' pixels');
     */
    getCanvasDimension: function() {
        return {
		w: gamvas._canvas.width,
		h: gamvas._canvas.height
	};
    },

    /*
     * Function: getCanvas
     *
     * Description:
     *
     * Get the canvas element
     *
     * Returns:
     *
     * Canvas DOM element
     */
    getCanvas: function() {
        return gamvas._canvas;
    },

    /*
     * Function: getContext2D
     *
     * Description:
     *
     * Get the 2D context object of the canvas
     *
     * Returns:
     *
     * The 2D context of the canvas
     *
     * See:
     *
     * http://dev.w3.org/html5/2dcontext/
     */
    getContext2D: function() {
        return gamvas._context2D;
    },

    /*
     * Function: isSet
     *
     * Description:
     *
     * Tests if a variable is set
     *
     * Parameters:
     *
     * v - the variable
     *
     * Returns:
     *
     * true or false
     *
     * Example:
     *
     * > var el = document.getElementById('myelement');
     * > if (!gamvas.isSet(el)) {
     * >     alert('myelement not found');
     * > }
     */
    isSet: function(v) {
        if ( (typeof v == 'undefined') || (v===null) ) return false;
        return true;
    }
};

var tmpScr = document.getElementsByTagName('script');
gamvas.baseURL = tmpScr[tmpScr.length - 1].src.replace(/\\/g, '/').replace(/\/[^\/]*$/, '');
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.Class
 *
 * Description:
 *
 * The basic class for inheritance
 *
 * Use this to make objects that can be extended
 *
 * Note:
 *
 * Compared to classic JavaScript inheritance, this method
 * is a very user friendly but performance costly way, so gamvas
 * uses this class only for objects that you are supposed to overwrite
 * and that are not created on a per frame basis, and so should you
 *
 * See:
 *
 * <gamvas.Actor>
 * <gamvas.ActorState>
 * <gamvas.State>
 *
 * Example:
 *
 * > myExtendableObject = gamvas.Class.extend({
 * >    create: function(param) {
 * >        // call super constructor
 * >        this._super(param);
 * >        // do our constructor stuff
 * >        this._par = param;
 * >    },
 * >    debug: function() {
 * >        console.log(this._par);
 * >    }
 * > });
 * > var obj = new myExtendableObject('test');
 * > obj.debug();
 */
(function() {
    var creating = false;
    gamvas.Class = function(){};
    gamvas.Class.extend = function(prop) {
        var _super = this.prototype;
        creating = true;
        var p = new this();
        creating = false;
        for (var name in prop) {
            if ( (typeof _super[name] == 'function') && (typeof prop[name] == 'function') ) {
                p[name] = (function(name, fn){
                    return function() {
                    var tmp = this._super;
                    this._super = _super[name];
                    var ret = fn.apply(this, arguments);       
                    this._super = tmp;
                   
                    return ret;
                };})(name, prop[name]);
            } else {
                p[name] = prop[name];
            }
        }
        ret = function() {
            if ( (!creating) && (typeof this.create == 'function') ) {
                this.create.apply(this, arguments);
            }
        };
        ret.prototype = p;
        ret.prototype.constructor = gamvas.Class;
        ret.extend = this.extend;
        return ret;
    };
})();
/*
 * Function: extend
 *
 * Description:
 *
 * Exend a class
 *
 * Parameters:
 *
 * A object defining the extending class
 *
 * Returns:
 *
 * A new class to be used for instancing
 *
 * Note:
 *
 * The special function 'create' is used as constructor. If you overwriting 'create'
 * you always should call 'this._super(params)' as first command in 'create'
 */
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.config
 *
 * Several userconfigurable variables
 */
gamvas.config = {
        /*
         * Variable: preventKeyEvents
         *
         * Capture the whole documents key events?
         *
         * If true, every key event on the page will be handled by gamvas.
	 * This disables scrolling with cursor keys and other keyboard shortcuts.
	 * When false, only keys that are explicitly handled by the running game
	 * are not given to the browser, everything else is put through
         *
         * Default:
         * true
         */
	preventKeyEvents: true,

        /*
         * Variable: preventMouseEvents
         *
         * Capture the whole documents mouse events?
         *
         * If true, every mouse event on the page will be handled by gamvas.
	 * You can not select input fields, or click on the scrollbar.
	 * When false, only if the event handler like for example <gamvas.actor.onKeyDown>
	 * returns false, the mouse event will not be put through to the browser.
         *
         * Default:
         * true
         */
	preventMouseEvents: true,
};
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.Resource
 *
 * Description:
 *
 * Class for resource handling, aka loading images and other game data
 *
 * Constructor:
 *
 * new gamvas.Resource();
 */
gamvas.Resource = function() {
    this._finished = true;
    this.cache = false;
    this._images = [];
    this._sounds = [];
    this.ret = 0;
    this._toLoad = 0;
    this._loaded = 0;
};

/*
 * Function: getImage
 *
 * Description:
 *
 * Load a image from url
 *
 * Parameters:
 *
 * url - The image url
 *
 * Returns:
 *
 * A javascript image object, that might be not fully loaded yet
 *
 * Example:
 *
 * > myState = gamvas.State.extend({
 * >     init: function() {
 * >         this.img = this.resource.getImage('myImage.png');
 * >     },
 * >     draw: function(t) {
 * >         if (this.resource.done()) { // everything loaded?
 * >             // do something with this.img
 * >         } else { // data is still loading
 * >             // print current loading status in percent
 * >             this.c.fillText("Loading... "+(100*this.resource.status())+"%", 10, 10);
 * >         }
 * >     }
 * > });
 */
gamvas.Resource.prototype.getImage = function(url) {
    this._finished = false;
    this._toLoad++;
    var img = new Image();
    img.src = url;
    this._images.push(img);
    return img;
};

/*
 * Function: getSound
 *
 * Description:
 *
 * Load a sound/music file from url
 *
 * Parameters:
 *
 * url - The audio file url
 *
 * Returns:
 *
 * A javascript Audio object, that might be not fully loaded yet
 *
 * Example:
 *
 * > myState = gamvas.State.extend({
 * >     create: function(name) {
 * >         this._super(name);
 * >         this.snd = this.resource.getSound('pling.wav');
 * >     },
 * >     draw: function(t) {
 * >         if (this.resource.done()) { // everything loaded?
 * >              // play the sound
 * >              gamvas.sound.play(this.snd);
 * >         } else { // data is still loading
 * >             // print current loading status in percent
 * >             this.c.fillText("Loading... "+(100*this.resource.status())+"%", 10, 10);
 * >         }
 * >     }
 * > });
 */
gamvas.Resource.prototype.getSound = function(url) {
    this._finished = false;
    this._toLoad++;
    var snd = new Audio();
    snd.src = url;
    snd.loop = false;
    this._sounds.push(snd);
    return snd;
};

gamvas.Resource.prototype._calculate = function() {
    if (this.cache) {
        return this._loaded;
    }
    var incomplete = false;
    var newimages = [];
    for (var i = 0; i < this._images.length; i++) {
        if (this._images[i].complete) {
            this._loaded++;
        } else {
            newimages.push(this._images[i]);
            incomplete = true;
        }
    }
    delete this._images;
    this._images = newimages;

    var newsounds = [];
    for (var i = 0; i < this._sounds.length; i++) {
        switch (this._sounds[i].readyState) {
            case HTMLMediaElement.HAVE_CURRENT_DATA:
            case HTMLMediaElement.HAVE_FUTURE_DATA:
            case HTMLMediaElement.HAVE_ENOUGH_DATA:
                this._loaded++;
                break;
            default:
                newsounds.push(this._sounds[i]);
                incomplete = true;
                break;
        }
    }
    delete this._sounds;
    this._sounds = newsounds;

    if (incomplete == false) {
        this._finished = true;
    }
    this.cache = true;
};

/*
 * Function: status
 *
 * Description:
 *
 * Get the status of all loading resources
 *
 * Returns:
 *
 * A value between 0 and 1 depending on how much of the resources are loaded
 *
 * See:
 * <gamvas.Resource.getImage>
 */
gamvas.Resource.prototype.status = function() {
    if (this._finished) return 1.0;
    if (this._toLoad === 0) return 1.0;
    this._calculate();
    return this._loaded/this._toLoad;
};

/*
 * Function: done
 *
 * Description:
 *
 * Are all resources loaded?
 *
 * Returns:
 *
 * true or false
 *
 * See:
 * <gamvas.Resource.getImage>
 */
gamvas.Resource.prototype.done = function() {
    if (this._finished) {
        return true;
    }
    this._calculate();
    return this._finished;
};
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.Rect
 *
 * Holds position and width information of a rectangle
 *
 * Constructor:
 *
 * new gamvas.Rect(x, y, w, h);
 *
 * Parameters:
 *
 * x/y - top left upper corner of the rectangle (optional)
 * w/h - the width and height of the rectangle (optional)
 */
gamvas.Rect = function(x, y, w, h) {
    /*
     * Variable: x
     *
     * The x position of the upper left corner of the rectangle
     */
    this.x = (x) ? x : 0;

    /*
     * Variable: y
     *
     * The y position of the upper left corner of the rectangle
     */
    this.y = (y) ? y : 0;

    /*
     * Variable: width
     *
     * The width of the rectangle
     */
    this.width = (w) ? w : 0;

    /*
     * Variable: height
     *
     * The height of the rectangle
     */
    this.height = (h) ? h : 0;
};
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.Vector2D
 *
 * Description:
 *
 * A 2D vector class
 *
 * Constructor:
 *
 * new gamvas.Vector2D(x, y);
 *
 * Parameters:
 *
 * x - The x part of the vector (optional)
 * y - The y part of the vector (optional)
 */
gamvas.Vector2D = function(x, y) {
    /*
     * Variable: x
     *
     * Description:
     *
     * The x direction of the vector, also used as coordinate value for some functions.
     */
    (x) ? this.x = x : this.x = 0;

    /*
     * Variable: y
     *
     * Description:
     *
     * The y direction of the vector, also used as coordinate value for some functions.
     */
    (y) ? this.y = y : this.y = 0;
};

/*
 * Function: length
 *
 * Description:
 *
 * Return the length of the vector in pixels.
 * This calculates the correct length of a vector which
 * uses sqrt() and therefor is slow. If you just want
 * compare two vectors, see <gamvas.Vector2D.quickLength>
 *
 * See:
 * <gamvas.Vector2D.quickLength>
 */
gamvas.Vector2D.prototype.length = function() {
    return Math.sqrt(this.x*this.x+this.y*this.y);
};

/*
 * Function: quickLength
 *
 * Description:
 *
 * Returns the non sqrt() length of the vector, which is faster
 * to calculate as it's real length. Use it if you want to compare
 * to vectors in length. If you need the actual length of the vector,
 * use the slower <gamvas.Vector2D.length>
 *
 * See:
 * <gamvas.Vector2D.length>
 */
gamvas.Vector2D.prototype.quickLength = function() {
    return this.x*this.x+this.y*this.y;
};

/*
 * Function: normalized
 *
 * Description:
 *
 * Returns the normalized vector of a vector. The normalized
 * vector is the vector with length of 1.
 *
 * Returns:
 *
 * <gamvas.Vector2D>
 */
gamvas.Vector2D.prototype.normalized = function() {
    var ret = new gamvas.Vector2D();
    var l = this.length();
    ret.x = this.x/l;
    ret.y = this.y/l;
    return ret;
};

/*
 * Function: rotate
 *
 * Description:
 *
 * Returns a new vector with the current vector rotated a certain angle. The angle is in radians.
 *
 * Parameters:
 *
 * r - Rotation in radians
 *
 * Returns:
 *
 * <gamvas.Vector2D>
 *
 * See:
 *
 * http://en.wikipedia.org/wiki/Radians
 */
gamvas.Vector2D.prototype.rotate = function(r) {
    var ret = new gamvas.Vector2D();
    var s = Math.sin(r);
    var c = Math.cos(r);
    ret.x = c*this.x-s*this.y;
    ret.y = s*this.x+c*this.y;
    return ret;
};
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.dom
 *
 * A collection of static DOM helper and utility functions
 */
gamvas.dom = {
    /*
     * Function: getPosition
     * 
     * Gets the position of a DOM element
     *
     * Parameters:
     *    el - The DOM element
     *
     * Returns:
     *    <gamvas.Vector2D>
     *
     * Example:
     * > var el = document.getElementById('myelement');
     * > var pos = gamvas.dom.getPosition(el);
     * > alert('myelement is at position '+pos.x+','+pos.y);
     */
    getPosition: function(el) {
        var pos = [0, 0];
        gamvas.dom.getPositionRec(el, pos);
        return new gamvas.Vector2D(pos[0], pos[1]);
    },

    getPositionRec: function(el, pos) {
        pos[0] += el.offsetLeft;
        pos[1] += el.offsetTop;
        if (el.offsetParent) {
            gamvas.dom.getPosition(el.offsetParent, pos);
        }
    }
};
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.event
 *
 * Functions for event handling
 */
gamvas.event = {
    _onLoadFunctions: [],
    _onLoadRan: false,
    _onReadyFunctions: [],
    _onReadyRan: false,

    /*
     * Function: addOnLoad
     *
     * Description:
     *
     * Add a function to be called when the HTML document is loaded
     *
     * Example:
     *
     * This function is usually used for initializing gamvas like following:
     *
     * > gamvas.event.addOnLoad(function() {
     * >    gamvas.state.addState(myState);
     * >    gamvas.start('myCanvas', true);
     * > }
     */
    addOnLoad: function(fnc) {
        if (!gamvas.event._onLoadRan) {
            gamvas.event._onLoadFunctions.push(fnc);
        } else {
            fnc();
        }
    },

    /*
     * Function: addOnReady
     *
     * Description:
     *
     * Add a function to be called when gamvas is ready and right before
     * rendering the first frame.
     *
     * Example:
     *
     * > gamvas.event.addOnReady(function() {
     * >     alert('Everything is up, we're ready to roll!');
     * > }
     */
    addOnReady: function(fnc) {
        if (!gamvas.event._onReadyRan) {
            gamvas.event._onReadyFunctions.push(fnc);
        } else {
            fnc();
        }
    },

    onLoad: function() {
        gamvas.event._onLoadRan = true;
        for (var i in gamvas.event._onLoadFunctions) {
            gamvas.event._onLoadFunctions[i]();
        }
    },

    onReady: function() {
        gamvas.event._onReadyRan = true;
        for (var i in gamvas.event._onReadyFunctions) {
            gamvas.event._onReadyFunctions[i]();
        }
    }
};

window.onload = gamvas.event.onLoad;
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.timer
 *
 * Functions to get information about timing. Mostly for internal use.
 */
gamvas.timer = {
    _date: new Date(),
    _ts: 1,

    /*
     * Function: getSeconds
     *
     * Description:
     *
     * Get the seconds since gamvas was started.
     */
    getSeconds: function() {
        return gamvas.timer.getMilliseconds() / 1000.0;
    },

    /*
     * Function: getMilliseconds
     *
     * Description:
     *
     * Get the milliseconds since gamvas was started.
     */
    getMilliseconds: function() {
        var d = new Date();
        return (d.getTime() - gamvas.timer._date.getTime())*gamvas.timer._ts;
    },

    /*
     * Function: setGlobalTimeScale
     *
     * Description:
     *
     * Scale the global time. Use it to achieve slow motion or fast forward effects.
     * Time scale must be >= 0
     *
     * Note:
     *
     * Does also 'scale' the frames per second value returned by <gamvas.screen.getFPS>.
     * So if you set a global time scale of 0.5, <gamvas.screen.getFPS> will return 120
     * when actually running on 60 fps
     *
     * Parameters:
     *
     * s - time scale factor (1 = normal, < 1 = slower, > 1 = faster)
     */
    setGlobalTimeScale: function(s) {
        gamvas.timer._ts = s;
    },

    /*
     * Function: getGlobalTimeScale
     *
     * Description:
     *
     * Returns the global timescale
     *
     * See:
     *
     * <gamvas.timer.setGlobalTimeScale>
     */
    getGlobalTimeScale: function() {
        return gamvas.timer._ts;
    }
};
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.Sound
 *
 * Description:
 *
 * A class for sound and music files.
 *
 * Constructur:
 *
 * new gamvas.Actor(file);
 *
 * Parameters:
 *
 * file - a JavaScript Audio class
 *
 * See:
 *
 * <gamvas.State.addSound>
 *
 * Example:
 *
 * > myState = gamvas.State.extend({
 * >     init: function() {
 * >         this.sound = this.addSound("introsound.wav");
 * >     },
 * >     onKeyDown: function() {
 * >         this.sound.play();
 * >     }
 * > });
 */
gamvas.Sound = function(file) {
    this._file = file;
};

/*
 * Function: play
 *
 * Description:
 *
 * Play the sound once, restarts the sound automatically
 *
 * See:
 * <gamvas.Sound.stop>
 * <gamvas.Sound.resume>
 */
gamvas.Sound.prototype.play = function() {
    if (this.isReady()) {
        this._file.currentTime = 0;
        this._file.play();
    }
};

/*
 * Function: loop
 *
 * Description:
 *
 * Play the sound looping (e.g. for background music)
 *
 * See:
 * <gamvas.Sound.stop>
 */
gamvas.Sound.prototype.loop = function() {
    if (this.isReady()) {
        this._file.currentTime = 0;
        this._file.loop = true;
        this._file.play();
    }
};

/*
 * Function: stop
 *
 * Description:
 *
 * Stops a playing sound
 *
 * See:
 * <gamvas.Sound.resume>
 */
gamvas.Sound.prototype.stop = function() {
    if (this.isReady()) {
        this._file.pause();
    }
};

/*
 * Function: resume
 *
 * Description:
 *
 * Resumes a stopped sound, without rewinding it
 *
 * See:
 * <gamvas.Sound.stop>
 * <gamvas.Sound.play>
 */
gamvas.Sound.prototype.resume = function() {
    if (this.isReady()) {
        this._file.play();
    }
};

/*
 * Function: setRate
 *
 * Description:
 *
 * Set the playback speed of the sound
 *
 * Parameters:
 *
 * r - the new speed (1 = normal, < 1 = faster, > 2 = slower)
 *
 * Note:
 *
 * Playback quality on none standard speeds differs drastically between browsers
 */
gamvas.Sound.prototype.setRate = function(r) {
    this._file.playbackRate = r;
};

/*
 * Function: setVolume
 *
 * Description:
 *
 * Set the volume of the sound
 *
 * Parameters:
 *
 * v - the new volume between 0 and 1
 */
gamvas.Sound.prototype.setVolume = function(v) {
    this._file.volume = v;
};

/*
 * Function: mute
 *
 * Description:
 *
 * Mute the sound
 */
gamvas.Sound.prototype.mute = function() {
    this._file.muted = true;
};

/*
 * Function: unmute
 *
 * Description:
 *
 * Unmute the sound
 */
gamvas.Sound.prototype.unmute = function() {
    this._file.muted = false;
};

/*
 * Function: isReady
 *
 * Description:
 *
 * Test if the sound is ready to play
 *
 * Returns:
 *
 * true/false
 */
gamvas.Sound.prototype.isReady = function() {
    switch (this._file.readyState) {
        case HTMLMediaElement.HAVE_CURRENT_DATA:
        case HTMLMediaElement.HAVE_FUTURE_DATA:
        case HTMLMediaElement.HAVE_ENOUGH_DATA:
            return true;
            break;
        default:
            return false;
    }
};
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.key
 *
 * Keyboard utilities and defines.
 *
 * The defines are listed without further description, should be pretty self explanatory.
 */
gamvas.key = {
    /* Variable: BACKSPACE */
    BACKSPACE: 8,
    /* Variable: TAB */
    TAB: 9,
    /* Variable: RETURN */
    RETURN: 13,
    /* Variable: SHIFT */
    SHIFT: 16,
    /* Variable: CTRL */
    CTRL: 17,
    /* Variable: ALT */
    ALT: 18,
    /* Variable: PAUSE */
    PAUSE: 19,
    /* Variable: BREAK */
    BREAK: 19,
    /* Variable: CAPSLOCK */
    CAPSLOCK: 20,
    /* Variable: ESCAPE */
    ESCAPE: 27,
    /* Variable: SPACE */
    SPACE: 32,
    /* Variable: PAGE_UP */
    PAGE_UP: 33,
    /* Variable: PAGE_DOWN */
    PAGE_DOWN: 34,
    /* Variable: END */
    END: 35,
    /* Variable: HOME */
    HOME: 36,
    /* Variable: LEFT */
    LEFT: 37,
    /* Variable: UP */
    UP: 38,
    /* Variable: RIGHT */
    RIGHT: 39,
    /* Variable: DOWN */
    DOWN: 40,
    /* Variable: INSERT */
    INSERT: 45,
    /* Variable: DELETE */
    DELETE: 46,
    /* Variable: NUM_0 */
    NUM_0: 48,
    /* Variable: NUM_1 */
    NUM_1: 49,
    /* Variable: NUM_2 */
    NUM_2: 50,
    /* Variable: NUM_3 */
    NUM_3: 51,
    /* Variable: NUM_4 */
    NUM_4: 52,
    /* Variable: NUM_5 */
    NUM_5: 53,
    /* Variable: NUM_6 */
    NUM_6: 54,
    /* Variable: NUM_7 */
    NUM_7: 55,
    /* Variable: NUM_8 */
    NUM_8: 56,
    /* Variable: NUM_9 */
    NUM_9: 57,
    /* Variable: A */
    A: 65,
    /* Variable: B */
    B: 66,
    /* Variable: C */
    C: 67,
    /* Variable: D */
    D: 68,
    /* Variable: E */
    E: 69,
    /* Variable: F */
    F: 70,
    /* Variable: G */
    G: 71,
    /* Variable: H */
    H: 72,
    /* Variable: I */
    I: 73,
    /* Variable: J */
    J: 74,
    /* Variable: K */
    K: 75,
    /* Variable: L */
    L: 76,
    /* Variable: M */
    M: 77,
    /* Variable: N */
    N: 78,
    /* Variable: O */
    O: 79,
    /* Variable: P */
    P: 80,
    /* Variable: Q */
    Q: 81,
    /* Variable: R */
    R: 82,
    /* Variable: S */
    S: 83,
    /* Variable: T */
    T: 84,
    /* Variable: U */
    U: 85,
    /* Variable: V */
    V: 86,
    /* Variable: W */
    W: 87,
    /* Variable: X */
    X: 88,
    /* Variable: Y */
    Y: 89,
    /* Variable: Z */
    Z: 90,
    /* Variable: WIN_LEFT */
    WIN_LEFT: 91,
    /* Variable: WIN_RIGHT */
    WIN_RIGHT: 92,
    /* Variable: SELECT */
    SELECT: 93,
    /* Variable: NUMPAD_0 */
    NUMPAD_0: 96,
    /* Variable: NUMPAD_1 */
    NUMPAD_1: 97,
    /* Variable: NUMPAD_2 */
    NUMPAD_2: 98,
    /* Variable: NUMPAD_3 */
    NUMPAD_3: 99,
    /* Variable: NUMPAD_4 */
    NUMPAD_4: 100,
    /* Variable: NUMPAD_5 */
    NUMPAD_5: 101,
    /* Variable: NUMPAD_6 */
    NUMPAD_6: 102,
    /* Variable: NUMPAD_7 */
    NUMPAD_7: 103,
    /* Variable: NUMPAD_8 */
    NUMPAD_8: 104,
    /* Variable: NUMPAD_9 */
    NUMPAD_9: 105,
    /* Variable: NUMPAD_MULTIPLY */
    NUMPAD_MULTIPLY: 106,
    /* Variable: NUMPAD_ADD */
    NUMPAD_ADD: 107,
    /* Variable: NUMPAD_SUBTRACT */
    NUMPAD_SUBTRACT: 109,
    /* Variable: NUMPAD_DECIMALPOINT */
    NUMPAD_DECIMALPOINT: 110,
    /* Variable: NUMPAD_DIVIDE */
    NUMPAD_DIVIDE: 111,
    /* Variable: F1 */
    F1: 112,
    /* Variable: F2 */
    F2: 113,
    /* Variable: F3 */
    F3: 114,
    /* Variable: F4 */
    F4: 115,
    /* Variable: F5 */
    F5: 116,
    /* Variable: F6 */
    F6: 117,
    /* Variable: F7 */
    F7: 118,
    /* Variable: F8 */
    F8: 119,
    /* Variable: F9 */
    F9: 120,
    /* Variable: F10 */
    F10: 121,
    /* Variable: F11 */
    F11: 122,
    /* Variable: F12 */
    F12: 123,
    /* Variable: NUMLOCK */
    NUMLOCK: 144,
    /* Variable: SCROLLLOCK */
    SCROLLLOCK: 145,
    /* Variable: SEMICOLON */
    SEMICOLON: 186,
    /* Variable: EQUAL */
    EQUAL: 187,
    /* Variable: COMMA */
    COMMA: 188,
    /* Variable: DASH */
    DASH: 189,
    /* Variable: PERIOD */
    PERIOD: 190,
    /* Variable: SLASH */
    SLASH: 191,
    /* Variable: BRACKET_OPEN */
    BRACKET_OPEN: 219,
    /* Variable: BACKSLASH */
    BACKSLASH: 220,
    /* Variable: BRACKET_CLOSE */
    BRACKET_CLOSE: 221,
    /* Variable: SINGLE_QUOTE */
    SINGLE_QUOTE: 222,

    _pressedMap: [],

    /*
     * Function: isPressed
     *
     * Description:
     *
     * Check if a key is pressed
     *
     * Parameters:
     * 
     * k - key to check
     *
     * Returns:
     *
     * true or false
     *
     * Example:
     *
     * > if (gamvas.key.isPressed(gamvas.key.Q)) {
     * >    this.quit = true;
     * > }
     */
    isPressed: function(k) {
        if (!gamvas.isSet(gamvas.key._pressedMap[k])) {
            return false;
        }
        return gamvas.key._pressedMap[k];
    },

    setPressed: function(k, v) {
        gamvas.key._pressedMap[k] = v;
    },

    /*
     * Function: exitEvent
     *
     * Description:
     *
     * return from a unhandled keypress on onKey* functions
     *
     * See:
     *
     * <gamvas.State.onKeyDown>
     * <gamvas.State.onKeyUp>
     *
     */
    exitEvent: function() {
	    if (gamvas.config.preventKeyEvents) {
		    return false;
	    }
	    return true;
    }
};
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.mouse
 *
 * Description:
 *
 * Information and functions for the mouse.
 *
 * PLEASE NOTE:
 *
 * There are browsers that are not able to handle
 * middle and right mouse buttons without problems.
 *
 * Middle and rigth mouse button is successfully tested on current versions of:
 *
 * * Chrome
 * * Firefox
 * * xxxterm
 */
gamvas.mouse = {
    /*
     * Variable: LEFT
     *
     * Description:
     *
     * Defines the left mouse button
     */
    LEFT: 0,

    /*
     * Variable: MIDDLE
     *
     * Description:
     *
     * Defines the middle mouse button
     */
    MIDDLE: 1,

    /*
     * Variable: RIGHT
     *
     * Description:
     *
     * Defines the right mouse button
     */
    RIGHT: 2,

    _pressedMap: [],
    _mouseX: 0,
    _mouseY: 0,

    /*
     * Function: isPressed
     *
     * Description:
     *
     * Check if a mouse button is pressed
     *
     * Returns:
     *
     * true/false
     *
     * Example:
     *
     * > if (gamvas.mouse.isPressed(gamvas.mouse.LEFT)) {
     * >     alert('you have found your left mouse button');
     * > }
     */
    isPressed: function(k) {
        if (!gamvas.isSet(gamvas.key._pressedMap[k])) {
            return false;
        }
        return gamvas.key._pressedMap[k];
    },

    setPosition: function(x, y) {
        gamvas.mouse._mouseX = x;
        gamvas.mouse._mouseY = y;
    },

    /*
     * Function: getX
     *
     * Description:
     *
     * Returns the x position of the mouse over the canvas.
     */
    getX: function(ev) {
        if (ev) {
            gamvas.mouse.setPosition(ev.pageX, ev.pageY);
        }
        var cp = gamvas.getCanvasPosition();
        return gamvas.mouse._mouseX-cp.x;
    },

    /*
     * Function: getY
     *
     * Description:
     *
     * Returns the y position of the mouse over the canvas.
     */
    getY: function(ev) {
        if (ev) {
            gamvas.mouse.setPosition(ev.pageX, ev.pageY);
        }
        var cp = gamvas.getCanvasPosition();
        return gamvas.mouse._mouseY-cp.y;
    },

    /*
     * Function: getPosition
     *
     * Description:
     *
     * Returns the position of the mouse as gamvas.Vector2D
     *
     * Note:
     *
     * The position is relative to the canvas, so 0/0 is the upper left
     * corner and canvaswidth/canvasheight is the lower right corner.
     *
     * To convert the canvas mouse position the the current states world
     * use <gamvas.camera.toWorld> from this.camera of the current state
     *
     * Returns:
     *
     * <gamvas.Vector2D>
     */
    getPosition: function(ev) {
        if (ev) {
            gamvas.mouse.setPosition(ev.pageX, ev.pageY);
        }

        var cp = gamvas.getCanvasPosition();
        return new gamvas.Vector2D(gamvas.mouse._mouseX-cp.x, gamvas.mouse._mouseY-cp.y);
    },

    setPressed: function(k, v) {
        gamvas.key._pressedMap[k] = v;
    },

    /*
     * Function: exitEvent
     *
     * Description:
     *
     * return from a unhandled mouseevent on onMouse* functions
     *
     * See:
     *
     * <gamvas.State.onMouseDown>
     * <gamvas.State.onMouseUp>
     * <gamvas.State.onMouseMove>
     *
     */
    exitEvent: function() {
	    if (gamvas.config.preventMouseEvents) {
		    return false;
	    }
	    return true;
    }
};
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.State
 *
 * Description:
 *
 * Game state class, overwrite for own states
 *
 * Constructor:
 *
 * new gamvas.State(name);
 *
 * Parameters:
 *
 * name - a unique identifier as name
 *
 * Example:
 *
 * A hello world state
 *
 * > myState = gamvas.State.extend({
 * >     draw: function(t) {
 * >         this.c.fillText("Hello World!", 10, 10);
 * >     }
 * > });
 */
gamvas.State = gamvas.Class.extend({
        create: function(name) {
            this._isInitialized = false;

            /*
             * Variable: disableCamera
             *
             * Description:
             *
             * By default a state provides a full environment for game development.
             * This includes a default camera under this.camera.
             *
             * If you do not want this behaviour, because you need custom camera
             * code, setting disableCamera to true will disable the default camera.
             *
             * Default:
             *
             * false
             */
            this.disableCamera = false;

            /*
             * Variable: name
             *
             * Description:
             *
             * The name of the state, must be unique within all states
             */
            this.name = name;

            /*
             * Variable: autoClear
             *
             * Description:
             *
             * If set to true, the screen will automatically cleared before drawing
             * functions are called.
             *
             * Default:
             *
             * true
             *
             * See:
             *
             * <gamvas.State.clearColor>
             */
            this.autoClear = true;

            /*
             * Variable: clearColor
             *
             * Description:
             *
             * The color to clear the screen, if <gamvas.State.autoClear> is enabled.
             * It is a string in css color notation.
             *
             * Default:
             *
             * '#000000'
             */
            this.clearColor = '#000000';
            this.actors = [];
            this.eventActors = [];
            this._afiles = [];

            /*
             * Variable: resource
             *
             * a <gamvas.Resource> instance, to allow easy file loading
             *
             * Example:
             *
             * > var image = this.resource.getImage('myImage.png');
             */
            this.resource = null;

            /*
             * Variable: canvas
             *
             * The DOM element of the canvas the game is running on.
             */
            this.canvas = null;

            /*
             * Variable: c
             *
             * a HTML5 2D context of the canvas the game is running on.
             * Use this to have low level access to the canvas 2D context.
             *
             * Example:
             *
             * > myState = gamvas.State.extend({
             * >     draw: function(t) {
             * >         this.c.fillText("Score: "+this.score, 10, 10);
             * >     }
             * > });
             */
            this.c = null;

            /*
             * Variable: dimension
             *
             * a object holding the canvas dimension in {w, h}
             *
             * Example:
             *
             * > console.log("Canvas width/height: "+this.dimension.w+"/"+this.dimension.h);
             */
            this.dimension = null;

            /*
             * Variable: camera
             *
             * a <gamvas.Camera> instance, to modify camera.
             *
             * Example:
             *
             * > myState = gamvas.State.extend({
             * >     draw: function(t) {
             * >         this.actor.draw(t);
             * >         // rotate 90 degrees per second (in radians 90 degrees is 0.5*PI)
             * >         this.camera.rotate(0.5*Math.PI*t);
             * >     }
             * > });
             */
            this.camera = null;
        },
        _setup: function() {
            this.resource = new gamvas.Resource();
            this.canvas = gamvas.getCanvas();
            this.c = gamvas.getContext2D();
            this.dimension = gamvas.getCanvasDimension();
            this.camera = new gamvas.Camera();
        },

        /*
         * Function: init
         *
         * Description:
         *
         * The init function is called only once per state to initialize it.
         *
         * Overwrite this function to do things like loading images and sounds
         * that are necessary for this state
         */
        init: function() {
        },

        /*
         * Function: enter
         *
         * Description:
         *
         * The enter function is called when the state is entered.
         *
         * Overwrite this function with code that should run when
         * the state is entered, which can be at the beginning of the
         * game or when the system switches between two states.
         *
         * See:
         *
         * <gamvas.State.leave>
         */
        enter: function() {
        },

        /*
         * Function: leave
         *
         * Description:
         *
         * The enter function is called when the state is left.
         *
         * Overwrite this function with code that should run when
         * the state is left and a other state will be called
         * after this.
         *
         * See:
         *
         * <gamvas.State.enter>
         */
        leave: function() {
        },

        /*
         * Function: clearScreen
         *
         * Description:
         *
         * Clears the screen.
         * This is usually called automatically
         * unless you deactivate automatic screen clearing with <gamvas.State.autoClear>
         */
        clearScreen: function() {
            this.c.fillStyle = this.clearColor;
            this.c.fillRect(0, 0, this.dimension.w, this.dimension.h);
        },

        /*
         * Function: preDraw
         *
         * Description:
         *
         * This function is called before camera is applied.
         * Overwrite it to
         * draw things after screen clear and before zoom/rotation/move of camera
         * is applied.
         *
         * Parameters:
         *
         * t - the time elapsed since the last frame
         *
         */
        preDraw: function(t) {
        },

        /*
         * Function: draw
         *
         * Description:
         *
         * Overwrite this function to draw your game objects.
         * All objects drawn in the draw function are drawn under camera
         * influence, means the are drawn moved, scaled and rotated on the
         * screen, depending on how the settings of this.camera are.
         *
         * Parameters:
         *
         * t - the time elapsed since the last frame
         *
         * Example:
         *
         * Draw a actor moving 20 pixels per second along the x axis
         *
         * > myState = gamvas.State.extend({
         * >     draw: function(t) {
         * >         this.myActor.draw(t);
         * >         this.myActor.move(20*t, 0);
         * >     }
         * > });
         */
        draw: function(t) {
        },

        /*
         * Function: postDraw
         *
         * Description:
         *
         * This function is called after camera is applied.
         * Overwrite it to
         * draw things that are not under camera influence, like score or HUD elements.
         *
         * Parameters:
         *
         * t - the time elapsed since the last frame
         *
         * Example:
         *
         * > myState = gamvas.State.extend({
         * >     postDraw: function(t) {
         * >         this.c.fillText("Score: "+this.score, 10, 470);
         * >     }
         * > });
         */
        postDraw: function(t) {
        },

        /*
         * Function: loading
         *
         * Description:
         *
         * Overwrite for a custom loading screen
         *
         * Parameters:
         *
         * t - the time elapsed since the last frame
         *
         * See:
         *
         * <gamvas.Resource.status>
         *
         * Example:
         *
         * > myState = gamvas.State.extend({
         * >    loading: function(t) {
         * >       this.c.fillText("Loading: "+(100*this.resource.status())+"%", 10, 10);
         * >    }
         * > });
         */
        loading: function(t) {
            var d = gamvas.getCanvasDimension();
            var tp = (d.h/2)-5;
            var w = parseInt(d.w*0.7, 10);
            var off = parseInt((d.w-w)/2, 10);
            this.c.lineWidth = 2;
            this.c.fillStyle = '#ffffff';
            this.c.strokeStyle = '#ffffff';
            this.c.strokeRect(off+2, tp, w, 10);
            this.c.fillRect(off+4, tp+2, (w-2)*this.resource.status(), 6);
        },

        /*
         * Function: onKeyDown
         *
         * Description:
         *
         * A key on the keyboard was pressed
         *
         * Parameters:
         *
         * keyCode - The keycode of the key (e.g. gamvas.key.RETURN)
         * character - The actual character (e.g. 'a')
         * ev - The JavaScript event object
         *
         * Returns:
         *
         * true or false, depending if it handled the event or not
         *
         * See:
         *
         * <gamvas.key.exitEvent>
         * <gamvas.State.onKeyUp>
         *
         * Example:
         *
         * > myState = gamvas.State.extend({
         * >     onKeyDown: function(keyCode) {
         * >         if (keyCode == gamvas.key.SPACE) {
         * >             this.firePlayerGun();
         * >         }
	 * >         return gamvas.key.exitEvent();
         * >     }
         * > });
         */
        onKeyDown: function(keyCode, character, ev) {
		return gamvas.key.exitEvent();
        },

        /*
         * Function: onKeyUp
         *
         * Description:
         *
         * A key on the keyboard was released
         *
         * Parameters:
         *
         * keyCode - The keycode of the key (e.g. gamvas.key.RETURN)
         * character - The actual character (e.g. 'a')
         * ev - The JavaScript event object
         *
         * Returns:
         *
         * true or false, depending if it handled the event or not
         *
         * See:
         *
         * <gamvas.key.exitEvent>
         * <gamvas.State.onKeyDown>
         */
        onKeyUp: function(keyCode, character, ev) {
		return gamvas.key.exitEvent();
        },

        /*
         * Function: onMouseDown
         *
         * Description:
         *
         * A mouse button was pressed
         *
         * Parameters:
         *
         * button - The mouse button that was pressed (e.g. gamvas.mouse.LEFT)
         * x/y - The position on the screen the mousepointer was while pressed
         * ev - The JavaScript event object
         *
         * Returns:
         *
         * true or false, depending if it handled the event or not
         *
         * See:
         *
         * <gamvas.mouse.exitEvent>
         * <gamvas.mouse>
         */
        onMouseDown: function(button, x, y, ev) {
		return gamvas.mouse.exitEvent();
        },

        /*
         * Function: onMouseUp
         *
         * Description:
         *
         * A mouse button was released
         *
         * Parameters:
         *
         * button - The mouse button that was released (e.g. gamvas.mouse.LEFT)
         * x/y - The position on the screen the mousepointer was while released
         * ev - The JavaScript event object
         *
         * Returns:
         *
         * true or false, depending if it handled the event or not
         *
         * See:
         *
         * <gamvas.mouse.exitEvent>
         * <gamvas.mouse>
         */
        onMouseUp: function(button, x, y, ev) {
		return gamvas.mouse.exitEvent();
        },

        /*
         * Function: onMouseMove
         *
         * Description:
         *
         * The mouse was moved
         *
         * Parameters:
         *
         * x/y - The position where the mousecursor was
         * ev - The JavaScript event object
         *
         * Returns:
         *
         * true or false, depending if it handled the event or not
	 *
	 * See:
	 *
         * <gamvas.mouse.exitEvent>
         *
         * Example:
         *
         * > myState = gamvas.State.extend({
         * >     onMouseMove: function(x, y) {
         * >         this.mousePointerImage.setPosition(x, y);
	 * >         return gamvas.mouse.exitEvent();
         * >     }
         * > });
         */
        onMouseMove: function(x, y, ev) {
		return gamvas.mouse.exitEvent();
        },

        /*
         * Function: addActor
         *
         * Description:
         *
         * Add a <gamvas.Actor> to the state.
         * Actors added to a state are drawn automatically.
         *
         * Parameters:
         *
         * act - a <gamvas.Actor> object
         *
         * Example:
         *
         * > myState = gamvas.State.extend({
         * >     init: function() {
         * >         this.myActor = new gamvas.Actor('myactor', 10, 20);
         * >         this.addActor(this.myActor);
         * >     },
         * >     draw: function(t) {
         * >         // i dont need to draw this.myActor, it draws automatically
         * >     }
         * > });
         */
        addActor: function(act) {
            this.actors[act.name] = act;
        },

        /*
         * Function: registerInputEvents
         *
         * Description:
         *
         * Send keyboard/mouse events to <gamvas.Actor> onKeyDown and similar functions
         *
         * Parameters:
         *
         * act - a <gamvas.Actor> object
         *
         * Example:
         *
         * > // default a actor state for the logic
         * > myActorState = gamvas.ActorState.extend({
         * >     onKeyDown: function(keyCode) {
         * >         if (keyCode = gamvas.key.SPACE) {
         * >             this.fireGun()
         * >         }
         * >     },
         * >     fireGun: function() {
         * >         console.log(this.actor.name + ' shoots');
         * >     }
         * > });
         * >
         * > // define a actor
         * > myActor = gamvas.Actor.extend({
         * >     create: function(name, x, y) {
         * >         this._super(name, x, y);
         * >         this.addState(new myActorState('standard'));
         * >         this.setState('standard');
         * >     },
         * > });
         * >
         * > // now in our game state, create the actor and
         * > // register it for receiving input events
         * > myState = gamvas.State.extend({
         * >     init: function() {
         * >         this.player = new myActor('player');
         * >         this.registerInputEvents(this.player);
         * >     }
         * > });
         */
        registerInputEvents: function(act) {
            this.eventActors[act.name] = act;
        },

        /*
         * Function: addSound
         *
         * Description:
         *
         * Add a <gamvas.Sound> to the state.
         *
         * Parameters:
         *
         * snd - either a url to the sound file, or a <gamvas.Sound> object
         *
         * Returns:
         *
         * A <gamvas.Sound> object
         *
         * Example:
         *
         * > myState = gamvas.State.extend({
         * >     init: function() {
         * >         this.shotSound = this.addSound("shotsound.wav");
         * >     },
         * >     myState.onKeyDown = function(keyCode, character) {
         * >         if (keyCode == gamvas.key.SPACE) {
         * >             this.shotSound.play();
         * >         }
         * >     }
         * > });
         */
        addSound: function(snd) {
            var s = null;
            if (typeof snd == 'string') {
                s=new gamvas.Sound(this.resource.getSound(snd));
            } else {
                s=snd;
            }
            this._afiles[s.url] = s;
            return s;
        },

        update: function(t) {
            var ts = gamvas.timer.getGlobalTimeScale();
            for (var i in this._afiles) {
                this._afiles[i].setRate(ts);
            }
        },

        /*
         * Function: getActors
         *
         * Description:
         *
         * returns all the actors added with <gamvas.State.addActor> as array, sorted by their <gamvas.Actor.layer>
         *
         * Note:
         *
         * sorting is expensive, try to avoid this function by processing
         * <gamvas.State.actors> directly or cache the result
         */
        getActors: function() {
            var sorted = [];
            for (var i in this.actors) {
                sorted.push(this.actors[i]);
            }
            sorted.sort(function(a,b) {return (b.layer - a.layer);});
            return sorted;
        }
});
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.state
 *
 * Helper functions for state management
 */
gamvas.state = {
    _states: [],
    _currentState: null,

    /*
     * Function: addState
     *
     * Description:
     *
     * Add a state to the state system
     *
     * Parameters:
     *
     * state - a <gamvas.State> object
     *
     * Example:
     *
     * > var myState = gamvas.State.extend({
     * >     init: {
     * >         console.log('state '+this.name+' initialized');
     * >     }
     * > });
     * > gamvas.state.addState(new myState('mystate'));
     */
    addState: function(state) {
        gamvas.state._states[state.name] = state;
        if (gamvas.state._currentState === null) {
            gamvas.state._currentState = state.name;
        }
    },

    /*
     * Function: setState
     *
     * Description:
     *
     * switch to a new state
     *
     * Parameters:
     *
     * stateName - the name of the state to switch to
     */
    setState: function(stateName) {
        if (gamvas.isSet(gamvas.state._states[stateName])) {
            if (gamvas.state._currentState !== null) {
                var cur = gamvas.state.getCurrentState();
                if (cur) {
                    cur.leave();
                }
            }

            var st = gamvas.state._states[stateName];
            gamvas.state._currentState = stateName;
            if (!st._isInitialized) {
                st.init();
                st._isInitialized = true;
            }
            st.enter();
        }
    },

    /*
     * Function: getCurrentState
     *
     * Description:
     *
     * Get the current state
     *
     * Example:
     *
     * > var cs = gamvas.getCurrentState();
     * > console.log("current state is: " + cs.name);
     */
    getCurrentState: function() {
        if (gamvas.state._currentState !== null) {
            var cur = gamvas.state._states[gamvas.state._currentState];
            return cur;
        }
        return false;
    },

    update: function(t) {
        var cur = gamvas.state.getCurrentState();
        if (cur) {
            cur.resource.cache = false;
            var ready = cur.resource.done();
            var world = null;
            if ( (gamvas._usePhys) && (ready) ) {
                world = gamvas.physics.getWorld();
                world.Step(t, gamvas.physics.velocityIterations, gamvas.physics.positionIterations);
                var allBd = world.GetBodyList();
                var i = null;
                var b = null;
                var a = null;
                for (i = world.m_bodyList; i; i = i.m_next) {
                    a = i.GetUserData();
                    if (a !== null) {
                        if (a.type == 'actor') {
                            a.data.updatePhysics();
                        }
                    }
                }
            }

            var allActors = cur.getActors();
            if (ready) {
                cur.update(t);
            }

            if (cur.autoClear) {
                cur.clearScreen();
            }
            if (ready) {
                for (var n = 0; n < allActors.length; n++) {
                    if (allActors[n].isActive()) {
                        allActors[n].preDraw(t);
                    }
                }
                cur.preDraw(t);
            }
            var useCam = !cur.disableCamera;
            if (useCam) cur.camera.start();
            if (ready) {
                for (var n2 = 0; n2 < allActors.length; n2++) {
                    if (allActors[n2].isActive()) {
                        allActors[n2].draw(t);
                    }
                }
                cur.draw(t);
            } else {
                cur.loading(t);
            }
            if (useCam) cur.camera.stop();
            if (ready) {
                for (var n3 = 0; n3 < allActors.length; n3++) {
                    if (allActors[n3].isActive()) {
                        allActors[n3].postDraw(t);
                    }
                }
                cur.postDraw(t);
            }
            if ( (gamvas._usePhys) && (world) ) {
                world.ClearForces();
            }
        }
    },

    onKeyDown: function(ev) {
        gamvas.key.setPressed(ev.keyCode, true);
        var cur = gamvas.state.getCurrentState();
        if (cur) {
            for (var i in cur.eventActors) {
                cur.eventActors[i].onKeyDown(ev.keyCode, ev.charCode, ev);
            }
	    console.log('return on state');
            return cur.onKeyDown(ev.keyCode, ev.charCode, ev);
        }
	return gamvas.key.exitEvent();
    },

    onKeyUp: function(ev) {
        gamvas.key.setPressed(ev.keyCode, false);
        var cur = gamvas.state.getCurrentState();
        if (cur) {
            for (var i in cur.eventActors) {
                cur.eventActors[i].onKeyUp(ev.keyCode, ev.charCode, ev);
            }
            return cur.onKeyUp(ev.keyCode, ev.charCode, ev);
        }
	return gamvas.key.exitEvent();
    },

    onMouseDown: function(ev) {
        if (ev.preventDefault) {
            ev.preventDefault();
        }
        gamvas.key.setPressed(ev.button, true);
        var cur = gamvas.state.getCurrentState();
        if (cur) {
            var pos = gamvas.mouse.getPosition();
            for (var i in cur.eventActors) {
                cur.eventActors[i].onMouseDown(ev.button, pos.x, pos.y, ev);
            }
            var res = cur.onMouseDown(ev.button, pos.x, pos.y, ev);
            ev.returnValue = res;
            return res;
        }
	var ret = gamvas.mouse.exitEvent();
	ev.returnValue=ret;
	return ret;
    },

    onMouseUp: function(ev) {
        if (ev.preventDefault) {
            ev.preventDefault();
        }
        gamvas.key.setPressed(ev.button, false);
        var cur = gamvas.state.getCurrentState();
        if (cur) {
            var pos = gamvas.mouse.getPosition();
            for (var i in cur.eventActors) {
                cur.eventActors[i].onMouseUp(ev.button, pos.x, pos.y, ev);
            }
            var res = cur.onMouseUp(ev.button, pos.x, pos.y, ev);
            ev.returnValue= res;
            return res;
        }
	var ret = gamvas.mouse.exitEvent();
	ev.returnValue=ret;
	return ret;
    },

    onMouseMove: function(ev) {
        if (ev.preventDefault) {
            ev.preventDefault();
        }
        gamvas.mouse.setPosition(ev.pageX, ev.pageY);
        var cur = gamvas.state.getCurrentState();
        if (cur) {
            var pos = gamvas.mouse.getPosition();
            for (var i in cur.eventActors) {
                cur.eventActors[i].onMouseMove(pos.x, pos.y, ev);
            }
            var res = cur.onMouseMove(pos.x, pos.y, ev);
            ev.returnValue=res;
            return res;
        }
	var ret = gamvas.mouse.exitEvent();
	ev.returnValue=ret;
	return ret;
    },

    setup: function() {
        if (gamvas.state._currentState !== null) {
            var n = 0;
            for (var i in gamvas.state._states) {
                gamvas.state._states[i]._setup();
                if (n === 0) {
                    gamvas.state.setState(i);
                }
                n++;
            }
        }
    }
};

document.onkeydown = gamvas.state.onKeyDown;
document.onkeyup = gamvas.state.onKeyUp;
document.onmousedown = gamvas.state.onMouseDown;
document.onmouseup = gamvas.state.onMouseUp;
document.onmousemove = gamvas.state.onMouseMove;
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.Camera
 *
 * Description:
 *
 * The camera class.
 *
 * Note:
 *
 * Every <gamvas.State> has a default camera, which you can access with
 * this.camera
 *
 * Constructor:
 *
 * new gamvas.Camera([x], [y], [rot], [zoom]);
 *
 * Parameters:
 *
 * x/y - The position of the cameras center (optional)
 * rot - The rotation of the camera in radians (optional)
 * zoom - The zoom factor of the camera (optional)
 */
gamvas.Camera = function(x, y, rot, zoom) {
    /*
     * Variable: position
     *
     * Description:
     *
     * A <gamvas.Vector2D> object that holds the position of the camera. It is recommended to use it only for reading.
     */
    this.position = new gamvas.Vector2D( (x) ? x : 0, (y) ? y : 0);

    /*
     * Variable: rotation
     *
     * Description:
     *
     * The rotation of the camera in radians. It is recommended to use it only for reading.
     */
    this.rotation = (rot) ? rot : 0;

    /*
     * Variable: zoomFactor
     *
     * Description:
     *
     * The zoom factor of the camera. It is recommended to use it only for reading.
     */
    this.zoomFactor = (zoom) ? zoom : 1;
};

/*
 * Function: setPosition
 *
 * Description:
 *
 * Set the position of the camera to absolute values.
 *
 * Parameters:
 *
 * x - The new x position of the cameras center
 * y - The new y position of the cameras center
 */
gamvas.Camera.prototype.setPosition = function(x, y) {
    this.position.x = x;
    this.position.y = y;
};

/*
 * Function: move
 *
 * Description:
 *
 * Move the camera, relative to its current position.
 *
 * Parameters:
 *
 * x - The amount of pixels to move along the horizontal axis
 * y - The amount of pixels to move along the vertical axis
 */
gamvas.Camera.prototype.move = function(x, y) {
    this.position.x += x;
    this.position.y += y;
};

/*
 * Function: setRotation
 *
 * Description:
 *
 * Set the rotation to a specific degree in radians
 *
 * Parameters:
 *
 * r - rotation in radians
 */
gamvas.Camera.prototype.setRotation = function(r) {
    this.rotation = r;
};

/*
 * Function: rotate
 *
 * Description:
 *
 * Rotate the camera for r radians
 *
 * Parameters:
 *
 * r - rotation in radians
 */
gamvas.Camera.prototype.rotate = function(r) {
    this.rotation += r;
};

/*
 * Function: setZoom
 *
 * Description:
 *
 * Set a specific zoom level to the camera.
 *
 * Parameters:
 *
 * z - the zoom level, as 1 = original size, < 1 = zoom out, > 1 = zoom in
 */
gamvas.Camera.prototype.setZoom = function(z) {
    this.zoomFactor = z;
};

/*
 * Function: zoom
 *
 * Description:
 *
 * Zoom the camera
 *
 * Parameters:
 *
 * z - the change of zoom, relative to its current zoom
 */
gamvas.Camera.prototype.zoom = function(z) {
    this.zoomFactor += z;
};

/*
 * Function: start
 *
 * Description:
 *
 * Start the camera. Needs to be called before every drawing operation that should
 * be in the influence of the camera (e.g. your game actors)
 *
 * See:
 *
 * <gamvas.Camera.stop>
 *
 * Example:
 *
 * > this.camera.start();
 * > // draw everything under camera influence
 * > this.drawGameObjects();
 * > this.camera.stop();
 * > // draw everything not under camera influence (e.g. Score, Health, ...)
 * > this.drawHUD();
 */
gamvas.Camera.prototype.start = function() {
    var d = gamvas.getCanvasDimension();
    var c = gamvas.getContext2D();
    var centerX = d.w/(this.zoomFactor*2);
    var centerY = d.h/(this.zoomFactor*2);
    c.save();
    c.scale(this.zoomFactor, this.zoomFactor);
    c.translate(centerX, centerY);
    c.rotate(this.rotation);
    c.translate(-this.position.x, -this.position.y);
};

/*
 * Function: stop
 *
 * Description:
 *
 * Stop the camera, everything drawn from now on will not be under
 * the influence of the camera. You usually do this before drawing
 * player score and health.
 *
 * See:
 *
 * <gamvas.Camera.start>
 */
gamvas.Camera.prototype.stop = function() {
    var c = gamvas.getContext2D();
    c.restore();
};

/*
 * Function: toWorld
 *
 * Description:
 *
 * Convert screen coordinates to world coordinates. For example if you have a
 * rotated camera and the user clicks on the screen, use this function
 * to check which position the click on the rotated camera is in the unrotated
 * world.
 *
 * Returns:
 *
 * <gamvas.Vector2D>
 *
 * See:
 *
 * <gamvas.Camera.toScreen>
 */
gamvas.Camera.prototype.toWorld = function(x, y) {
    var d = gamvas.getCanvasDimension();
    var ret = new gamvas.Vector2D((x-d.w/2)/this.zoomFactor, (y-d.h/2)/this.zoomFactor).rotate(-this.rotation);
    ret.x+=this.position.x;
    ret.y+=this.position.y;
    return ret;
};

/*
 * Function: toScreen
 *
 * Description:
 *
 * Convert world coordinates to screen coordinates. If you have
 * a object in your world and your camera has moved 3 screen aside,
 * this function gets you the actual screen position of the object.
 *
 * Returns:
 *
 * <gamvas.Vector2D>
 * 
 * See:
 *
 * <gamvas.Camera.toWorld>
 */
gamvas.Camera.prototype.toScreen = function(x, y) {
    var d = gamvas.getCanvasDimension();
    var ret = new gamvas.Vector2D(x-this.x, y-this.y).rotate(this.rotation);
    ret.x = ret.x*this.zoomFactor+d.w/2;
    ret.y = ret.y*this.zoomFactor+d.h/2;
    return ret;
};
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.screen
 *
 * Functions with info about the screen
 */
gamvas.screen = {
    /*
     * Variable: frameLimit
     *
     * Description:
     *
     * Sets the maximum of rendertime in seconds until a frame is skipped. To prevent hickups
     * on performance bottlenecks. So if you want to ensure a minimur of 30 frames per second
     * you would set it to 1/30 = 0.033, but it is not recommended to set this value to
     * low or your game will not work on slower systems.
     *
     * Default:
     *
     * 0.1
     */
    frameLimit: 0.1,
    _pause: false,
    _redrawHandlers: [],
    _lastFrameTime: -1,
    _lastFrameLength: 0,
    _lastFPSCheck: 0,
    _lastFPS: 0,
    _fpsCounter: 0,

    redraw: function() {
        window.requestAnimationFrame(gamvas.screen.redraw);
        if (gamvas.screen._pause) {
            return;
        }
        var ms = gamvas.timer.getMilliseconds();
        if (gamvas.screen._lastFPSCheck < ms) {
            gamvas.screen._lastFPS = gamvas.screen._fpsCounter+1;
            gamvas.screen._fpsCounter = 0;
            gamvas.screen._lastFPSCheck = ms+1000;
        } else {
            gamvas.screen._fpsCounter++;
        }

        var frameSecs = 0;
        if (gamvas.screen._lastFrameTime>-1) {
            var lt = gamvas.screen._lastFrameTime;
            gamvas.screen._lastFrameTime = ms/1000;
            gamvas.screen._lastFrameLength = gamvas.screen._lastFrameTime-lt;
            if (gamvas.screen._lastFrameLength > gamvas.screen.frameLimit) {
                gamvas.screen._lastFrameLength = gamvas.screen.frameLimit;
            }
        } else {
            gamvas.screen._lastFrameTime = 0;
        }
        // draw states
        gamvas.state.update(gamvas.screen._lastFrameLength);

        // draw handlers, if available
        for (var i in gamvas.screen._redrawHandlers) {
            (gamvas.screen._redrawHandlers[i])(gamvas.screen._lastFrameLength);
        }
    },

    /*
     * Function: getLastFrameLength
     *
     * Description:
     *
     * Returns the time elapsed since the last call to render a frame.
     */
    getLastFrameLength: function() {
        return gamvas.screen._lastFrameLength;
    },

    /*
     * Function: getFPS
     *
     * Description:
     *
     * Returns the current frames per second. Browsers try to run
     * your game at 60 FPS. If your game runs significantly slower
     * you should try to enhance the performance.
     *
     * Example:
     *
     * > console.log(gamvas.screen.getFPS());
     */
    getFPS: function() {
        return gamvas.screen._lastFPS;
    },

    setRedrawHandler: function(fnc) {
        gamvas.screen._redrawHandlers['_default'] = fnc;
    },

    unsetRedrawHandler: function() {
        delete gamvas.screen._redrawHandlers['_default'];
    },

    addRedrawHandler: function(id, fnc) {
        gamvas.screen._redrawHandlers[id] = fnc;
    },

    removeRedrawHandler: function(id) {
        delete gamvas.screen._redrawHandlers[id];
    },

    pause: function() {
        gamvas.screen._pause = true;
    },

    resume: function() {
        var ms = gamvas.timer.getMilliseconds();
        gamvas.screen._lastFrameTime = (ms/1000)-gamvas.screen.frameLimit;
        gamvas.screen._lastFrameLength = gamvas.screen.frameLimit;
        gamvas.screen._pause = false;
    }
};

window.onblur = gamvas.screen.pause;
window.onfocus = gamvas.screen.resume;
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.Image
 *
 * Description:
 *
 * A plain image with methods to move, rotate and zoom.
 *
 * Use this if you need performance and are sure that the image
 * is never needed as logic or physics element, otherwise
 * use <gamvas.Actor>
 *
 * Constructur:
 *
 * new gamvas.Image(file, x, y, cx, cy);
 *
 * Parameters:
 *
 * file - a JavaScript Image object (see <gamvas.Resource>)
 * x/y - the position of the image (optional)
 * cx/cy - the center of rotation of the image (optional)
 *
 * See:
 * <gamvas.Actor>
 */
gamvas.Image = function(file, x, y, cx, cy, ctx) {
    this.image = file;
    this.position = new gamvas.Vector2D((x)?x:0, (y)?y:0);
    this.center = new gamvas.Vector2D((cx)?cx:0, (cy)?cy:0);
    this.rotation = 0;
    this.scaleFactor = 1;
    this.scaleFactor2 = 1;
    this.cr = null;
    this.c = gamvas.getContext2D();

    /*
     * Function: setRotation
     *
     * Description:
     *
     * Set certain rotation of the image in radians
     *
     * Parameters:
     *
     * r - the rotation in radians
     *
     * See:
     *
     * <gamvas.Image.rotate>
     * http://en.wikipedia.org/wiki/Radians
     */
    this.setRotation = function(r) {
        this.rotation = r;
    };

    /*
     * Function: rotate
     *
     * Description:
     *
     * Rotate the image
     *
     * Parameters:
     *
     * r - the amount to rotate the image in radians
     *
     * See:
     *
     * <gamvas.Image.setRotation>
     * http://en.wikipedia.org/wiki/Radians
     */
    this.rotate = function(r) {
        this.rotation += r;
    };

    /*
     * Function: setPosition
     *
     * Description:
     *
     * Set the position of a image
     *
     * Parameters:
     *
     * x/y - the position of the image in pixels
     *
     * See:
     *
     * <gamvas.Image.move>
     */
    this.setPosition = function(x, y) {
        this.position.x = x;
        this.position.y = y;
    };

    /*
     * Function: move
     *
     * Description:
     *
     * Move the image
     *
     * Parameters:
     *
     * x/y - the pixels to move the image
     *
     * See:
     *
     * <gamvas.Image.setPosition>
     */
    this.move = function(x, y) {
        this.position.x += x;
        this.position.y += y;
    };

    /*
     * Function: setScale
     *
     * Description:
     *
     * Set a certain scale factor
     *
     * Parameters:
     *
     * s - the scale value (1 = no scale, < 1 = smaller, > 1 = bigger)
     *
     * See:
     *
     * <gamvas.Image.scale>
     * <gamvas.Image.setScaleXY>
     */
    this.setScale = function(s) {
        this.scaleFactor = s;
        this.scaleFactor2 = s;
    };

    /*
     * Function: scale
     *
     * Description:
     *
     * Scale the image
     *
     * Parameters:
     *
     * s - the scale factor (< 0 = shrink, > 0 = enlarge)
     *
     * See:
     *
     * <gamvas.Image.setScale>
     */
    this.scale = function(s) {
        this.scaleFactor += s;
        this.scaleFactor2 += s;
    };

    /*
     * Function: setScaleXY
     *
     * Description:
     *
     * Set a different scale for x and y axis
     *
     * Parameters:
     *
     * x - the scale of the x axis (1 = no scale, < 1 = smaller, > 1 = bigger)
     * y - the scale of the y axis (1 = no scale, < 1 = smaller, > 1 = bigger)
     *
     * See:
     *
     * <gamvas.Image.setScale>
     */
    this.setScaleXY = function(x, y) {
        this.scaleFactor = x;
        this.scaleFactor2 = y;
    };

    /*
     * Function: setCenter
     *
     * Description:
     *
     * Set the center for an Image. If you have a round object
     * for example with a size of 64 by 64 pixels and you want
     * to rotate it around the center, you would use
     * myObject.setCenter(32, 32);
     *
     * Parameters:
     *
     * x/y - the center, as seen of the upper left corner of the object
     */
    this.setCenter = function(x, y) {
        this.center.x = x;
        this.center.y = y;
    };

    /*
     * Function: setFile
     *
     * Description:
     *
     * Sets the image
     *
     * Parameters:
     * image - a JavaScript Image object
     */
    this.setFile = function(f) {
        this.image = f;
    };

    /*
     * Function: draw
     *
     * Description:
     *
     * draws the image, using its position, rotation and scale information
     */
    this.draw = function() {
        var r = this.getClipRect();
        if ( (r.x >= 0) && (r.x < this.image.width) && (r.y >= 0) && (r.y < this.image.height) && (r.x+r.width <= this.image.width) && (r.y+r.height <= this.image.height) ) {
            this.c.save();
            this.c.translate(this.position.x, this.position.y);
            this.c.rotate(this.rotation);
            this.c.scale(this.scaleFactor, this.scaleFactor2);
            this.c.drawImage(this.image, r.x, r.y, r.width, r.height, -this.center.x+r.x, -this.center.y+r.y, r.width, r.height);
            this.c.restore();
        } else {
            console.log('not drawing because of clip rect: ');
            console.log(r);
        }
    };

    /*
     * Function: setClipRect
     *
     * Sets the clipping rectangle of a image
     *
     * A clipping rectangle defines which portion of the image will
     * be drawn. It has to be inside the image and is specified by
     * its top left corner and a width and height.
     *
     * By default, a clipping rectange of x/y = 0/0 and
     * width/height = image width and height is used
     *
     * Parameters:
     *
     * rx - either a <gamvas.Rect> object or the x coordinate of the upper left corner
     * y - the y coordinate of the uppder left corner (if rx is not a <gamvas.Rect> object)
     * w - the width of the clipping rectangle (if rx is not a <gamvas.Rect> object)
     * h - the height of the clipping rectangle (if rx is not a <gamvas.Rect> object)
     *
     * See:
     *
     * <gamvas.Image.getClipRect>
     */
    this.setClipRect = function(rx, y, w, h) {
        if (rx instanceof gamvas.Rect) {
            this.cr = rx;
        } else {
            this.cr = new gamvas.Rect(rx, y, w, h);
        }
    };

    /*
     * Function: getClipRect
     *
     * Gets the clipping rectangle of a image as <gamvas.Rect>
     *
     * See:
     *
     * <gamvas.Image.setClipRect>
     * <gamvas.Rect>
     */
    this.getClipRect = function() {
        if (this.cr === null) {
            this.cr = new gamvas.Rect(0, 0, this.image.width, this.image.height);
        }
        return this.cr;
    };
};
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.Animation
 *
 * Description:
 *
 * Class for animated sprites
 *
 * The image for the animation is a JavaScript Image object, that holds
 * all the frames with a certain width in one image.
 *
 * For example, if you have a animation with 10 frames sized 64x64, you
 * could make a image of 640x64 and put the 10 images side by side, or
 * you could make 2 rows with 5 images on each and make a 320x128 image.
 *
 * Constructor:
 *
 * new gamvas.Animation(name, [image], [frameWidth], [frameHeight], [numberOfFrames], [fps]);
 *
 * Parameters:
 *
 * name - A unique identifier as name
 * image - A Image object that holds the frames of the animation (optional)
 * framwWidth - The width of a single frame (optional)
 * frameHeight - The height of a single frame (optional)
 * numberOfFrames - The number of frames the animation has (optional)
 * fps - The speed of the animation in frames per second (optional)
 */
gamvas.Animation = gamvas.Class.extend({
    create: function(name, image, frameWidth, frameHeight, numberOfFrames, fps) {
        /*
         * Variable: name
         *
         * The name of the animation.
         * Must be unique within all animations of a actor
         */
        this.name = name;
        this.sprite = (image) ? image : null;

        /*
         * Variable: width
         *
         * The width of a single frame in pixels
         */
        this.width = (frameWidth) ? frameWidth : 0;

        /*
         * Variable: height
         *
         * The height of a single frame in pixels
         */
        this.height = (frameHeight) ? frameHeight : 0;

        /*
         * Variable: numberOfFrames
         *
         * The the frame count of the animation
         */
        this.numberOfFrames = (numberOfFrames) ? numberOfFrames : 0;

        /*
         * Variable: currentFrame
         *
         * The index of the current frame
         */
        this.currentFrame = 0;
        this.currentFrameTime = 0;
        this.fDur = (fps) ? 1/fps : 0.1;
        this.c = gamvas.getContext2D();
        this.frameList = [];

        /*
         * Variable: position
         *
         * The current position as <gamvas.Vector2D>
         */
        this.position = new gamvas.Vector2D();

        /*
         * Variable: center
         *
         * The center of rotation of the animation
         */
        this.center = new gamvas.Vector2D();

        /*
         * Variable: rotation
         *
         * The rotation in radians
         */
        this.rotation = 0;

        /*
         * Variable: scaleFactor
         *
         * The scale factor (1 = not scaled, < 1 = smaller, > 1 = bigger)
         */
        this.scaleFactor = 1;
        this.scaleFactor2 = 1;
    },

    /*
     * Function: setFile
     *
     * Description:
     *
     * Sets a image as source of a animation
     *
     * Parameters:
     * image - a JavaScript Image object holding the frames
     * frameWidth - the with of a single frame
     * frameHeight - the height of a single frame
     * numberOfFrames - the number of frames of the animation
     * fps - the speed of the animation in fps
     */
    setFile: function(image, frameWidth, frameHeight, numberOfFrames, fps) {
        this.sprite = image;
        this.width = (frameWidth) ? frameWidth : this.sprite.width;
        this.height = (frameHeight) ? frameHeight : this.sprite.height;
        this.numberOfFrames = (numberOfFrames) ? numberOfFrames : 1;
        this.currentFrame = 0;
        this.currentFrameTime = 0;
        this.setFPS((fps) ? fps : 1);
        this.needInit = false;
        if ( (typeof this.width == 'undefined') || (this.width === 0) ) {
            this.needInit = true;
        }
    },

    /*
     * Function: setFPS
     *
     * Description:
     *
     * Set the speed of the animation in frames per second
     *
     * Parameters:
     *
     * fps - The animation speed in frames per second
     */
    setFPS: function(fps) {
        this.fDur = 1/fps;
    },

    /*
     * Function: setRotation
     *
     * Description:
     *
     * Set certain rotation of the animation in radians
     *
     * Parameters:
     *
     * r - the rotation in radians
     *
     * See:
     *
     * <gamvas.Animation.rotate>
     * http://en.wikipedia.org/wiki/Radians
     */
    setRotation: function(r) {
        this.rotation = r;
    },

    /*
     * Function: rotate
     *
     * Description:
     *
     * Rotate the animation
     *
     * Parameters:
     *
     * r - the amount to rotate the animation in radians
     *
     * See:
     *
     * <gamvas.Animation.setRotation>
     * http://en.wikipedia.org/wiki/Radians
     */
    rotate: function(r) {
        this.rotation += r;
    },

    /*
     * Function: setPosition
     *
     * Description:
     *
     * Set the position of a animation
     *
     * Parameters:
     *
     * x/y - the position of the animation in pixels
     *
     * See:
     *
     * <gamvas.Animation.move>
     */
    setPosition: function(x, y) {
        this.position.x = x;
        this.position.y = y;
    },

    /*
     * Function: move
     *
     * Description:
     *
     * Move the animation
     *
     * Parameters:
     *
     * x/y - the pixels to move the animation
     *
     * See:
     *
     * <gamvas.Animation.setPosition>
     */
    move: function(x, y) {
        this.position.x += x;
        this.position.y += y;
    },

    /*
     * Function: setScale
     *
     * Description:
     *
     * Set a certain scale factor
     *
     * Parameters:
     *
     * s - the scale value (1 = no scale, < 1 = smaller, > 1 = bigger)
     *
     * See:
     *
     * <gamvas.Animation.scale>
     * <gamvas.Animation.setScaleXY>
     */
    setScale: function(s) {
        this.scaleFactor = s;
        this.scaleFactor2 = s;
    },

    /*
     * Function: scale
     *
     * Description:
     *
     * Scale the animation
     *
     * Parameters:
     *
     * s - the scale factor (< 0 = shrink, > 0 = enlarge)
     *
     * See:
     *
     * <gamvas.Animation.setScale>
     */
    scale: function(s) {
        this.scaleFactor += s;
        this.scaleFactor2 += s;
    },

    /*
     * Function: setScaleXY
     *
     * Description:
     *
     * Set a different scale for x and y axis
     *
     * Parameters:
     *
     * x - the scale of the x axis (1 = no scale, < 1 = smaller, > 1 = bigger)
     * y - the scale of the y axis (1 = no scale, < 1 = smaller, > 1 = bigger)
     *
     * See:
     *
     * <gamvas.Animation.setScale>
     */
    setScaleXY: function(x, y) {
        this.scaleFactor = x;
        this.scaleFactor2 = y;
    },

    /*
     * Function: setCenter
     *
     * Description:
     *
     * Set the center for an Animation. If you have a round object
     * for example with a size of 64 by 64 pixels and you want
     * to rotate it around the center, you would use
     * myObject.setCenter(32, 32);
     *
     * Parameters:
     *
     * x/y - the center, as seen of the upper left corner of the object
     */
    setCenter: function(x, y) {
        this.center.x = x;
        this.center.y = y;
    },

    /*
     * Function: draw
     *
     * Description:
     *
     * Draw the animation according to its position, rotation and scale settings
     *
     * Parameters:
     *
     * t - the time since last redraw
     *
     */
    draw: function(t) {
        this.drawFixed(t, this.c, this.position.x, this.position.y, -this.center.x, -this.center.y, this.rotation);
    },

    drawFixed: function(t, c, x, y, offX, offY, rot, scale) {
        if (this.needInit) {
            if ( (typeof this.sprite.width == 'undefined') || (this.sprite.width === 0) ) {
                return;
            }
            this.width = this.sprite.width;
            this.height = this.sprite.height;
            this.needInit = false;
        }
        this.currentFrameTime += t;
        if (this.currentFrameTime > this.fDur) {
            while (this.currentFrameTime > this.fDur) {
                this.currentFrameTime -= this.fDur;
            }
            this.currentFrame++;
            if (this.currentFrame >= this.numberOfFrames) {
                this.currentFrame = 0;
            }
        }
        this.drawFrame(c, this.currentFrame, x, y, offX, offY, rot, scale);
    },

    drawFrame: function(c, fn, x, y, offX, offY, rot, scale) {
        if (!gamvas.isSet(this.sprite)) return;

        if (this.frameList.length > 0) {
            fn = this.frameList[fn];
        }

        var px = this._frameX(fn)*this.width;
        var py = this._frameY(fn)*this.height;
        var tRot = this.rotation;
        var tOffX = -this.center.x;
        var tOffY = -this.center.y;
        var tScale = this.scaleFactor;
        if (gamvas.isSet(offX)) {
            tOffX = offX;
        }
        if (gamvas.isSet(offY)) {
            tOffY = offY;
        }
        if (gamvas.isSet(rot)) {
            tRot = rot;
        }
        if (gamvas.isSet(scale)) {
            tScale = scale;
        }
        c.save();
        c.translate(x, y);
        c.rotate(rot);
        c.scale(tScale, this.scaleFactor2);
        c.drawImage(this.sprite, px, py, this.width, this.height, -tOffX, -tOffY, this.width, this.height);
        c.restore();
    },

    _frameX: function(fn) {
        if (this.height === 0) return 0;
        var imagesX = this.sprite.width/this.width;
        return fn%imagesX;
    },

    _frameY: function(fn) {
        if (this.width === 0) return 0;

        var imagesX = this.sprite.width/this.width;
        return Math.floor(fn/imagesX);
    },

    /*
     * Function: setFrameList
     *
     * Description:
     *
     * Allows to set a list of frames that are considdered to be the animation
     *
     * Immagine a animation of a jumping ball, where on the first frame (0) it
     * is squashed as it hits the ground, then jumps up, slows down and falls
     * back down where it is squased again.
     *
     * You would want to play the ground hitting frames fast, while you would
     * want to play the slow down frames a bit slower, so assuming your animation
     * had 5 frames, you would simple repeat * them, by setting a framelist of
     * setFrameList([0, 1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 3, 3, 3, 2, 2, 1]);
     *
     * Parameters:
     *
     * fl - a array of frame indexes for the animation
     */
    setFrameList: function(fl) {
        this.frameList = fl;
        this.numberOfFrames = fl.length;
    }
});
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.physics
 *
 * Physics functions
 */
gamvas.physics = {
    /* Define: DYNAMIC
     *
     * define for a dynamic body
     *
     * Dynamic bodies are the objects that are under influence of forces
     */
    DYNAMIC: (typeof Box2D == 'undefined') ? null : Box2D.Dynamics.b2Body.b2_dynamicBody,

    /* Define: STATIC
     *
     * define for a static body
     *
     * Static bodies are the objects that do never move, like walls or ground objects
     */
    STATIC: (typeof Box2D == 'undefined') ? null : Box2D.Dynamics.b2Body.b2_staticBody,

    /* Define: KINEMATIC
     *
     * define for a kinematic body
     *
     * Kinematic bodies are objects that move, but not by physics engine, but by either
     * player input or AI
     */
    KINEMATIC: (typeof Box2D == 'undefined') ? null : Box2D.Dynamics.b2Body.b2_kinematicBody,

    /*
     * Variable: pixelsPerMeter
     *
     * Description:
     *
     * Sets the pixels per meter ratio.
     * The physics engine calculates in meters, so if for example you have
     * a car that is 4 meters long, then you set a bounding box with the
     * with of '4' for the physics engine, ofcourse a car with 4 pixels
     * in size would be only hard so see on the screen, so you can set
     * a pixels per meter ratio. Default is 64, so the image for your
     * 4 meter long car would have to be 256 pixels in with.
     *
     * Default:
     *
     * 64
     */
    pixelsPerMeter: 64,

    /*
     * Variable: velocityIterations
     *
     * Description:
     *
     * Sets the Box2D velocity iteration steps
     *
     * Default:
     *
     * 10
     *
     * See:
     *
     * http://box2d.org/manual.html
     */
    velocityIterations: 10,

    /*
     * Variable: positionIterations
     *
     * Description:
     *
     * Sets the Box2D position iteration steps
     *
     * Default:
     *
     * 8
     *
     * See:
     *
     * http://box2d.org/manual.html
     */
    positionIterations: 8,

    /*
     * Variable: debugAlpha
     *
     * Description:
     *
     * The alpha of physics debug information
     *
     * Default:
     *
     * 0.3
     */
    debugAlpha: 0.3,
    /*
     * Variable: debugStrokeWidth
     *
     * Description:
     *
     * The stroke width if you draw physics debug information
     *
     * Default:
     *
     * 2
     */
    debugStrokeWidth: 2,

    /*
     * Function: toScreen
     *
     * Description:
     *
     * Calculates a physics coordinate or size to the screen coordinate or size
     *
     * Parameters:
     *
     * v - the physics value
     *
     * Examples:
     *
     * > var physPos = myActor.body.GetPosition();
     * > var screenX = gamvas.physics.toScreen(physPos.x);
     */
    toScreen: function(v) {
        return v*gamvas.physics.pixelsPerMeter;
    },

    /*
     * Function: toWorld
     *
     * Description:
     *
     * Calculates a screen coordinate or size to a coordinate or size in the physics world
     *
     * Parameters:
     *
     * v - the screen value
     *
     * Examples:
     *
     * > var startPos = new gamvas.Vector2D(640, 480);
     * > myActor.body.SetPosition(
     * >    new Box2D.Common.Math.b2Vec2(
     * >       gamvas.physics.toWorld(startPos.x),
     * >       gamvas.physics.toWorld(startPos.y)
     * >    )
     * > );
     */
    toWorld: function(v) {
        return v/gamvas.physics.pixelsPerMeter;
    },

    _debugDrawer: null,
    /*
     * Function: drawDebug
     *
     * Description:
     *
     * Draw the physics debug information. Should be used after all objects are drawn.
     * This is a reduced debug
     * information with simpler handling then the Box2D version, you
     * still can use the Box2D way though.
     *
     * Example:
     *
     * > myState = gamvas.State.extend({
     * >     draw: function(t) {
     * >         this.drawAllObjects(t);
     * >         gamvas.physics.drawDebug();
     * >     }
     * > });
     */
    drawDebug: function() {
        if (this._debugDrawer === null) {
            this._debugDrawer = new Box2D.Dynamics.b2DebugDraw();
            this._debugDrawer.SetSprite(gamvas.getContext2D());
            this._debugDrawer.SetDrawScale(gamvas.physics.pixelsPerMeter);
            this._debugDrawer.SetFillAlpha(gamvas.physics.debugAlpha);
            this._debugDrawer.SetLineThickness(gamvas.physics.debugStrokeWidth);
            this._debugDrawer.SetFlags(Box2D.Dynamics.b2DebugDraw.e_shapeBit | Box2D.Dynamics.b2DebugDraw.e_jointBit);
            this._debugDrawer.m_sprite.graphics.clear = function() {};
            gamvas.physics.getWorld().SetDebugDraw(this._debugDrawer);
        }
        var c = gamvas.getContext2D();
        c.save();
        this.getWorld().DrawDebugData();
        c.restore();
    },

    /*
     * Function: setGravity
     *
     * Description:
     *
     * Sets the physics worlds gravity to a <gamvas.Vector2D>
     *
     * Example:
     *
     * myState = gamvas.State.extend({
     *     draw: function(t) {
     *         // ajust gravity to camera rotation
     *         var rot = this.camera.rotation;
     *         var vec = new gamvas.Vector2D(0. 9.8);
     *         gamvas.physics.setGravity(vec.rotate(-r));
     *     }
     * });
     */
    setGravity: function(vec) {
        var world = gamvas.physics.getWorld();
        if (gamvas.isSet(world)) {
            world.SetGravity(new Box2D.Common.Math.b2Vec2(vec.x, vec.y));
        }
    },

    /*
     * Function: resetWorld
     *
     * Description:
     *
     * Resets the physics world
     *
     * Parameters:
     *
     * gravx - the gravity in x direction (meters per second) (optional, default 0)
     * gravy - the gravity in y direction (meters per second) (optional, default 9.8)
     * slp - enable sleeping (optional, default true)
     * listener - a Box2D.Dynamics.b2ContactListener (optional)
     *
     * When sleeping is enabled, objects that have not moved for a while fall into a
     * sleep state so they are not simulated until a collider hits them.
     *
     * If no listener is specified (recommende) a default listener is installed, that
     * handles the actor collision. Usually this should be enough for almost any
     * purpose, but if you are a total Box2D guru, you might use a own listener
     *
     *
     * Note:
     *
     * Box2D and Gamvas differ in how the y coordinte is handled. While Box2D
     * tries to resemble real world sitation where a positive y coordinte would
     * go upward, gamvas is a 2D raster graphics game engine, where a positive y
     * coordinate would go downward.
     *
     * This leaves two special things that you should know.
     *
     * A) While in real world, gravity is roughly -9.8 (negative) meters per second, in gamvas
     * the default gravity is +9.8 (positive) to make objects fall down
     *
     * B) While Box2D asks you to use counter clock wise (CCW) polygons, in gamvas
     * you have to specify clock wise polygons
     *
     * Example:
     *
     * Start with a new empty world with default settings
     *
     * > gamvas.resetWorld();
     */
    resetWorld: function(gravx, gravy, slp, listener) {
        var doSleep = (typeof slp == 'undefined' ) ? true : slp;
        var gx = (gravx) ? gravx : 0;
        var gy = (gravy) ? gravy : 9.8;
        gamvas._doSleep = doSleep;
        gamvas._world = new Box2D.Dynamics.b2World(new Box2D.Common.Math.b2Vec2(gx, gy), doSleep);
        var l = (listener) ? listener : new Box2D.Dynamics.b2ContactListener();
        gamvas._world.SetContactListener(l);
    },

    /*
     * Function: getWorld
     *
     * Description:
     *
     * Get the Box2D world object
     *
     * Returns:
     *
     * The Box2D world object
     *
     * See:
     *
     * https://code.google.com/p/box2dweb/
     *
     * http://www.box2d.org/
     */
    getWorld: function() {
        return gamvas._world;
    }
};
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.physics.UserData
 *
 * Description:
 *
 * Holds information about the object related to a
 * a physics object
 *
 * Constructor:
 *
 * new gamvas.physics.UserData(type, data);
 *
 * Parameters:
 *
 * type - a string to identify the type of the data
 * data - the data object
 */
gamvas.physics.UserData = function(type, data) {
    /*
     * Variable: type
     *
     * Description:
     *
     * Type object type (e.g. 'actor')
     */
    this.type = type;

    /*
     * Variable: data
     *
     * Description:
     *
     * The actual object
     */
    this.data = data;
};
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
if ( (typeof Box2D != 'undefined') && (typeof Box2D.Dynamics.b2ContactListener != 'undefined') ) {
    gamvas.physics.ContactListener = Box2D.Dynamics.b2ContactListener;

    gamvas.physics.ContactListener.prototype.BeginContact = function(c) {
        var uda = c.GetFixtureA().GetBody().GetUserData();
        var udb = c.GetFixtureB().GetBody().GetUserData();
        if ( (uda.type == 'actor') && (udb.type == 'actor' ) ) {
            uda.data.onCollisionEnter(udb.data, c);
            udb.data.onCollisionEnter(uda.data, c);
        }
    };

    gamvas.physics.ContactListener.prototype.EndContact = function(c) {
        var uda = c.GetFixtureA().GetBody().GetUserData();
        var udb = c.GetFixtureB().GetBody().GetUserData();
        if ( (uda.type == 'actor') && (udb.type == 'actor' ) ) {
            uda.data.onCollisionLeave(udb.data, c);
            udb.data.onCollisionLeave(uda.data, c);
        }
    };

    gamvas.physics.ContactListener.prototype.PreSolve = function(c, om) {
        var uda = c.GetFixtureA().GetBody().GetUserData();
        var udb = c.GetFixtureB().GetBody().GetUserData();
        if ( (uda.type == 'actor') && (udb.type == 'actor' ) ) {
            if ( (!uda.data.doCollide(udb.data, c, om)) || (!udb.data.doCollide(uda.data, c, om))) {
                c.SetEnabled(false);
            }
        }
    };

    gamvas.physics.ContactListener.prototype.PostSolve = function(c, imp) {
        var uda = c.GetFixtureA().GetBody().GetUserData();
        var udb = c.GetFixtureB().GetBody().GetUserData();
        if ( (uda.type == 'actor') && (udb.type == 'actor' ) ) {
            var ni = imp.normalImpulses[0];
            var ti = imp.tangentImpulses[0];
            uda.data.onCollision(udb.data, ni, ti, c);
            udb.data.onCollision(uda.data, ni, ti, c);
        }
    };
}
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.ActorState
 *
 * Description:
 *
 * A class for actor states
 *
 * ActorState is the brain of a actor. Every actor has a default
 * ActorState, that you can use for your logic by using
 * <gamvas.Actor.getCurrentState> or you could add own states for
 * different situations, for example a
 * soldier ai could have states like patrol, detect, follow, fight
 *
 * Constructor:
 *
 * new gamvas.ActorState(name)
 *
 * Parameters:
 *
 * name - a unique state name within the actor
 */
gamvas.ActorState = gamvas.Class.extend({
	create: function(name) {
	    this._isInitialized = false;

	    /*
	     * Variable: name
	     *
	     * The name of the actor state
	     */
	    this.name = name;

	    /*
	     * Variable: actor
	     *
	     * The actor the state is assigned to, or null if not assigned
	     */
	    this.actor = null;
	},

	/*
	 * Function: init
	 *
	 * Description:
	 *
	 * Gets called once on state initialization.
	 * Overwrite with code that needs to be done one, like resource loading.
	 */
	init: function() {
	},

	/*
	 * Function: enter
	 *
	 * Description:
	 *
	 * Gets called when the state is entered.
	 */
	enter: function() {
	},

	/*
	 * Function: leave
	 *
	 * Description:
	 *
	 * Gets called when the state is left.
	 */
	leave: function() {
	},

	/*
	 * Function: preDraw
	 *
	 * Description:
	 *
	 * Gets called after screen clear and before camera handling.
	 * Use this for example for static background elements that do not move with the camera.
	 *
	 * Parameters:
	 *
	 * t - the time since last redraw
	 */
	preDraw: function(t) {
	},

	/*
	 * Function: draw
	 *
	 * Description:
	 *
	 * Gets called when the state should draw.
	 * In most cases you would not overwrite this function.
	 * If you put logic like ai in a actor, overwrite the
	 * update function.
	 *
	 * Parameters:
	 *
	 * t - the time since last redraw
	 *
	 * See:
	 *
	 * <gamvas.ActorState.update>
	 */
	draw: function(t) {
	    var anim = this.actor.getCurrentAnimation();
	    anim.drawFixed(t, gamvas.getContext2D(), this.actor.position.x, this.actor.position.y, this.actor.center.x, this.actor.center.y, this.actor.rotation, this.actor.scaleFactor);
	},

	/*
	 * Function: postDraw
	 *
	 * Description:
	 *
	 * Gets called after camera handling.
	 * Use this to render static things in the screen forground, like score or health.
	 *
	 * Parameters:
	 *
	 * t - the time since last redraw
	 */
	postDraw: function(t) {
	},

	/*
	 * Function: update
	 *
	 * Description:
	 *
	 * Update the ai.
	 * Overwrite with things your actor should do on a per frame basis.
	 *
	 * Parameters:
	 *
	 * t - the time since last redraw
	 */
	update: function(t) {
	},

	/*
	 * Function: onKeyDown
	 *
	 * Description:
	 *
	 * A key on the keyboard was pressed
	 *
	 * Note:
	 *
	 * Requires the actor to be registered with <gamvas.State.registerInputEvents>
	 *
	 * Parameters:
	 *
	 * keyCode - The keycode of the key (e.g. gamvas.key.RETURN)
	 * character - The actual character (e.g. 'a')
	 * ev - The JavaScript event object
	 *
	 * See:
	 *
	 * <gamvas.State.registerInputEvents>
	 * <gamvas.ActorState.onKeyUp>
	 *
	 * Example:
	 *
	 * > myActorState = gamvas.ActorState.extend({
	 * >     onKeyDown: function(keyCode) {
	 * >         if (keyCode == gamvas.key.SPACE) {
	 * >             this.firePlayerGun();
	 * >         }
	 * >     },
	 * >     firePlayerGun() {
	 * >         console.log('BAZOING!');
	 * >     }
	 * > });
	 */
	onKeyDown: function(keyCode, character, ev) {
	},

	/*
	 * Function: onKeyUp
	 *
	 * Description:
	 *
	 * A key on the keyboard was released
	 *
	 * Parameters:
	 *
	 * keyCode - The keycode of the key (e.g. gamvas.key.RETURN)
	 * character - The actual character (e.g. 'a')
	 * ev - The JavaScript event object
	 *
	 * See:
	 *
	 * <gamvas.ActorState.onKeyDown>
	 */
	onKeyUp: function(keyCode, character, ev) {
	},

	/*
	 * Function: onMouseDown
	 *
	 * Description:
	 *
	 * A mouse button was pressed
	 *
	 * Parameters:
	 *
	 * button - The mouse button that was pressed (e.g. gamvas.mouse.LEFT)
	 * x/y - The position on the screen the mousepointer was while pressed
	 * ev - The JavaScript event object
	 *
	 * See:
	 *
	 * <gamvas.mouse>
	 */
	onMouseDown: function(button, x, y, ev) {
	},

	/*
	 * Function: onMouseUp
	 *
	 * Description:
	 *
	 * A mouse button was released
	 *
	 * Parameters:
	 *
	 * button - The mouse button that was released (e.g. gamvas.mouse.LEFT)
	 * x/y - The position on the screen the mousepointer was while released
	 * ev - The JavaScript event object
	 *
	 * See:
	 *
	 * <gamvas.mouse>
	 */
	onMouseUp: function(button, x, y, ev) {
	},

	/*
	 * Function: onMouseMove
	 *
	 * Description:
	 *
	 * The mouse was moved
	 *
	 * Parameters:
	 *
	 * x/y - The position where the mousecursor was
	 * ev - The JavaScript event object
	 *
	 * Example:
	 *
	 * > myActorState = gamvas.ActorState.extend({
	 * >     onMouseMove: function(x, y) {
	 * >        this.followPosition(x, y);
	 * >     }
	 * > });
	 */
	onMouseMove: function(x, y, ev) {
	},

	/*
	 * Function: onCollision
	 *
	 * Description:
	 *
	 * The actor starts a collision with another actor
	 *
	 * Parameters:
	 *
	 * a - the colliding actor
	 * ni - the normal impulse (aka how hard did we hit)
	 * ti - the tangent impulse (how much rotational force did we get out of the collision)
	 * c - a b2Contact object holding low level information about the contact
	 *
	 * See:
	 * <gamvas.actorstate.onCollisionEnter>
	 * <gamvas.actorstate.onCollisionLeave>
	 *
	 * Example:
	 *
	 * > spaceShipFlying = gamvas.ActorState.extend({
	 * >     onCollision: function(a, ni) {
	 * >         if ( (a.type == "asteroid") && (ni > 15) ) {
	 * >             console.log("here is your captain speaking, we got hit hard by a asteroid... abandon ship!");
	 * >         }
	 * >     }
	 * > });
	 */
	onCollision: function(a, ni, ti, c) {
	},

	/*
	 * Function: onCollisionEnter
	 *
	 * Description:
	 *
	 * The actor starts a collision with another actor
	 *
	 * Parameters:
	 *
	 * a - the colliding actor
	 * c - a b2Contact object holding low level information about the contact
	 *
	 * See:
	 * <gamvas.actorstate.onCollisionEnter>
	 *
	 * Example:
	 *
	 * > myActorState = gamvas.ActorState.extend({
	 * >     onCollisionEnter: function(a) {
	 * >         console.log("i got hit by "+a.name);
	 * >     }
	 * > });
	 */
	onCollisionEnter: function(a, c) {
	},

	/*
	 * Function: onCollisionLeave
	 *
	 * Description:
	 *
	 * The actor leaves a collision with another actor
	 *
	 * Parameters:
	 *
	 * a - the colliding actor
	 * c - a b2Contact object holding low level information about the contact
	 *
	 * See:
	 * <gamvas.actorstate.onCollisionEnter>
	 */
	onCollisionLeave: function(a, c) {
	},

	/*
	 * Function: doCollide
	 *
	 * Description:
	 *
	 * This function is called to check of two objects should collide.
	 * If one of the two objects returns false, the collision between
	 * the two objects will be ignored.
	 *
	 * This is a important function for creating jump and run games.
	 * On collision with so called cloud objects - which are typical
	 * jump and run objects that you can for example jump through from
	 * below but not fall down when standing on them, you would check
	 * if you jump upwards, then disable the collision, or if you fall
	 * downwards and are positioned above the colliding object, you
	 * would enable the collision.
	 *
	 * Parameters:
	 *
	 * a - the colliding actor
	 * c - a b2Contact object holding low level information about the contact
	 * om - the old b2Manifold object holding information about the collision points
	 *
	 * See:
	 * <gamvas.actorstate.onCollisionEnter>
	 *
	 * Example:
	 *
	 * Do not collide with ghost objects
	 *
	 * > myActorState = gamvas.ActorState.extend({
	 * >     doCollide: function(opponent) {
	 * >         if (opponent.type == "ghost") {
	 * >             return false;
	 * >         }
	 * >         return true;
	 * >     }
	 * > });
	 */
	doCollide: function(a, c, om) {
	    return true;
	}
});
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.Actor
 *
 * Description:
 *
 * The actor class is the most important class in gamvas. You use it
 * for the player, ai opponents, physics objects, maybe even static objects
 * in your gameworld.
 * It forms a object that can hold animations, physical properties, states, logic
 * and can react to events like keyboard or mouse input or collisions.
 * Actors hold a default animation and a default state, so if you only need one of them,
 * you can use these, see example below.
 *
 * Constructur:
 *
 * new gamvas.Actor(name, x, y);
 *
 * Parameters:
 *
 * name - a unique name in the game world
 * x/y - the position of the actors center in the world (optional)
 *
 * See:
 *
 * <gamvas.ActorState>
 * <gamvas.Class>
 * <gamvas.Image>
 *
 * Example:
 *
 * > // extend gamvas.Actor to create a new instancable actor
 * > myActor = gamvas.Actor.extend({
 * >     // the create function is the constructor
 * >     create: function(name, x, y) {
 * >         // call our super class constructor to initialize the actor
 * >         this._super(name, x, y);
 * >         // get our current state to get access to the resource handler
 * >         var st = gamvas.state.getCurrentState();
 * >         // load our animation with 64x64 pixels per frames, 10 frames
 * >         // playing with 20 frames per second
 * >         this.setFile(st.resource.getImage('anim.png', 64, 64, 10, 20));
 * >         // get default actor state
 * >         var defState = this.myActor.getCurrentState();
 * >         // install brain, move 10 pixels per second to the right
 * >         defState.update = function(t) {
 * >             this.actor.move(10*t, 0);
 * >         };
 * >     }
 * > });
 */
gamvas.Actor = gamvas.Class.extend({
    create: function(name, x, y) {
        /*
         * Variable: name
         *
         * The name of the actor
         */
        this.name = (name) ? name : 'unnamed';

        /*
         * Variable: position
         *
         * A <gamvas.Vector2D> object with the position information
         *
         * See:
         *
         * <gamvas.actor.setPosition>
         * <gamvas.actor.move>
         */
        this.position = new gamvas.Vector2D((x)?x:0, (y)?y:0);

        /*
         * Variable: center
         *
         * A <gamvas.Vector2D> object with the center offset of the actor
         *
         * See:
         *
         * <gamvas.actor.setCenter>
         */
        this.center = new gamvas.Vector2D(0, 0);
        this.animations = [];

        /*
         * Variable: currentAnimation
         *
         * The name of the current animation
         */
        this.currentAnimation = null;

        /*
         * Variable: rotation
         *
         * The rotation of the actor
         *
         * See:
         *
         * <gamvas.actor.setRotation>
         * <gamvas.actor.rotate>
         */
        this.rotation = 0;

        /*
         * Variable: scaleFactor
         *
         * The scale factor of the object
         *
         * Note:
         *
         * Scaling actors does not work with physics. Well... scaling does, but
         * the physics collision box will not be scaled.
         */
        this.scaleFactor = 1;

        /*
         * Variable: density
         *
         * The physical property of density.
         * In case of impact, objects with higher density will push objects
         * with lower density away more.
         *
         * Default:
         * 1.0
         */
        this.density = 1.0;

        /*
         * Variable: friction
         *
         * The physical property of friction
         * Defines the friction of objects. A ice ground would have a very
         * low friction, maybe 0.1 or less, while a rubber ground would have
         * a high value, close to 1.0. So if you would push a ball over these
         * grounds, on the low friction ice ground, the ball might slip over
         * where as on the high friction rubber ground it would immediately start
         * to roll.
         *
         * Default:
         * 0.5
         */
        this.friction = 0.5;

        /*
         * Variable: resitution
         *
         * The physical property of resitution (bounce).
         * Defines how bouncy a object is. A value of 1.0 means, that
         * if falling to the ground, the object would jump exactly as
         * high up as it fell down, wile smaller values will give a
         * more natural bouncyness where with every bounce, it would
         * bounce less, until it stops bouncing.
         *
         * Default:
         * 0.3
         */
        this.restitution = 0.3;

        /*
         * Variable: type
         *
         * Allows you to set types to your physics objects to identify
         * them. You could for example use "player", "wall", "opponent",
         * "tree" to identify what type of object you are colliding
         * and react accordingly.
         *
         * Default:
         * empty string
         */
        this.type = "";

        /*
         * Variable: layer
         *
         * Integer value used for z-sorting on automatic draw through <gamvas.State.addActor>
         *
         * Default:
         * 0
         */
        this.layer = 0;

        /*
         * Variable: usePhysics
         *
         * Delivers true/false if this object has physics enabled.
         * This is meant to be read only, it will be set automatically
         * when creating a physics body.
         *
         * See:
         *
         * <gamvas.Actor.createBody>
         * <gamvas.Actor.bodyRect>
         * <gamvas.Actor.bodyCircle>
         * <gamvas.Actor.bodyPolygon>
         */
        this.usePhysics = false;
        this._isActive = true;

        /*
         * Variable: body
         *
         * The Box2D b2Body, if physics is enabled
         */
        this.body = null;
        this.bodyDef = null;

        /*
         * Variable: fixture
         *
         * The Box2D b2Fixture, if physics is enabled
         */
        this.fixture = null;
        this.fixtureDef = null;

        this.states = [];
        this.currentState = null;

        this.addState(new gamvas.ActorState('default'));
        this.currentState = 'default';

        this.addAnimation(new gamvas.Animation('default'));
        this.currentAnimation = 'default';
        this.init();
    },

    /*
     * Function: init
     *
     * Description
     *
     * Overwrite with code to get executed on actor initialisation
     */
    init: function() {
    },

    /*
     * Function: addState
     *
     * Description:
     *
     * Adds a actor state
     *
     * Parameters:
     *
     * state - a <gamvas.ActorState> object
     * activate - if true, immediately switch to the new state (optional)
     *
     * Example:
     *
     * > myRunningState = gamvas.ActorState.extend({
     * >     update: function(t) {
     * >          this.actor.move(100*t, 0);
     * >     },
     * >     onKeyUp: function(keyCode) {
     * >         if ( (keyCode = gamvas.key.RIGHT) || (keyCode = gamvas.key.LEFT) ) {
     * >             this.actor.setState('walking');
     * >         }
     * >     }
     * > });
     * > myActor = gamvas.Actor.extend({
     * >     create: function(name, x, y) {
     * >         this._super(name, x, y);
     * >         this.addState(new myRunningState('running'));
     * >     }
     * > });
     */
    addState: function(state, activate) {
         this.states[state.name] = state;
         this.states[state.name].actor = this;
         if ( (gamvas.isSet(activate)) && (activate) ) {
             this.setState(state.name);
         }
    },

    /*
     * Function: addAnimation
     *
     * Description:
     *
     * Adds a animation
     *
     * Parameters:
     *
     * anim - a <gamvas.Animation> object
     *
     * Example:
     *
     * > myActor = gamvas.Actor.extend({
     * >     create: function(name, x, y) {
     * >         this._super(name, x, ,y);
     * >         this.addAnimation(
     * >             new gamvas.Animation('running', this.resource.getImage('playerRun.png'), 64, 64, 12, 20)
     * >         );
     * >     }
     * > });
     */
    addAnimation: function(anim) {
        this.animations[anim.name] = anim;
    },

    /*
     * Function: createBody
     *
     * Description:
     *
     * Create a Box2D b2Body object for the actor.
     * This is the most flexible way to create a body, but you
     * have to do the Box2D stuff quite low level
     *
     * Note:
     *
     * This is the low level function to add physics to your object.
     * Unless you are required to for some reason, you would normally
     * not use this function directly, but use <gamvas.Actor.bodyRect>,
     * <gamvas.Actor.bodyCircle> or <gamvas.Actor.bodyPolygon>
     *
     * Parameters:
     *
     * type - the Box2D body type (dynamic/static/...)
     * shape - the Box2D shape for collision detection
     *
     * Types:
     *
     * gamvas.physics.DYNAMIC - A body that moves, collides, etc (<gamvas.physics.DYNAMIC>)
     * gamvas.physics.STATIC - A body that does not move, like ground, walls, etc (<gamvas.physics.STATIC>)
     * gamvas.physics.KINEMATIC - A body that does move, but under player or AI controll (<gamvas.physics.KINEMATIC>)
     *
     * See:
     *
     * <gamvas.Actor.bodyRect>
     * <gamvas.Actor.bodyPolygon>
     * <gamvas.Actor.bodyCircle>
     *
     * Example:
     *
     * > myPlayer = gamvas.Actor.extend({
     * >     create: function(name, x ,y) {
     * >         this._super(name, x, y);
     * >         // create a polygon shape
     * >         var shape = new Box2D.Collision.Shapes.b2PolygonShape;
     * >         // create a box shape with 200 by 100 pixels
     * >         // NOTE: For demonstration only, use gamvas.Actor.bodyRect for this scenario
     * >         shape.SetAsBox(gamvas.physics.toWorld(200), gamvas.physics.toWorld(100));
     * >         // now finally create the body, adds the actor to physics simulation automatically
     * >         this.createBody(gamvas.physics.DYNAMIC, shape);
     * >     }
     * > });
     */
    createBody: function(type, shape) {
        this.bodyDef = new Box2D.Dynamics.b2BodyDef;
        this.bodyDef.type = type;
        this.fixtureDef = new Box2D.Dynamics.b2FixtureDef;
        this.fixtureDef.density = this.density;
        this.fixtureDef.friction = this.friction;
        this.fixtureDef.restitution = this.restitution;
        this.fixtureDef.shape = shape;
        this.body = gamvas.physics.getWorld().CreateBody(this.bodyDef);
        this.body.SetUserData(new gamvas.physics.UserData('actor', this));
        this.fixture = this.body.CreateFixture(this.fixtureDef);
        this.usePhysics = true;
    },

    /*
     * Function: bodyRect
     *
     * Description:
     *
     * make the actor a physics object with a rectangular body shape
     *
     * Parameters:
     *
     * x/y - the position of the body in the physics world
     * width/height - the half dimension of the rectangle, in pixels
     * type - the Box2D body type (See <gamvas.Actor.createBody>) (optional)
     *
     * See:
     * <gamvas.Actor.createBody>
     * <gamvas.Actor.bodyPolygon>
     * <gamvas.Actor.bodyCircle>
     */
    bodyRect: function(x, y, width, height, type) {
        if (!gamvas.isSet(type)) {
            type = Box2D.Dynamics.b2Body.b2_dynamicBody;
        }
        var hw = width/2;
        var hh = height/2;
        var s = new Box2D.Collision.Shapes.b2PolygonShape;
        s.SetAsBox(gamvas.physics.toWorld(hw), gamvas.physics.toWorld(hh));
        this.createBody(type, s);
        this.center.x = hw;
        this.center.y = hh;
        this.setPosition(x, y);
    },

    /*
     * Function: bodyCircle
     *
     * Description:
     *
     * make the actor a physics object with a circle as body shape
     *
     * Parameters:
     *
     * x/y - the position of the body in the physics world
     * radius - the radius of the circle in pixels
     * type - the Box2D body type (See <gamvas.Actor.createBody>) (optional)
     *
     * See:
     * <gamvas.Actor.createBody>
     * <gamvas.Actor.bodyRect>
     * <gamvas.Actor.bodyPolygon>
     */
    bodyCircle: function(x, y, radius, type) {
        if (!gamvas.isSet(type)) {
            type = Box2D.Dynamics.b2Body.b2_dynamicBody;
        }
        var s = new Box2D.Collision.Shapes.b2CircleShape(gamvas.physics.toWorld(radius));
        this.createBody(type, s);
        this.center.x = radius;
        this.center.y = radius;
        this.setPosition(x, y);
    },

    /*
     * Function: bodyPolygon
     *
     * Description:
     *
     * make the actor a physics object with a non rectangular polygon shape
     * If you just need a rectangle, there is a shortcut: <gamvas.actor.bodyRect>
     *
     * Note:
     *
     * Although Box2D documentation requires a counter clock wise (CCW) polygon, gamvas does
     * require you to secify a clock wise polygon, as in the gamvas worl, the y axis
     * rpresents screen pixels and therefor runs down, whereas Box2D world represents
     * a real life world, where positive values on the y axis run up.
     *
     * Parameters:
     *
     * x/y - the position of the body in the physics world
     * polys - a array holding arrays of the pixel coordinates of the vertices
     * cx/cy - the center of the polygon object (optional)
     * type - the Box2D body type (See <gamvas.Actor.createBody>) (optional)
     *
     * See:
     * <gamvas.Actor.createBody>
     * <gamvas.Actor.bodyRect>
     * <gamvas.Actor.bodyCircle>
     *
     * Example:
     *
     * Create a triangle object with a image of the size 64 by 64 pixels with the center in the middle, at screen position 200, 50
     *
     * > this.bodyPolygon(200, 50, [ [32, 0], [64, 64], [0, 64] ], 32, 32);
     */
    bodyPolygon: function(x, y, polys, cx, cy, type) {
        if (!gamvas.isSet(type)) {
            type = Box2D.Dynamics.b2Body.b2_dynamicBody;
        }
        var s = new Box2D.Collision.Shapes.b2PolygonShape;
        var v = [];
        for (var i = 0; i < polys.length; i++) {
            if ( (polys[i].length>1) ) {
                v.push(new Box2D.Common.Math.b2Vec2(gamvas.physics.toWorld(polys[i][0]-cx), gamvas.physics.toWorld(polys[i][1]-cy)));
            }
        }

        s.SetAsArray(v, v.length);
        this.createBody(type, s);
        this.setCenter(cx, cy);
        this.setPosition(x, y);
    },

    updatePhysics: function() {
        var pos = this.body.GetPosition();
        this.position.x = pos.x * gamvas.physics.pixelsPerMeter;
        this.position.y = pos.y * gamvas.physics.pixelsPerMeter;
        this.rotation = this.body.GetAngle();
    },

    /*
     * Function: resetForces
     *
     * Description:
     *
     * Resets all physical forces on the actor
     *
     * When called without parameter, it just stops rotation and movement.
     * With the optional parameters, you can fully reset the actor to
     * its starting position/rotation
     *
     * Parameters:
     * x/y - The position to set the actor to (optional)
     * r - The rotation to set the actor to (in radians)
     */
    resetForces: function(x, y, r) {
        this.body.SetLinearVelocity(new Box2D.Common.Math.b2Vec2(0, 0));
        this.body.SetAngularVelocity(0);
        if ( (typeof x != 'undefined') && (typeof y != 'undefined') ) {
            this.setPosition(x, y);
            if (typeof r != 'undefined') {
                this.setRotation(r);
            }
        }
        this.body.SetAwake(true);
    },

    /*
     * Function: setBullet
     *
     * Description:
     *
     * Defines if a actor is a physics 'bullet'.
     *
     * A bullet gets increased precision (but slower) collision
     * calculation. Immagine a bullet flying towards a thin piece
     * of metal. In real world, that bullet would hit the metal,
     * no matter what, because the real world is not running in
     * frames per second. The physics simulation on the other
     * hand does, therefor, if the bullet moves very fast, the
     * bullet actually skips through the air, because it traveled
     * a certain amount since the last frame redraw. This can
     * lead to very fast moving objects falling through walles
     * that they would collide when they would move slower, because
     * they skiped a distance larger then this colliding objects
     * width.
     *
     * By setting a actor to a bullet, you can ensure that it always
     * collides, even if it skips that part because of the frame
     * based simulation.
     *
     * Note:
     *
     * This is a expensive operation, only use it on important objects
     * (like e.b. the players spaceship) and only if necessary because
     * your objects falls through stuff that it should collide with
     *
     * Parameters:
     *
     * b - true/false
     *
     * Default:
     *
     * false
     */
    setBullet: function(b) {
        this.body.SetBullet(b);
    },

    /*
     * Function: setSensor
     *
     * Defines if a actor is a physics sensor or not
     *
     * Sensors do recognize collisions, but are not influenced by
     * the collision forces, so will continue moving as there would
     * not be a collision, but there is one.
     *
     * You would use sensors to trigger events, like open a door
     * if the player stands right before it.
     *
     * Parameters:
     *
     * b - true/false
     *
     * Default:
     *
     * false
     */
    setSensor: function(b) {
        this.fixture.SetSensor(b);
    },

    /*
     * Function: setAwake
     *
     * Description:
     *
     * Allows you to wake (or sleep) a actor in the physics simulation
     *
     * Sleeping actors are not considered in the simulation, until
     * something hits them. Then they are automatically switched awake.
     *
     * Parameters:
     *
     * b - true/false
     */
    setAwake: function(b) {
        this.body.SetAwake(b);
    },

    /*
     * Function: setFixedRotation
     *
     * Description:
     *
     * Object does not rotate, when under physics controll
     *
     * Parameters:
     *
     * b - true/false
     *
     * Default:
     *
     * false
     */
    setFixedRotation: function(b) {
        this.body.SetFixedRotation(b);
    },

    /*
     * Function: setName
     *
     * Description:
     *
     * Set the actor name
     *
     * Parameters:
     *
     * name - the actors name, must be unique in the game world
     */
    setName: function(name) {
        this.name = name;
    },

    /*
     * Function: setRotation
     *
     * Description:
     *
     * Set certain rotation of the actor in radians
     *
     * Parameters:
     *
     * r - the rotation in radians
     *
     * See:
     *
     * <gamvas.Actor.rotate>
     * http://en.wikipedia.org/wiki/Radians
     */
    setRotation: function(r) {
        this.rotation = r;
        if (this.usePhysics) {
            this.body.SetAngle(r);
        }
    },

    /*
     * Function: rotate
     *
     * Description:
     *
     * Rotate the actor
     *
     * Parameters:
     *
     * r - the amount to rotate the actor in radians
     *
     * See:
     *
     * <gamvas.Actor.setRotation>
     * http://en.wikipedia.org/wiki/Radians
     */
    rotate: function(r) {
        this.rotation += r;
    },

    /*
     * Function: setPosition
     *
     * Description:
     *
     * Set the position of a actor
     *
     * Parameters:
     *
     * x/y - the position of the actor in pixels
     *
     * See:
     *
     * <gamvas.Actor.move>
     */
    setPosition: function(x, y) {
        this.position.x = x;
        this.position.y = y;
        if (this.usePhysics) {
            this.body.SetPosition(new Box2D.Common.Math.b2Vec2(x/gamvas.physics.pixelsPerMeter, y/gamvas.physics.pixelsPerMeter));
        }
    },

    /*
     * Function: move
     *
     * Description:
     *
     * Move the actor
     *
     * Parameters:
     *
     * x/y - the pixels to move the actor
     *
     * See:
     *
     * <gamvas.Actor.setPosition>
     */
    move: function(x, y) {
        this.position.x += x;
        this.position.y += y;
    },

    /*
     * Function: setScale
     *
     * Description:
     *
     * Set a certain scale factor
     *
     * Note:
     *
     * Do not use scale for objects under physics controll
     * it will work for the image, but not for the collision
     * object
     *
     * Parameters:
     *
     * s - the scale value (1 = no scale, < 1 = smaller, > 1 = bigger)
     *
     * See:
     *
     * <gamvas.Actor.scale>
     */
    setScale: function(s) {
        this.scaleFactor = s;
    },

    /*
     * Function: scale
     *
     * Description:
     *
     * Scale a object
     *
     * Note:
     *
     * Do not use scale for objects under physics controll
     * it will work for the image, but not for the collision
     * object
     *
     * Parameters:
     *
     * s - the scale factor (< 0 = shrink, > 0 = enlarge)
     *
     * See:
     *
     * <gamvas.Actor.setScale>
     */
    scale: function(s) {
        this.scaleFactor += s;
    },

    /*
     * Function: setCenter
     *
     * Description:
     *
     * Set the center for an actor. If you have a round object
     * for example with a size of 64 by 64 pixels and you want
     * to rotate it around the center, you would use
     * myObject.setCenter(32, 32);
     *
     * Parameters:
     *
     * x/y - the center, as seen of the upper left corner of the object
     */
    setCenter: function(x, y) {
        this.center.x = x;
        this.center.y = y;
    },

    /*
     * Function: getCurrentAnimation
     *
     * Description:
     *
     * Get the current <gamvas.Animation> object that is playing
     *
     * Returns:
     *
     * <gamvas.Animation>
     */
    getCurrentAnimation: function() {
        return this.animations[this.currentAnimation];
    },

    /*
     * Function: getCurrentState
     *
     * Description:
     *
     * Get the current <gamvas.ActorState> the actor is in
     *
     * Returns:
     *
     * <gamvas.ActorState>
     */
    getCurrentState: function() {
        return this.states[this.currentState];
    },

    /*
     * Function: setFile
     *
     * Description:
     *
     * Sets a image file for the current animation
     *
     * Parameters:
     *
     * file - a JavaScript Image object holding the animation in tiles
     * frameWidth - the width of a single frame tile of the animation
     * frameHeight - the height of a single frame tile of the animation
     * numberOfFrames - the number of frames in the animation
     * fps - the speed of the animation in frames per second
     *
     * Example:
     *
     * > myActor = gamvas.Actor.extend({
     * >     create: function(name, x, y) {
     * >         this._super(name, x,, y);
     * >         var st = gamvas.state.getCurrentState();
     * >         this.setFile(st.resource.getImage('anim.png', 64, 64, 10, 20));
     * >     }
     * > });
     */
    setFile: function(file, frameWidth, frameHeight, numberOfFrames, fps) {
        this.getCurrentAnimation().setFile(file, frameWidth, frameHeight, numberOfFrames, fps);
    },

    /*
     * Function: preDraw
     *
     * Description:
     *
     * Gets called after screen clear and before camera handling.
     *
     * Parameters:
     *
     * t - the time since last redraw
     *
     * See:
     *
     * <gamvas.ActorState.update>
     */
    preDraw: function(t) {
        var s = this.getCurrentState();
        if (s) {
            s.preDraw(t);
        }
    },

    /*
     * Function: draw
     *
     * Description:
     *
     * Gets called when the actor is drawn. Usually you would not
     * overwrite this function with your logic, use the <gamvas.ActorState.update>
     * function of either the default state or a custom <gamvas.ActorState>
     *
     * Parameters:
     *
     * t - the time since last redraw
     *
     * See:
     *
     * <gamvas.ActorState.update>
     */
    draw: function(t) {
        var s = this.getCurrentState();
        if (s) {
            s.update(t);
            s.draw(t);
        }
    },

    /*
     * Function: postDraw
     *
     * Description:
     *
     * Gets called after camera handling.
     *
     * Parameters:
     *
     * t - the time since last redraw
     *
     * See:
     *
     * <gamvas.ActorState.update>
     */
    postDraw: function(t) {
        var s = this.getCurrentState();
        if (s) {
            s.postDraw(t);
        }
    },

    /*
     * Function: isActive
     *
     * Description:
     *
     * Check if the actor is active
     *
     * See:
     *
     * <gamvas.Actor.setActive>
     */
    isActive: function() {
        return this._isActive;
    },

    /*
     * Function: setActive
     *
     * Description:
     *
     * Enable or disable the actor for automatic drawing (if in states actors list)
     *
     * See:
     *
     * <gamvas.State.addActor>
     */
    setActive: function(yesno) {
        this._isActive = yesno;
    },

    /*
     * Function: setState
     *
     * Description:
     *
     * Switch the actor to a certain <gamvas.ActorState>
     *
     * Parameters:
     *
     * stateName - the name of the <gamvas.ActorState>
     *
     * See:
     *
     * <gamvas.ActorState>
     */
    setState: function(stateName) {
        if (gamvas.isSet(this.states[stateName])) {
            if (this.currentState !== null) {
                var cur = this.getCurrentState();
                if (cur) {
                    cur.leave();
                }
            }

            var st = this.states[stateName];
            if (!st._isInitialized) {
                st.init();
                st._isInitialized = true;
            }
            st.enter();
            this.currentState = stateName;
        }
    },

    /*
     * Function: setAnimation
     *
     * Description:
     *
     * Switch the actor to a certain <gamvas.Animation>
     *
     * Parameters:
     *
     * a - the name of the <gamvas.Animation>
     *
     * See:
     *
     * <gamvas.Animation>
     */
    setAnimation: function(a) {
        if (gamvas.isSet(this.animations[a])) {
            this.currentAnimation = a;
        }
    },

    onCollision: function(a, ni, ti, c) {
        var cur = this.getCurrentState();
        if (cur) {
            cur.onCollision(a, ni, ti, c);
        }
    },

    onCollisionEnter: function(a, c) {
        var cur = this.getCurrentState();
        if (cur) {
            cur.onCollisionEnter(a, c);
        }
    },

    onCollisionLeave: function(a, c) {
        var cur = this.getCurrentState();
        if (cur) {
            cur.onCollisionLeave(a, c);
        }
    },

    doCollide: function(a, c, om) {
        var cur = this.getCurrentState();
        if (cur) {
            return cur.doCollide(a, c, om);
        }
        return true;
    },

    onKeyDown: function(keyCode, character, ev) {
        var cur = this.getCurrentState();
        if (cur) {
            return cur.onKeyDown(keyCode, character, ev);
        }
	return false;
    },

    onKeyUp: function(keyCode, character, ev) {
        var cur = this.getCurrentState();
        if (cur) {
            return cur.onKeyUp(keyCode, character, ev);
        }
	return false;
    },

    onMouseDown: function(button, x, y, ev) {
        var cur = this.getCurrentState();
        if (cur) {
            return cur.onMouseDown(button, x, y, ev);
        }
	return false;
    },

    onMouseUp: function(button, x, y, ev) {
        var cur = this.getCurrentState();
        if (cur) {
            return cur.onMouseUp(button, x, y, ev);
        }
	return false;
    },

    onMouseMove: function(x, y, ev) {
        var cur = this.getCurrentState();
        if (cur) {
            return cur.onMouseMove(x, y, ev);
        }
	return false;
    },

    /*
     * Function: setFPS
     *
     * Sets the frames per seconds of the current animation
     *
     * See:
     *
     * <gamvas.Animation.setFPS> for more information
     */
    setFPS: function(fps) {
        this.getCurrentAnimation().setFPS(fps);
    },

    /*
     * Function: setFrameList
     *
     * Sets the list of frames of the current animation
     *
     * See:
     *
     * <gamvas.Animation.setFrameList> for more information
     */
    setFrameList: function(fl) {
        this.getCurrentAnimation().setFrameList(fl);
    },

    /*
     * Function: setGroupIndex
     *
     * Description:
     *
     * Set the Box2D filter group index
     *
     * All members of a negative group never collide, or of a positive group
     * always collide
     *
     * Parameters:
     *
     * g - The group index, negative or positive values disable or enable collision
     *
     * Example:
     *
     * Set 4 objects group index, so ncol1 and ncol2 never collide with each other
     * but all other objects collide with everything
     *
     * > this.ncol1.setGroupIndex(-1);
     * > this.ncol2.setGroupIndex(-1);
     * > this.col1.setGroupIndex(1);
     * > this.col2.setGroupIndex(1);
     */
    setGroupIndex: function(g) {
        var fd = null;
        for (f = this.body.GetFixtureList(); f; f = f.m_next) {
            fd = f.GetFilterData();
            fd.groupIndex = g;
            f.SetFilterData(fd);
        }
    },

    /*
     * Function: setCategoryBits
     *
     * Description:
     *
     * Sets the Box2D category bits
     *
     * You can have up to 16 categories for your physics objects
     * like for example player, monster, coins and together with
     * <gamvas.actor.setMaskBits> you can specify which category
     * can collide with which
     *
     * Parameters:
     *
     * b - a bitfield which category the current object belongs
     *
     * See:
     *
     * <gamvas.actor.setMaskBits>
     */
    setCategoryBits: function(b) {
        var fd = null;
        for (f = this.body.GetFixtureList(); f; f = f.m_next) {
            fd = f.GetFilterData();
            fd.categoryBits = b;
            f.SetFilterData(fd);
        }
    },

    /*
     * Function: setMaskBits
     *
     * Description:
     *
     * Sets the Box2D mask bits
     *
     * You can have up to 16 categories for your physics objects.
     * With the mask bits you specify with which category to collide
     * with, whereas 1 means collide, and 0 means ignore collision.
     *
     * Parameters:
     *
     * b - a bitfield which categories to collide with
     *
     * See:
     *
     * <gamvas.actor.setCategoryBits>
     */
    setMaskBits: function(b) {
        var fd = null;
        for (f = this.body.GetFixtureList(); f; f = f.m_next) {
            fd = f.GetFilterData();
            fd.maskBits = b;
            f.SetFilterData(fd);
        }
    },

    getBox2DVectorFromValue: function(t, v) {
        var tpv = null;
        if (v instanceof Box2D.Common.Math.b2Vec2) {
            tpv = v;
        } else if (v instanceof gamvas.Vector2D) {
            tpv = new Box2D.Common.Math.b2Vec2(v.x, v.y);
        } else {
            tpv = t.body.GetWorldCenter();
        }
        return tpv;
    },

    /*
     * Function: addRevoluteJoint
     *
     * Description:
     *
     * Add a Box2D revolute joint between this actor and another
     *
     * Revolute joints are hinge like joints, you can limit their
     * angle and use them as motors.
     *
     * Parameters:
     *
     * t - the target actor
     * tp - the target position, either as b2Vec2D or <gamvas.Vector2D> (optional)
     * params - a list of joint parameters, see below (optional)
     * 
     * Joint Parameters:
     *
     * For detailed description see Box2D manual on http://box2d.org/manual.html
     *
     * lowerAngle - angle in radians
     * upperAngle - angle in radians
     * enableLimit - true/false
     * maxMotorTorque - maximum torque value
     * motorSpeed - current motor speed
     * enableMotor - true/false
     */
    addRevoluteJoint: function(t, tp, params) {
        var jd = new Box2D.Dynamics.Joints.b2RevoluteJointDef();
        var tpv = this.getBox2DVectorFromValue(t, tp);
        jd.Initialize(this.body, t.body, tpv);
        if (gamvas.isSet(params)) {
            for (var i in params) {
                switch (i) {
                case 'upperAngle':
                    jd.upperAngle = params[i];
                    break;
                case 'lowerAngle':
                    jd.lowerAngle = params[i];
                    break;
                case 'enableLimit':
                    jd.enableLimit = params[i];
                    break;
                case 'maxMotorTorque':
                    jd.maxMotorTorque = params[i];
                    break;
                case 'motorSpeed':
                    jd.motorSpeed = params[i];
                    break;
                case 'enableMotor':
                    jd.enableMotor = params[i];
                    break;
                default:
                    break;
                }
            }
        }
        return new gamvas.physics.getWorld().CreateJoint(jd);
    },

    /*
     * Function: addPrismaticJoint
     *
     * Description:
     *
     * Add a Box2D prismatic joint between this actor and another
     *
     * Prismatic joints are joints movable along a axis, similar
     * to springs
     *
     * Parameters:
     *
     * t - the target actor
     * tp - the target position, either as b2Vec2D or <gamvas.Vector2D> (optional)
     * tv - the target vector, aka the direction of the joint, either as b2Vec2D or <gamvas.Vector2D> (optional)
     * params - a list of joint parameters, see below (optional)
     * 
     * Joint Parameters:
     *
     * For detailed description see Box2D manual on http://box2d.org/manual.html
     *
     * upperTranslation - the upper translation limit
     * lowerTranslation - the lower translation limit
     * enableLimit - true/false
     * maxMotorForce - the maximum motor force
     * motorSpeed - the current motor speed
     * enableMotor - true/false
     *
     * Note:
     *
     * upperTranslation and lowerTranslation should allow the value zero
     * between them, because the joint will start with zero. For example:
     * upperTranslation = -0.2 lowerTranslation = 0.3
     */
    addPrismaticJoint: function(t, tp, tv, params) {
        var jd = new Box2D.Dynamics.Joints.b2PrismaticJointDef();
        var tpv = this.getBox2DVectorFromValue(t, tp);
        var tvv = this.getBox2DVectorFromValue(t, tv);
        jd.Initialize(this.body, t.body, tpv, tvv);
        if (gamvas.isSet(params)) {
            for (var i in params) {
                switch (i) {
                case 'upperTranslation':
                    jd.upperTranslation = params[i];
                    break;
                case 'lowerTranslation':
                    jd.lowerTranslation = params[i];
                    break;
                case 'enableLimit':
                    jd.enableLimit = params[i];
                    break;
                case 'maxMotorForce':
                    jd.maxMotorForce = params[i];
                    break;
                case 'motorSpeed':
                    jd.motorSpeed = params[i];
                    break;
                case 'enableMotor':
                    jd.enableMotor = params[i];
                    break;
                default:
                    break;
                }
            }
        }
        return new gamvas.physics.getWorld().CreateJoint(jd);
    }
});
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
gamvas.Particle = function(emitter, lifetime) {
    this.e = emitter;
    this.lt = lifetime;

    this.anim = null;
    this.position = new gamvas.Vector2D();
    this.velocity = new gamvas.Vector2D();
    this.life = 0;
    this.rotation = 0;
    this.scale = 1;
    this.rotVel = 0;
    this.dmp = 0;
    this.rotDmp = 0;

    this.update = function(t, gravx, gravy) {
        if (this.rotVel > 0) {
            this.rotVel -= this.rotDmp*t;
            if (this.rotVel <= 0) {
                this.rotDemp = 0;
                this.rotVel = 0;
            }
        } else if (this.rotVel < 0) {
            this.rotVel += this.rotDmp*t;
            if (this.rotVel >= 0) {
                this.rotDemp = 0;
                this.rotVel = 0;
            }
        }
        this.rotation += this.rotVel*t;

        if ( (this.dmp > 0) && (this.velocity.quickLength() > 0) ) {
            this.velocity.x *= 1.0-this.dmp*t;
            if (this.velocity.x === 0) {
                this.velocity.x = 0;
                this.velocity.y = 0;
            } else {
                this.velocity.y *= 1.0-this.dmp*t;
            }
        }
        this.velocity.x += this.e.gravity.x*t;
        this.velocity.y += this.e.gravity.y*t;
        this.position.x += this.velocity.x*t;
        this.position.y += this.velocity.y*t;

        if (this.anim !== null) {
            this.anim.setPosition(this.position.x, this.position.y);
        }
    };

    this.getLife = function() {
        return this.life/this.lt;
    };
};
/**
 * Copyright (C) 2012 Heiko Irrgang <hi@93i.de>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Class: gamvas.ParticleEmitter
 *
 * Description:
 *
 * A particle emitter
 *
 * The particle emitter allows you to achive many special
 * effects like smoke, fire, rain and similar.
 * It can emitt images or animations.
 *
 * The emitter extends the <gamvas.Actor> class and can
 * therefor be added with <gamvas.State.addActor> for
 * automatic drawing
 *
 * Contructor:
 *
 * new gamvas.ParticleEmitter(name, x, y, img, anim);
 *
 * Parameters:
 *
 * name - a unique name
 * x/y - the position of the emitter
 * img - a <gamvas.Image> or <gamvas.Animation> instance
 * anim - true/false, if img is a <gamvas.Animation> instead of <gamvas.Image>
 *
 * Note:
 *
 * The emitter provides a number of function named set{value} with a corresponding
 * set{value}Range function. The range specifies a range of possible values
 * around the non range value. This allows you to bring variance in the particle
 * emission and therefor realism.
 *
 * For example, you could use emitter.setParticleSpeed(150); with
 * emitter.setParticleSpeedRange(50); This would end up with the particles being
 * generated with speeds between 125 to 175 pixels per second.
 *
 * Example:
 *
 * > myEmitter = gamvas.ParticleEmitter.extend({
 * >     onParticleEnd: function(pos, rot, scale, vel) {
 * >         concole.log('rest in peace, particle at '+pos.x+','+pos.y);
 * >     }
 * > });
 * >
 * > myState = gamvas.State.extend({
 * >    init: function() {
 * >       var em = new myEmitter('smoke', 0, 0, new gamvas.Image(this.resource.getImage('smoke.png')));
 * >       em.setParticleRate(20);
 * >       em.setRotationRange(Math.PI*0.1); 
 * >       this.addActor(em);
 * >    },
 * > });
 */
gamvas.ParticleEmitter = gamvas.Actor.extend({
    create: function(name, x, y, img, anim) {
        this._super(name, x, y);
        this._emit = 0;
        this.limit = 0;
        /*
         * Variable: name
         *
         * The name of the particle emitter
         */
        /*
         * Variable: lifetime
         *
         * The time the emitter is running
         */
        this.lifeTime = 0;
        /*
         * Variable: emitted
         *
         * The amount of emitted particles
         */
        this.emitted = 0;
        this.isAnim = (anim) ? anim : false;
        this.animLifeTime = true;
        this.f = (img) ? img : null;
        this.particles = [];
        this.partRate = 10;
        this.partRateRange = 0;
        this.rotRange = Math.PI*0.002;
        this.partAlign = false;
        this.partRot = 0;
        this.partRotRange = 0;
        this.partRotVel = 0;
        this.partRotVelRange = 0;
        this.partScale = 1.0;
        this.partScaleRange = 0;
        this.partSpeed = 20;
        this.partSpeedRange = 0;
        this.partLifeTime = 5;
        this.partLifeTimeRange = 0;
        this.partDamp = 0;
        this.partDampRange = 0;
        this.partRotDamp = 0;
        this.partRotDampRange = 0;
        this.positionRange = new gamvas.Vector2D();
        this.gravity = new gamvas.Vector2D();
        this.st = [ [0.0, 1.0], [1.0, 1.0] ];
        this.spstx = [ [0.0, 1.0], [1000.0, 1.0] ];
        this.spsty = [ [0.0, 1.0], [1000.0, 1.0] ];
        this.at = [ [0.0, 1.0], [1.0, 1.0] ];
        /*
         * Variable: position
         *
         * A <gamvas.Vector2D> object with the position information
         *
         * See:
         *
         * <gamvas.ParticleEmitter.setPosition>
         * <gamvas.ParticleEmitter.move>
         */
        /*
         * Variable: center
         *
         * A <gamvas.Vector2D> object with the center offset of the particle emitter
         *
         * See:
         *
         * <gamvas.ParticleEmitter.setCenter>
         */
        /*
         * Variable: rotation
         *
         * The rotation of the particle emitter
         *
         * See:
         *
         * <gamvas.ParticleEmitter.setRotation>
         * <gamvas.ParticleEmitter.rotate>
         */
        /*
         * Variable: scaleFactor
         *
         * The scale factor of the object
         */
    },

    /*
     * Function: setImage
     *
     * Description:
     *
     * Set a <gamvas.Image> as Particle
     */
    setImage: function(img) {
        this.isAnim = false;
        this.f = img;
    },

    /*
     * Function: setAnimation
     *
     * Description:
     *
     * Set a <gamvas.Animation> as Particle
     */
    setAnimation: function(anim) {
        this.isAnim = true;
        this.f = anim;
    },

    /*
     * Function: setAnimationLifeTime
     *
     * Description:
     *
     * Set if the animation should be played once for the lifetime
     *
     * If set to true, the Animation plays once from start to end
     * until the particle dies, otherwise the animation plays with
     * whatever it has set as FPS
     *
     * Paramters:
     *
     * yesno - true/false, true = play once over lifetime, false = play as specified by FPS
     */
    setAnimationLifeTime: function(yesno) {
        this.animLifeTime = yesno;
    },

    /*
     * Function: setRotation
     *
     * Description:
     *
     * Set the rotation of the nozzle
     *
     * Parameters:
     *
     * r - the rotation in radians
     */
    /*
     * Function: setRotationRange
     *
     * Description:
     *
     * Set the angle around rotation how particles are emitted from the nozzle
     *
     * Parameters:
     *
     * r - the range around the center (half plus, half minus)
     */
    setRotationRange: function(r) {
        this.rotRange = r;
    },

    /*
     * Function: setParticleRate
     *
     * Description:
     *
     * Set the rate the particles are emitted
     *
     * Parameters:
     *
     * r - the rate in particles per second
     */
    setParticleRate: function(r) {
        this.partRate = r;
    },

    /*
     * Function: getParticleRate
     *
     * Description:
     *
     * Get the current particle emission rate
     */
    getParticleRate: function() {
        return this.partRate;
    },

    /*
     * Function: setParticleRateRange
     *
     * Description:
     *
     * Set the rate range particles are emitted
     *
     * Parameters:
     *
     * r - the range around the center (half plus, half minus)
     */
    setParticleRateRange: function(r) {
        this.partRateRange = r;
    },

    /*
     * Function: setParticleLimit
     *
     * Description:
     *
     * Set the maximum number of particles that will be emitted
     *
     * Parameters:
     *
     * l - the particle limit
     */
    setParticleLimit: function(l) {
        this.limit = l;
    },

    /*
     * Function: alignParticleToPath
     *
     * Description:
     *
     * If true, align particle orientation along their movement
     *
     * Parameters:
     *
     * yesno - true/false if to set the particle orientation along its path
     */
    alignParticleToPath: function(yesno) {
        this.partAlign = yesno;
    },

    /*
     * Function: setParticleRotation
     *
     * Description:
     *
     * Set the particles starting rotation
     *
     * Parameters:
     *
     * r - the rotation in radians
     */
    setParticleRotation: function(r) {
        this.partRot = r;
    },

    /*
     * Function: setParticleRotationRange
     *
     * Description:
     *
     * Set the range around particles starting rotation
     *
     * Parameters:
     *
     * r - the range of variance around the particles rotation
     */
    setParticleRotationRange: function(r) {
        this.partRotRange = r;
    },

    /*
     * Function: setParticleRotationVelocity
     *
     * Description:
     *
     * The rotational velocity of created particles
     *
     * Defines how much they continue to rotate while moving
     *
     * Parameters:
     *
     * r - the rotational velocity in radians per second
     */
    setParticleRotationVelocity: function(r) {
        this.partRotVel = r;
    },

    /*
     * Function: setParticleRotationVelocityRange
     *
     * Description:
     *
     * The range of the rotational velocity
     *
     * Parameters:
     *
     * r - the range around the rotational velocity in radians per second
     */
    setParticleRotationVelocityRange: function(r) {
        this.partRotVelRange = r;
    },

    /*
     * Function: setParticleScale
     *
     * Description:
     *
     * Set the scale of newly created particles
     *
     * Parameters:
     *
     * s - the scale factor (1 = original, < 1 = smaller, > 1 = bigger)
     */
    setParticleScale: function(s) {
        this.partScale = s;
    },

    /*
     * Function: setParticleScaleRange
     *
     * Description:
     *
     * Set the range of scale
     *
     * Parameters:
     *
     * s - the range around the scale factor
     */
    setParticleScaleRange: function(s) {
        this.partScaleRange = s;
    },

    /*
     * Function: setParticleSpeed
     *
     * Description:
     *
     * Set the initial speed of created particles
     *
     * Parameters:
     *
     * s - the speed in pixels per second
     */
    setParticleSpeed: function(s) {
        this.partSpeed = s;
    },

    /*
     * Function: setParticleSpeedRange
     *
     * Description:
     *
     * Set the range around the initial speed of created particles
     *
     * Parameters:
     *
     * s - the range around the speed in pixels per second
     */
    setParticleSpeedRange: function(s) {
        this.partSpeedRange = s;
    },
 
    /*
     * Function: setParticleLifeTime
     *
     * Description:
     *
     * Set the lifetime of new particles
     *
     * Parameters:
     *
     * l - the lifetime in seconds
     */
    setParticleLifeTime: function(l) {
        this.partLifeTime = l;
    },

    /*
     * Function: setParticleLifeTimeRange
     *
     * Description:
     *
     * Set the range around the lifetime of new particles
     *
     * Parameters:
     *
     * l - the range around the lifetime in seconds
     */
    setParticleLifeTimeRange: function(l) {
        this.partLifeTimeRange = l;
    },

    /*
     * Function: setParticleVelocityDamping
     *
     * Description:
     *
     * Set how much the velocity is slowed down over time
     *
     * Parameters:
     *
     * v - the damping value, between 0 (none) and 1 (immediate stop)
     */
    setParticleVelocityDamping: function(v) {
        this.partDamp = v;
    },

    /*
     * Function: setParticleVelocityDampingRange
     *
     * Description:
     *
     * Set the range around velocity damping
     *
     * Parameters:
     *
     * v - the velocity damping range
     */
    setParticleVelocityDampingRange: function(v) {
        this.partDampRange = v;
    },

    /*
     * Function: setParticleRotationDamping
     *
     * Description:
     *
     * Set how much the rotation is slowed down over time
     *
     * Parameters:
     *
     * r - the damping value, between 0 (none) and 1 (immediate stop)
     */
    setParticleRotationDamping: function(r) {
        this.partRotDamp = r;
    },

    /*
     * Function: setParticleRotationDampingRange
     *
     * Description:
     *
     * Set range around the rotation damping
     *
     * Parameters:
     *
     * r - the range around the rotation damping
     */
    setParticleRotationDampingRange: function(r) {
        this.partRotDampRange = r;
    },

    /*
     * Function: setParticleStartPositionRange
     *
     * Description:
     *
     * Sets the area where particles are created
     *
     * By default every particle is created exactly at the position of
     * the particle emitter, by setting a start position range, you
     * can define a rectangle where the particles are created
     *
     * Parameters:
     *
     * s - the starting position range as <gamvas.Vector2D>
     */
    setParticleStartPositionRange: function(s) {
        this.positionRange = s;
    },

    /*
     * Function: setGravity
     *
     * Description:
     *
     * Set the gravity that affects the particle emitter
     *
     * Parameters:
     *
     * s - the gravity in pixels per second as <gamvas.Vector2D>
     *
     * Note:
     *
     * Gravity is in pixels per second, other then in real life, if
     * you want your particles to fall down, you specify a positive
     * Y value
     */
    setGravity: function(g) {
        this.gravity = g;
    },

    /*
     * Function: setScaleTable
     *
     * Description:
     *
     * Set the scale over lifetime
     *
     * Parameters:
     *
     * scaleTable - a array with scale factors over the particles lifetime
     *
     * The array has to be made the following way:
     *
     * It has a index (first value) between 0 and 1 where 0 is when the particle is created
     * and 1 is when it is destroyed. You can add values between them feely,
     * but you have to ensure, that there is a 0 and 1 index.
     *
     * The following would be a table that scales from 0 to original size
     * at the half, and then back to 0 at end:
     *
     * > [ [0, 0], [0.5, 1], [1, 0] ]
     *
     * See:
     *
     * <gamvas.ParticleEmitter.setSpeedScaleTable>
     */
    setScaleTable: function(scaleTable) {
        this.st = scaleTable;
    },

    /*
     * Function: setAlphaTable
     *
     * Description:
     *
     * Set the alpha (transparency) over lifetime
     *
     * Parameters:
     *
     * alphaTable - a array with alpha values over the particles lifetime
     *
     * The array has to be made the following way:
     *
     * It has a index (first value) between 0 and 1 where 0 is when the particle is created
     * and 1 is when it is destroyed. You can add values between them feely,
     * but you have to ensure, that there is a 0 and 1 index.
     *
     * The following would be a table that alpha from 0 to original size, then
     * quite early it gets full opacity, and then fades out slowly to the end:
     *
     * > [ [0, 0], [0.1, 1], [1, 0] ]
     */
    setAlphaTable: function(alphaTable) {
        this.at = alphaTable;
    },

    /*
     * Function: setSpeedScaleTable
     *
     * Description:
     *
     * Set the scale depending on particle speed
     *
     * Parameters:
     *
     * spdScaleTable - a array with scale factors for x/y depending on particle speed
     *
     * The array has to be made the following way:
     *
     * It has a index (first value) which is the speed of the particle in pixels per second,
     * you should have at least a speed of 0 and some speed bigger then 0
     *
     * Following the index, there are two values representing the x and y scale factor where
     * 1 is original size, smaller then 1 is smaller and higher then 1 is bigger
     *
     * The following would be a table that scales the particle very small when it is slow, then
     * when moving at medium speed, it scales it to original size, wen moving fast it streches
     * it by reducing the x scale and significantly increasing the y scale.
     *
     * > [ [0, 0.1, 0.1], [30, 1, 1], [100, 0.2, 3] ]
     *
     * Note:
     *
     * This is usually used with <gamvas.ParticleEmitter.alignParticleToPath>, as x and y are regarding to the particles original orientation
     *
     * See:
     *
     * <gamvas.ParticleEmitter.setScaleTable>
     */
    setSpeedScaleTable: function(spdScaleTable) {
        this.spstx = [];
        this.spsty = [];
        for (var i in spdScaleTable) {
            this.spstx.push([spdScaleTable[i][0], spdScaleTable[i][1]]);
            this.spsty.push([spdScaleTable[i][0], spdScaleTable[i][2]]);
        }
    },

    /*
     * Function: draw
     *
     * Description:
     *
     * Draw the particle emitter
     *
     * Paramters:
     *
     * t - the time since last redraw in seconds
     *
     * Note:
     *
     * Particle emitter extends <gamvas.Actor> and therefor can be added
     * via <gamvas.State.addActor> for automatic drawing
     */
    draw: function(t) {
    	this.lifeTime+=t;
        if ( (this.limit === 0) || (this.emitted < this.limit) ) {
            this._emit += t*this.getRandomValue(this.partRate, this.partRateRange);
            if (this._emit > 1) {
                var e = Math.floor(this._emit);
                this._emit -= e;
                for (var i = 0; i < e; i++) {
                    var newp = new gamvas.Particle(this, this.getRandomValue(this.partLifeTime, this.partLifeTimeRange));
                    if (this.isAnim) {
                        newp.anim = new gamvas.Animation(this.name+this.emitted, this.f.sprite, this.f.width, this.f.height, this.f.numberOfFrames);
                        newp.anim.setCenter(this.f.center.x, this.f.center.y);
                        newp.anim.fDur = this.f.fDur;
                    }
                    newp.rotation = this.getRandomValue(this.partRot, this.partRotRange);
                    newp.rotVel = this.getRandomValue(this.partRotVel, this.partRotVelRange);
                    newp.dmp = this.getRandomValue(this.partDamp, this.partDampRange);
                    newp.scale = this.getRandomValue(this.partScale, this.partScaleRange);
                    newp.rotDmp = this.getRandomValue(this.partRotDamp, this.partRotDampRange);
                    newp.position.x = this.getRandomValue(this.position.x, this.positionRange.x);
                    newp.position.y = this.getRandomValue(this.position.y, this.positionRange.y);
                    var r = new gamvas.Vector2D(0, this.getRandomValue(this.partSpeed, this.partSpeedRange));
                    newp.velocity = r.rotate(this.getRandomValue(this.rotation, this.rotRange));
                    this.particles.push(newp);
                    if (this.limit > 0) {
                        this.emitted++;
                        if (this.emitted >= this.limit) {
                            break;
                        }
                    }
                }
            }
        }
        var newparts = [];
        for (var pi = 0; pi < this.particles.length; pi++) {
            var p = this.particles[pi];
            p.life += t;
            if (p.life <= p.lt) {
                p.update(t);
                if (!this.isAnim) {
                    this.f.setPosition(p.position.x, p.position.y);
                }
                var sv = this.getTableValue(this.st, p.getLife());
                if (this.partAlign) {
                    var l = p.velocity.length();
                    var nsx = this.getTableValue(this.spstx, l);
                    var nsy = this.getTableValue(this.spsty, l);
                    if (this.isAnim) {
                        p.anim.setScaleXY(nsx*sv, nsy*sv);
                        p.anim.setRotation(Math.atan2(p.velocity.y, p.velocity.x)+0.5*Math.PI);
                    } else {
                        this.f.setScaleXY(nsx*sv, nsy*sv);
                        this.f.setRotation(Math.atan2(p.velocity.y, p.velocity.x)+0.5*Math.PI);
                    }
                } else {
                    if (this.isAnim) {
                        p.anim.setScale(p.scale*sv);
                        p.anim.setRotation(p.rotation);
                    } else {
                        this.f.setScale(p.scale*sv);
                        this.f.setRotation(p.rotation);
                    }
                }
                this.f.c.globalAlpha = this.getTableValue(this.at, p.getLife());
                if (this.isAnim) {
                    if (this.animLifeTime) {
                        var rest = p.lt-p.life;
                        var restfr = p.anim.numberOfFrames*(1-(p.life/p.lt));
                        p.anim.fDur = rest/restfr;
                        p.anim.draw(t);
                    } else {
                        p.anim.draw(t);
                    }
                } else {
                    this.f.draw(t);
                }

                newparts.push(p);
            } else {
		this.onParticleEnd(p.position, p.rotation, p.scale, p.velocity);
                delete p;
            }
        }
        this.particles = newparts;
        this.f.c.globalAlpha = 1.0;
    },

    getRandomValue: function(base, range) {
        return base+((Math.random()-0.5)*2)*range;
    },

    getTableValue: function(tbl, pos) {
        var last = 0;
        var current = 0;
        var fnd = false;
        for (var i in tbl) {
            if (tbl[i][0] < pos) {
                last = i;
            } else {
                fnd = true;
                current = i;
                break;
            }
        }

        // value too high, get highest
        if (tbl[tbl.length-1][0]<=pos) {
            return tbl[tbl.length-1][1];
        }

        // value not found at all
        if (!fnd) {
            return 1.0;
        }
        var offs = pos-tbl[last][0];
        var rng = tbl[current][0]-tbl[last][0];
        var fct = offs/rng;
        if (tbl[current][1]>tbl[last][1]) {
            var amt = tbl[current][1]-tbl[last][1];
            return tbl[last][1]+amt*fct;
        }
        var amt2 = tbl[last][1]-tbl[current][1];
        return tbl[current][1]+amt2*(1.0-fct);
    },

    /*
     * Function: reset
     *
     * Description:
     *
     * Reset the particle emitter
     *
     * Parameters:
     *
     * kill - true/false if old particles should be destroyed (optional)
     *
     */
    reset: function(kill) {
        this._emit = 0;
        this.lifeTime = 0;
        this.emitted = 0;
        if ( (gamvas.isSet(kill)) && (kill) ) {
            this.particles = [];
        }
    },

    /*
     * Function: onParticleEnd
     *
     * Description:
     *
     * Overwrite this function to do something everytime a particle is removed
     *
     * Parameters:
     *
     * pos - the position of the destroyed particle as <gamvas.Vector2D>
     * rot - the rotation in radians
     * scale - the scale of the particle
     * vel - the velocity of the particle as <gamvas.Vector2D>
     */
    onParticleEnd: function(pos, rot, scale, vel) {
    }

    /*
     * Function: setRotation
     *
     * Description:
     *
     * Set certain rotation of the particle emitter in radians
     *
     * Parameters:
     *
     * r - the rotation in radians
     *
     * See:
     *
     * <gamvas.ParticleEmitter.rotate>
     * http://en.wikipedia.org/wiki/Radians
     */
    /*
     * Function: rotate
     *
     * Description:
     *
     * Rotate the particle emitter
     *
     * Parameters:
     *
     * r - the amount to rotate the particle emitter in radians
     *
     * See:
     *
     * <gamvas.ParticleEmitter.setRotation>
     * http://en.wikipedia.org/wiki/Radians
     */
    /*
     * Function: setPosition
     *
     * Description:
     *
     * Set the position of a particle emitter
     *
     * Parameters:
     *
     * x/y - the position of the particle emitter in pixels
     *
     * See:
     *
     * <gamvas.ParticleEmitter.move>
     */
    /*
     * Function: move
     *
     * Description:
     *
     * Move the particle emitter
     *
     * Parameters:
     *
     * x/y - the pixels to move the particle emitter
     *
     * See:
     *
     * <gamvas.ParticleEmitter.setPosition>
     */
    /*
     * Function: setScale
     *
     * Description:
     *
     * Set a certain scale factor
     *
     * Parameters:
     *
     * s - the scale value (1 = no scale, < 1 = smaller, > 1 = bigger)
     *
     * See:
     *
     * <gamvas.ParticleEmitter.scale>
     */
    /*
     * Function: scale
     *
     * Description:
     *
     * Scale a object
     *
     * Note:
     *
     * Parameters:
     *
     * s - the scale factor (< 0 = shrink, > 0 = enlarge)
     *
     * See:
     *
     * <gamvas.ParticleEmitter.setScale>
     */
});
