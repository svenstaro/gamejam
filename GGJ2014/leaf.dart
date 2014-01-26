part of game;

class LeafBranch extends Sprite {
    Branch branch;
    num length;

    LeafBranch(this.branch) {
        length = random.nextDouble() * 2;

        graphics.beginPath();
        graphics.moveTo(0, 0);
        graphics.lineTo(0, -length);
        graphics.closePath();
        graphics.strokeColor(0x88000000, 0.05);
        x = (random.nextDouble() * 2 - 1) * branch.thickness;
        x = 0;
        y = -random.nextDouble();
        scaleX = 0.3;
        scaleY = 0.3;
        rotation = PI/4 + (random.nextDouble() * 2 - 1) * 0.5;
        if(random.nextInt(2) == 0) {
            rotation *= -1;
        }

        for(int i = 0; i < length * 7; ++i) {
            var l = new Leaf(this);
            if(i == 0) l.y = -length;
            addChild(l);
        }
    }
}

class Leaf extends Sprite {
    LeafBranch leafBranch;
    num baseRotation;
    num rotationSpeed = 0;

    Leaf(this.leafBranch) {
        // graphics.fillColor(0xFF00CC00);
        //graphics.fillColor(0x44FFFFFF);
        //graphics.fillColor((new AwesomeColor.fromHex(0x44FFFFFF) * Environment.getLightColorFor(leafBranch.branch)).hex);
        x = 0;
        y = - random.nextDouble() * leafBranch.length;
        var scale = colorLeaves ? 1.6 : 0.6; 
        scaleX = scale;
        scaleY = scale;
        baseRotation = random.nextDouble() * 2 - 1;
        alpha = 0;
        
        drawLeaf(leafBranch);

        onEnterFrame.listen((EnterFrameEvent e) {
            this.rotation = lerp(baseRotation, PI * .5, Wind.power * 0.02);
            
            if(alpha < 1) {
                alpha = (alpha + e.passedTime * 0.4).clamp(0,1);
            }
            
            if(colorLeaves) drawLeaf(leafBranch);
        });
    }
     
    void drawLeaf(leafBranch) {
        graphics.clear();
            graphics.beginPath();
            graphics.moveTo(0, 0);
            graphics.bezierCurveTo(-0.3, -0.3, -0.3, -0.3, 0, -1);
            graphics.bezierCurveTo( 0.3, -0.3,  0.3, -0.3, 0,  0);
            graphics.closePath();
        
        if(colorLeaves) {
            graphics.fillColor(leafBranch.branch.branchColor);
        }
        else {
            graphics.fillColor(0xAAFFFFFF);
        }
     }

}
