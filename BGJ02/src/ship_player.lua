require("ship")

ShipPlayer = class("ShipPlayer", Ship)

function ShipPlayer:__init()
    Ship.__init(self)
end

function ShipPlayer:update(dt)
    speed_add = 0
    if love.keyboard.isDown("up") then
        speed_add = 1
    elseif love.keyboard.isDown("down") then
        speed_add = -1
    end

    self:move(speed_add, dt)

    if love.keyboard.isDown("left") then
        self.rotation = self.rotation - turn_speed * dt
    elseif love.keyboard.isDown("right") then
        self.rotation = self.rotation + turn_speed * dt
    end

    Ship.update(self, dt)
end
