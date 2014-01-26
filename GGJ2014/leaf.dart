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
        scaleX = 0.6;
        scaleY = 0.6;
        baseRotation = random.nextDouble() * 2 - 1;

        onEnterFrame.listen((EnterFrameEvent e) {
            this.rotation = lerp(baseRotation, PI * .5, Wind.power * 0.02);
        });
        //     graphics.clear();
        //     if(!debug) {

                graphics.beginPath();
                graphics.moveTo(0, 0);
                graphics.bezierCurveTo(-0.3, -0.3, -0.3, -0.3, 0, -1);
                graphics.bezierCurveTo( 0.3, -0.3,  0.3, -0.3, 0,  0);
                graphics.closePath();
                graphics.fillColor(0xAAFFFFFF);

                // graphics.fillColor(leafBranch.branch.branchColor);
        //     }
        // });
    }

}
