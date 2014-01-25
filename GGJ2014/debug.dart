part of game;

void debugTree(int depth, var parent) {
    int count = depth~/2 + 1;
    for(int i = 0; i < count; ++i) {
        Branch b = new Branch();
        b.baseRotation = (i*1.0/(count-1) - 0.5) * 1 * log(depth+1);
        parent.addChild(b);
        if(depth > 1) {
            debugTree(depth - 1, b);
        }
    }
}

