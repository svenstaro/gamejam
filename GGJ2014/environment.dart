part of game;

class Environment extends Animatable {
    
    static num _time = 0;
    static AwesomeColor sunColor1 = new AwesomeColor.fromHex(0xffffc48f); //sun at zenith
    static AwesomeColor sunColor2 = new AwesomeColor.fromHex(0xffb84e43); //sun at horizon
    
    static AwesomeColor moonColor1 = new AwesomeColor.fromHex(0xff082e4e); //moon at zenith
    static AwesomeColor moonColor2 = new AwesomeColor.fromHex(0xff031f37); //moon at horizon
    
    bool advanceTime(num time) {
        _time += time;
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