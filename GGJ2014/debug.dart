part of game;

void gameTree(var parent){
    int count = 2;
    for(int i = 0; i < count; ++i) {
        Branch b = new Branch(0);
        b.baseRotation = (i*1.0/(count-1) - 0.5);
        b.baseRotation += (random.nextDouble() - 0.5) * 0.5;
        b.length = random.nextDouble() * 0.5 + 0.5;
        b._valve = 0.5;
        parent.addChild(b);
        b.reset();
    }
}

void debugTree(int depth, var parent) {
    int count = random.nextInt(4) == 0 ? 1 : 2;
    for(int i = 0; i < count; ++i) {
        Branch b = new Branch(parent.thickness * 0.6);
        if(count > 1)
            b.baseRotation = (i*1.0/(count-1) - 0.5);
        b.baseRotation += (random.nextDouble() - 0.5) * 0.5;
        b.length = random.nextDouble() * 0.5 + 0.5;
        b._valve = 1/count;
        parent.addChild(b);
        if(depth < 4) {
            if(random.nextInt(10) > 0) {
                debugTree(depth + 1, b);
            }
        }
        b.reset();

        if(colorLeaves) {
            b.growLeaves((5 - b.depth) * 1);
        }
    }
}

void retardTree(int depth, var parent) {
    int count = 1;
    for(int i = 0; i < count; ++i) {
        Branch b = new Branch(parent.thickness * 0.6);
        if(count > 1)
            b.baseRotation = (i*1.0/(count-1) - 0.5);
        b.baseRotation += (random.nextDouble() - 0.5) * 0.5;
        b.length = random.nextDouble() * 0.5 + 0.5;
        parent.addChild(b);
        if(depth < 4) {
            if(random.nextInt(10) > 0) {
                retardTree(depth + 1, b);
            }
        }
        b.growLeaves((5 - b.depth) * 3);
        b.reset();
    }
}

void debugRoots(int depth, var parent) {
    int count = 2;
    if(depth == 0) count = 4;

    for(int i = 0; i < count; ++i) {
        Branch b = new Branch(parent.thickness * 0.3);

        num spread = 0.2;
        if(depth == 0) {
            spread = 2.4;
        }

        b.baseRotation = (1 - pow(random.nextDouble(), 2)) * randSign() * spread;
        b._valve = 1/count;

        if(count > 1)
            b.baseRotation = (i*1.0/(count-1) - 0.5) * spread;
        b.baseRotation += (random.nextDouble() - 0.5) * 0.5;

        b.length = random.nextDouble() * 0.3 + 0.3;
        parent.addChild(b);
        if(depth < 2) {
            debugRoots(depth + 1, b);
        }
        b.isRoot = true;
        b.reset();
    }
}

