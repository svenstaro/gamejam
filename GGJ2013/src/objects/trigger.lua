-- triggers some callback when the player enters an area

require("core/object")

Trigger = class("Trigger", Object)

function Trigger:__init()
    self.z = 100
    self.inside = false

    self.onEnter = function() end
    self.onLeave = function() end
    self.onChange = function() end

    self.enabled = true
end

function Trigger:isInside(player)
    error("Trigger should not be used directly, take RectangleTrigger or CircleTrigger")
    return false
end

function Trigger:update(dt)
    if not self.enabled then return end

    local inside = self:isInside(main.player)

    if inside ~= self.inside then
        if inside then
            self.onEnter()
        else
            self.onLeave()
        end
        self.onChange()
    end

    self.inside = inside
end

--------------------------------------------------------------------------------

RectangleTrigger = class("RectangleTrigger", Trigger)

function RectangleTrigger:__init(x, y, w, h)
    Trigger.__init(self)
    self.x = x
    self.y = y
    self.w = w
    self.h = h
end

function RectangleTrigger:draw()
    if debug then
        love.graphics.setColor(0, 255, 0)
        love.graphics.rectangle("line", self.x, self.y, self.w, self.h)
    end
end

function RectangleTrigger:isInside(player)
    return player.x >= self.x and player.y >= self.y and player.x <= self.x + self.w and player.y <= self.y + self.h
end

--------------------------------------------------------------------------------

CircleTrigger = class("CircleTrigger", Trigger)

function CircleTrigger:__init(x, y, r)
    Trigger.__init(self)
    self.x = x
    self.y = y
    self.r = r
end

function CircleTrigger:draw()
    if debug then
        love.graphics.setColor(0, 255, 0)
        love.graphics.circle("line", self.x, self.y, self.r)
    end
end

function CircleTrigger:isInside(player)
    local dx = player.x - self.x
    local dy = player.y - self.y
    return math.sqrt(dx * dx + dy * dy) <= self.r
end
