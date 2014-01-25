part of game;

void debugTree(int depth, var parent) {
    int count = 2;
    for(int i = 0; i < count; ++i) {
        Branch b = new Branch();
        b.baseRotation = (i*1.0/(count-1) - 0.5) * (depth+2) / 5;
        b.thickness = depth / 15;
        parent.addChild(b);
        if(depth > 0) {
            debugTree(depth - 1, b);
        }
    }
}

