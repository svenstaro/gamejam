part of game;

class Environment extends Animatable {
    
    static num _time = 0.0;
    static AwesomeColor sunColor1 = new AwesomeColor.fromHex(0xffffc48f); //sun at zenith
    static AwesomeColor sunColor2 = new AwesomeColor.fromHex(0xffb84e43); //sun at horizon
    
    static AwesomeColor moonColor1 = new AwesomeColor.fromHex(0xff082e4e); //moon at zenith
    static AwesomeColor moonColor2 = new AwesomeColor.fromHex(0xff031f37); //moon at horizon
    
    var _seed = 1337.22;
    var _rainGen;
    var _windGen;
    
    Environment() {
        _rainGen = makeOctave2(simplex2, 3, 0.01);
        _windGen = makeOctave2(simplex2, 3, 0.01);
    }
    
    bool advanceTime(num time) {
        _time += time;
        
        
        //Wind.windPower = (_windGen(_seed, time * 0.00002) * -2);
        Wind.secondsPerWave = _windGen(_seed, time * 0.2) * 220;
        //print(_windGen(_seed, time * 0.00002));
        
        amountOfRain = (_rainGen(_seed, _time * 0.01)).clamp(0,1) * 100;
    }
    
    static AwesomeColor getLightColorFor(Branch b) {
        var angle = 1 - (b.getTipPosition().rads + PI) / (2 * PI);
        
        //print(Clock.daytime);
        
        var light = sinwave(Clock.daytime * 2);
        var sunColor = AwesomeColor.lerpColor(sunColor1, sunColor2, light);
        var moonColor = AwesomeColor.lerpColor(moonColor1, moonColor2, light);
        
        var dayLight = sinwave(Clock.daytime - .5);
        sunColor = sunColor * dayLight;
        moonColor = moonColor * (1-dayLight);
        
        
        var sunAngle = Clock.daytime;
        var moonAngle = (Clock.daytime + .5) % 1;
        var sunPower = angle01(angle, sunAngle);
        var moonPower = angle01(angle, moonAngle);
        
        return (sunColor * sunPower) + (moonColor * moonPower);
    }
}