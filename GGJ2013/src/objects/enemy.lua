-- the player object that moves around the screen

require("core/object")
require("objects/sprite")
require("core/vector")

Enemy = class("Enemy", Sprite)

function Enemy:__init(x, y, image)
    Sprite.__init(self, image)
    self.x = x
    self.y = y
    self.z = 0.8
    self.angle = 0
    self.speed = 64
    self.scale = 4
    self.startled = false
    self.range = 64 * 3
    self.attack = false

    self.route = {}
    self.currentTargetPoint = 1

    self.gotoTarget = nil
    self.gotoCallback = nil

    self.eating = resources:makeSound("knurren_bad_boy_2")
end

function Enemy:run()
    local p = self.route[self.currentTargetPoint]
    if not p then return end
    self:goto(p[1], p[2], function()
        if self.currentTargetPoint == #self.route then
            self.currentTargetPoint = 1
            self.x = self.route[1][1]
            self.y = self.route[1][2]
        else
            self.currentTargetPoint = self.currentTargetPoint + 1
        end
        self:run()
    end)
end

function Enemy:addRoutePoint(x, y)
    table.insert(self.route, {x, y})
end

function Enemy:goto(x, y, callback)
    self.gotoTarget = {x, y}
    self.gotoCallback = callback
end

function Enemy:update(dt)
    Sprite.update(self, dt)

    local diff = Vector(main.player.x - self.x, main.player.y - self.y)

    if diff:len() < 32 then
        main.player:kill(self)
    end

    local speed = 64 * 4

    if diff:len() <= self.range or self.startled then
        if self.attack == false then
            self.eating:play()
            self.attack = true
        end
    elseif self.gotoTarget and self.gotoCallback then
        diff = Vector(self.gotoTarget[1] - self.x, self.gotoTarget[2] - self.y)

        if (self.gotoTarget[1] == self.x and self.gotoTarget[2] == self.y) or diff:len() <= 1 then
            self.x, self.y = unpack(self.gotoTarget)
            self.gotoCallback()
            return
        end
        speed = self.speed
    else
        return -- don't go anywhere
    end

    diff:normalize()
    self.x = self.x + diff.x * dt * speed
    self.y = self.y + diff.y * dt * speed
    self.angle = math.atan2(diff.y, diff.x)
end
