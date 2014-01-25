part of game;

class HumanEvent extends DisplayObjectContainer {
    
    num _secondsPerStep = 0.84;
    num _stepLength = 0.4;
    num _totalTime = 0;
    num _time = 0;
    num _wayToGo = 4;
    
    HumanFootstep _foot1;
    HumanFootstep _foot2;
    HumanFootstep _nextFoot;
    
    HumanEvent() {
        
        this.transformationMatrix.translate(-_wayToGo/2, 0);
        this.onEnterFrame.listen(_onEnterFrame);
        
        _foot1 = new HumanFootstep();
        _foot2 = new HumanFootstep();
        
        this.addChild(_foot1);
        this.addChild(_foot2);
        
        _nextFoot = _foot1;
        _foot2.transformationMatrix.translate(_stepLength / 2, 0);
    }
    
    _onEnterFrame(EnterFrameEvent e) {
        _totalTime += e.passedTime;
        _time += e.passedTime;
        
        var progress = _time / _secondsPerStep;
        var otherFoot = _nextFoot == _foot1 ? _foot2 : _foot1;

        var fade = 5;
        
        _nextFoot.alpha = ((1-progress) * fade).clamp(0, 1);
        otherFoot.alpha = (progress * fade).clamp(0, 1);
        
        var wayGone = _totalTime / _secondsPerStep * _stepLength / 2;
        this.alpha = sinwave(wayGone / _wayToGo);
        
        if(wayGone > _wayToGo) {
            this.removeFromParent();
            this.onEnterFrame.cancelSubscriptions();
        }
                
        while(_time > _secondsPerStep) {
            _nextFoot.transformationMatrix.translate(_stepLength, 0);
            _nextFoot = otherFoot;
            _time -= _secondsPerStep;
        }
    }
}

class HumanFootstep extends Sprite {
    HumanFootstep() {
        this.transformationMatrix.scale(1, 0.6);
        
        this.graphics.rect(-0.25, 0, 0.5, 0.25);
        
        var gradient = new GraphicsGradient.radial(0, 0, 0, 0, 0, 0.15);
        gradient.addColorStop(0, 0xFFDAADAD);
        gradient.addColorStop(1, 0x00DAADAD);
        this.graphics.fillGradient(gradient);
    }
}
