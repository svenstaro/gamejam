part of game;

class Branch extends Sprite {
    bool isRoot = false;

    num _water = 0;
    num _old_water = 0;
    void set water(num water) {
        _old_water = _water;
        _water = water;
    }
    num get water => _water;
    num get waterDelta => max(0, _old_water - _water);
    num _waterCreated = 0;

    num _energy = 0;
    num _old_energy = 0;
    void set energy(num energy) {
        _old_energy = _energy;
        _energy = energy;
    }
    num get energy => _energy;
    num get energyDelta => max(0, _energy - _old_energy);
    num _energyCreated = 0;

    num _wither = 0;
    num _old_wither = 0;
    void set wither(num wither) {
        _old_wither = _wither;
        _wither = wither;
    }
    num get wither => _wither;
    num get witherDelta => max(0, _wither - _old_wither);

    bool deleteSoon = false;

    num baseRotation = 0.0;

    num _valve = 1;
    void set valve(num value) {
        num diff = value - _valve;
        _valve = value.clamp(0.0, 1.0);
        if(parent is Branch) {
            for(var b in (parent as Branch).branches) {
                if(!identical(b, this)) {
                    b._valve -= diff / ((parent as Branch).branches.length - 1);
                    b._valve = b._valve.clamp(0, 1);
                }
            }
        }
    }
    num get valve => _valve;

    bool isClicked = false;
    Vector dragStartPoint = null;

    int branchColor = 0;

    num get treeSize => length + (branches.length == 0 ? 0 : branches.fold(0, (v, b) => max(v, b.treeSize)));

    GlassPlate shape = null;
    Shape debugShape = null;
    Sprite hurtSprite = null;
    TextField branchText = new TextField();

    num veinAlpha = 0;

    num maxLength = 1;
    num _length = 1;
    void set length(num value) {
        _length = value;
        reset();
    }
    num get length => _length;

    num _thickness = 1;
    void set thickness(num value) {
        _thickness = value;
        reset();
    }
    num get thickness => _thickness;

    Branch(this._thickness) {
        water = parent != null ? (parent as Branch).water : thickness;
        energy = parent != null ? (parent as Branch).energy : thickness;
        maxLength = random.nextDouble() * 0.8 + 0.2;
        reset();

        onEnterFrame.listen(_onEnterFrame);

        branchText.defaultTextFormat = new TextFormat('monospace', 10, Color.White);
        branchText.scaleX = 0.01;
        branchText.scaleY = 0.01;
        branchText.y = -0.75;
        branchText.x = 0.02;
        branchText.text = "branchText";
        branchText.mouseEnabled = false;
        branchText.visible = false;
        this.mouseEnabled = false;
        addChild(branchText);

        hurtSprite = new Sprite();
        hurtSprite.graphics.circle(0, 0, 1);
        GraphicsGradient g = new GraphicsGradient.radial(0, 0, 0, 0, 0, 1);
        g.addColorStop(0, 0xFFFF0000);
        g.addColorStop(1, 0x00FF0000);
        hurtSprite.graphics.fillGradient(g);
        hurtSprite.mouseEnabled = false;
        hurtSprite.alpha = 0;
        addChild(hurtSprite);

        stage.onMouseOver.listen((e) {
            if(!isRoot) {
                branchText.visible = debug;
            }
            else {
                branchText.visible = false;
            }
        });
    }

    void reset() {
        // click shape
        if(shape != null) removeChild(shape);
        num shapeWidth = max(thickness, 0.05);
        shape = new GlassPlate(shapeWidth, length);
        shape.pivotX = shapeWidth/2;
        shape.pivotY = length;
        addChild(shape);


        // debug shape
        // if(debugShape != null) removeChild(debugShape);
        // debugShape = new Shape();
        // debugShape.graphics.rect(-shapeWidth/2, -length, shapeWidth, length);
        // debugShape.graphics.strokeColor(0xFF00FF00, 0.01);
        // addChild(debugShape);

        y = parent is Branch ? -(parent as Branch).length : 0;

        for(var b in branches) {
            b.reset();
        }
    }

    num get totalValve => parent is Branch ? (parent as Branch).totalValve * valve : valve;

    int get depth => parent is Branch ? (parent as Branch).depth + 1 : 0;

    bool get isBase => !(parent is Branch);

    bool get isEndBranch => branches.length == 0;

    List<Branch> get allEndBranches {
        List<Branch> result = new List<Branch>();
        for(var branch in branches) {
            if(branch.isEndBranch) {
                result.add(branch);
            } else {
                result.addAll(branch.allEndBranches);
            }
        }
        return result;
    }

    List<Branch> get branches {
        List<Branch> result = new List<Branch>();
        for(int i = 0; i < numChildren; i++) {
            if(getChildAt(i) is Branch) result.add(getChildAt(i));
        }
        return result;
    }

    List<LeafBranch> get leaves {
        List<LeafBranch> result = new List<LeafBranch>();
        for(int i = 0; i < numChildren; i++) {
            if(getChildAt(i) is LeafBranch) result.add(getChildAt(i));
        }
        return result;
    }

    void growLeaves([int num = 30]) {
        for(int i = 0; i < num; ++i) {
            addChild(new LeafBranch(this));
        }
    }

    void _onEnterFrame(EnterFrameEvent e) {
        if(isDead) return;

        e = new EnterFrameEvent(e.passedTime * 1);

        // Update gameplay values
        num energyFactor = 0.05;
        num energyToWater = 1;
        num thirstiness = 0.001;
        num witherFactor = 5;
        num energyConversionRate = 0.0001;
        num transferRate = 0.01;
        num transferFactor = 1;
        num growthRate = 0.05; // length per second at full water
        num waterGrowthConversion = 0.2;

        // THIS IS THE RESOURCE SIMULATION PART
        if(!relaxMode) {
            // Energy is generated by leaves when sun is alight
            // For now just give it a fix value
            if(isEndBranch) {
                energy = (energy + e.passedTime * energyFactor * depth / 3).clamp(0, 1);
                water -= e.passedTime * thirstiness;
                if(water < 0) {
                    wither -= water * witherFactor;
                    water = 0;
                }

                if(!isRoot && water > 0) {
                    num growth = growthRate * e.passedTime * water;
                    water -= growth * waterGrowthConversion;
                    length += growth;
                    // if(length > maxLength) {
                    //     length = maxLength;
                    //     growChild(-0.8 * random.nextDouble(), 0);
                    //     growChild( 0.8 * random.nextDouble(), 0);
                    //     if(!canDie && depth > 0) {
                    //         canDie = true;
                    //     }
                    // }
                }

                if(length > maxLength && !isRoot && leaves.length == 0) {
                    growLeaves(depth);
                }
            }

            // Generate water in root
            if(isBase && !isRoot) {
                num de = min(energy, energyConversionRate);
                energy -= de;
                water = (water + de * energyToWater).clamp(0, 1);
            }

            _energyCreated += energyDelta;
            _waterCreated += waterDelta;

            num pulse_threshold = 0.05;
            if(_energyCreated >= pulse_threshold && !isRoot) {
                view.addChild(new Pulse(Pulse.ENERGY, this));
                _energyCreated -= pulse_threshold;
            }
            if(_waterCreated >= pulse_threshold) {
                view.addChild(new Pulse(Pulse.WATER, this));
                _waterCreated -= pulse_threshold;
            }

            // Aging -> thickness grows
            num maxThickness = isBase ? 0.8 : (parent as Branch).thickness * 0.9 / (parent as Branch).branches.length;
            num thicknessGrowth = 0.003;
            if(!isRoot) {
                thickness += (maxThickness - thickness) * thicknessGrowth * e.passedTime * length;
            }

            for(var child in branches) {
                num de = child.energy * valve * e.passedTime * transferRate;
                energy = (energy + de).clamp(0, 1);
                child.energy -= de;

                num dw = (child.valve * e.passedTime * transferRate) / branches.length;
                dw = min(dw, water);
                dw = min(dw, 1 - child.water);
                water -= dw;
                child.water = (child.water + dw * transferFactor);
            }
        }

        // Update debug info
        branchText.textColor = Color.Cyan;
        branchText.text = "H20: ${water.toStringAsFixed(3)}";
        branchText.text += "\nNRG: ${energy.toStringAsFixed(3)}";
        branchText.text += "\nVAL: ${valve.toStringAsFixed(3)}";
        branchText.text += "\nWIT: ${wither.toStringAsFixed(3)}";
        if(!debug) { branchText.visible = false; }

        if(isRoot || isBase) {
            this.rotation = baseRotation;
        } else {
            this.rotation = baseRotation + Wind.power * 0.2 * length;
        }

        num st = startThickness;
        num et = thickness;

        this.graphics.clear();

        if(isBase) {
            Spline spline = new Spline();
            addPoints(spline, this);
            spline.generatePath(graphics);
            graphics.fillColor(0xFF000000);
            graphics.strokeColor(relaxMode ? 0xFF000000 : 0x55FFFFFF, 1/view.scaleX);
        } else if(isEndBranch) {
            Spline spline = new Spline();
            addVeinPoints(spline, this, null, 0);
            spline.generatePath(graphics);
            veinAlpha = relaxMode ? 0 : totalValve;
            // veinAlpha = lerp(veinAlpha, alpha, 1/alphaSpeed/e.passedTime).clamp(0, 1);
            graphics.strokeColor(new AwesomeColor(1, 1, 1, veinAlpha).hex, 1/view.scaleX);
        }

        if(colorLeaves) {
            branchColor = (new AwesomeColor.fromHex(0x55DDFFDD) * Environment.getLightColorFor(this)).hex;
        }

        if(wither >= 1.0) {
            deleteSoon = true;
        }

        for(var child in branches) {
            if(child.deleteSoon) {
                child.delete();
            }
        }

        if(isClicked) {
            valve += frameTime * 2;
            debugMessage = "Valve: " + valve.toString();
        }

        if(!isRoot && witherDelta>0) {
            hurtSprite.scaleX = thickness*4;
            hurtSprite.scaleY = length*0.7;
            hurtSprite.alpha = (witherDelta*5000).clamp(0, 1);
            hurtSprite.y = -length/2;
        }
    }

    void addPoints(Spline spline, Branch base) {
        num st = startThickness;
        num et = thickness;

        var branches = this.branches;

        num off = branches.length > 0 ? min(0.3 * thickness, branches.map((b) => b.length).reduce(max) * 0.5) : 0;

        num tangentLength = isEndBranch ? 0.0 : off * length;

        // going up on the left
        if(isBase) {
            spline.add(base.globalToLocal(localToGlobal(new Point(-st/2,  0))), 0.1);
        }
        spline.add(base.globalToLocal(localToGlobal(new Point(-et/2, -length))), tangentLength);

        // sort children
        sortChildren((var l, var r) {
            if(l is Branch && r is Branch) {
                return l.baseRotation.compareTo(r.baseRotation);
            } else {
                return 0;
            }
        });

        int numBranches = branches.length;
        int branchNumber = 0;
        num totalThickness = branches.length > 0 ? branches.fold(0, (prev, elem) => (prev is Branch ? prev.thickness : prev) + (elem is Branch ? elem.thickness : elem)) : 0;
        num akkuThickness = 0;
        for(Branch branch in branches) {
            if(branchNumber > 0) {
                spline.add(base.globalToLocal(localToGlobal(new Point( et*(akkuThickness/totalThickness-0.5), -length-off))), 0.0);
            }
            branch.addPoints(spline, base);
            branchNumber++;
            akkuThickness += branch.thickness;
        }

        // going down on the right
        spline.add(base.globalToLocal(localToGlobal(new Point(et/2, -length))), tangentLength);
        if(isBase) {
            spline.add(base.globalToLocal(localToGlobal(new Point(st/2, 0))), 0.1);
        }
    }

    void addVeinPoints(Spline spline, Branch end_branch, Branch from, num offset) {
        num off = branches.length > 0 ? min(0.1, branches.map((b) => b.length).reduce(max) * 0.5) : 0;
        num tangentLength = off*length*2;

        if(from != null) {
            int index = this.branches.indexOf(from) + 1;
            offset += ((index/(this.branches.length+1))-0.5)*thickness;
            //debugMessage = "$offset";
        }

        num lenFac = isEndBranch ? 1 : 0.8;
        spline.add(end_branch.globalToLocal(localToGlobal(new Point(offset, -length*lenFac))), tangentLength);

        if(!isBase) {
            (parent as Branch).addVeinPoints(spline, end_branch, this, offset);
        } else {
            spline.add(end_branch.globalToLocal(localToGlobal(new Point(offset * 0.5, 0))), tangentLength);
        }
    }

    num get startThickness => isBase ? thickness * 1.3 : (parent as Branch).thickness;

    Branch growChild(num angle, [num length = 1]) {
        Branch b = new Branch(0.001);
        b.baseRotation = angle;
        b.length = length;
        b._valve = 0.5;
        addChild(b);
        b.reset();
        return b;
    }

    void delete() {
        if(shape != null) removeChild(shape);
        shape = null;
        removeFromParent();
    }

    Vector get tipPosition {
        var p = view.globalToLocal(localToGlobal(new Point(0, 0)));
        return new Vector(p.x, p.y);
    }

    Vector get basePosition {
        var p = view.globalToLocal(localToGlobal(new Point(0, -length)));
        return new Vector(p.x, p.y);
    }

    void splitAt(Point p) {
        if(length < 0.2) {
            print("I do not split small branches");
            return;
        }

        var ratio = (-p.y/length).clamp(0.2, 0.8);

        var childBranches = branches;
        for(var b in childBranches) removeChild(b);

        var secondPart = growChild(0, length * (1-ratio));
        var newBranch = growChild(sign(p.x), thickness*0.8);

        length *= ratio;
        secondPart.thickness = thickness;
        newBranch.thickness = 0;
        // newBranch.x = sign(p.x) * thickness/2;
        thickness = lerp(startThickness, thickness, ratio);

        for(var b in childBranches) secondPart.addChild(b);
    }
}
