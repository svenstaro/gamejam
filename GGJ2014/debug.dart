part of game;

void debugTree(int depth, var parent) {
    int count = random.nextInt(4) == 0 ? 1 : 2;
    for(int i = 0; i < count; ++i) {
        Branch b = new Branch();
        if(count > 1)
            b.baseRotation = (i*1.0/(count-1) - 0.5) * (depth+2) / 5;
        b.baseRotation += (random.nextDouble() - 0.5) * 0.5;
        b.thickness = depth / 15;
        parent.addChild(b);
        if(depth > 0) {
            if(random.nextInt(10) > 0) {
                debugTree(depth - 1, b);
            }
        }
    }
}

