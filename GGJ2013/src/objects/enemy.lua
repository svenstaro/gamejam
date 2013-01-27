-- the player object that moves around the screen

require("core/object")
require("objects/sprite")
require("core/vector")

Enemy = class("Enemy", Sprite)

function Enemy:__init(x, y)
    Sprite.__init(self)
    self:setAnimation(resources.images.slime, 16, 16, 0.5, 3)
    self.x = x
    self.y = y
    self.z = 0.8
    self.angle = 0
    self.speed = 64
    self.scale = 4
    self.startled = true

    self.route = {}
    self.currentTargetPoint = 1

    self.gotoTarget = {self.x, self.y}
    self.gotoCallback = nil
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

    if self.gotoTarget and self.gotoCallback then
        local diff = Vector(self.gotoTarget[1] - self.x, self.gotoTarget[2] - self.y)
        if (self.gotoTarget[1] == self.x and self.gotoTarget[2] == self.y) or diff:len() <= 1 then
            self.x, self.y = unpack(self.gotoTarget)
            self.gotoCallback()
        else
            diff:normalize()
            self.x = self.x + diff.x * dt * self.speed
            self.y = self.y + diff.y * dt * self.speed
        end

        self.angle = diff:angleTo(Vector(1, 0))-- math.atan2(main.player.y - self.y, main.player.x - self.x)
    end
end
