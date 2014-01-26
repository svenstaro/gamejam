part of game;

class Pulse extends Sprite {
    static const int WATER  = 0x01;
    static const int ENERGY = 0x02;

    Branch currentBranch;
    num currentBranchPosition = 0;

    int type = WATER;

    Pulse(this.type, this.currentBranch) {
        var radius = 0.05;
        graphics.circle(0, 0, radius);
        var g = new GraphicsGradient.radial(0, 0, 0, 0, 0, radius);
        if(type == WATER) {
            g.addColorStop(0, 0xAA0000FF);
            g.addColorStop(1, 0x000000FF);
        } else {
            g.addColorStop(0, 0xAA00FF00);
            g.addColorStop(1, 0x0000FF00);
        }
        graphics.fillGradient(g);
        alpha = 0;

        var s = new Shape();
        s.graphics.circle(0, 0, 0.01);
        if(type == WATER) {
            s.graphics.fillColor(0xFFAAAAFF);
        } else {
            s.graphics.fillColor(0xFFAAFFAA);
        }
        addChild(s);

        this.onEnterFrame.listen((e) {
            if(currentBranch != null) {
                alpha = (alpha + e.passedTime).clamp(0, 1);

                currentBranchPosition += e.passedTime / currentBranch.length * 0.5; // speed
                while(currentBranchPosition > 1) {
                    if(currentBranch.isEndBranch && movingToTip) {
                        parent.removeChild(this);
                        this.onEnterFrame.cancelSubscriptions();
                        currentBranchPosition = 0;
                    } else {
                        currentBranchPosition -= 1;
                        currentBranch = nextBranch;
                    }
                }

                // Vector s = lerp(currentBranch.basePosition, currentBranch.tipPosition, currentBranchPosition);
                var s = currentBranch.basePosition + (currentBranch.tipPosition - currentBranch.basePosition).scale(
                    movingToTip ? 1 - currentBranchPosition : currentBranchPosition);
                // var s = new Vector(0, 0);
                x = s.x;
                y = s.y;
            } else {
                parent.removeChild(this);
                this.onEnterFrame.cancelSubscriptions();
            }
        });
    }

    bool get movingToTip => (type == WATER) != (currentBranch.isRoot);

    Branch get nextBranch {
        if(movingToTip) {
            var b = currentBranch.branches;
            var r = random.nextDouble();
            var akku = 0;
            for(var branch in currentBranch.branches) {
                if(r > akku && r < akku + branch.valve) {
                    return branch;
                }
                akku += branch.valve;
            }
            // return b[random.nextInt(b.length)];
        } else {
            return !currentBranch.isBase ? currentBranch.parent : treeBase;
        }
    }
}
