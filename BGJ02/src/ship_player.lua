require("ship")

ShipPlayer = class("ShipPlayer", Ship)

function ShipPlayer:__init()
    Ship.__init(self)
end

function ShipPlayer:update(dt)
    speed_add = 0
    if love.keyboard.isDown("up") then
        speed_add = 500
    elseif  love.keyboard.isDown("down") then
        speed_add = -500
    end
    self.velocity = self.velocity + Vector(0, -speed_add * dt):rotated(self.rotation)
    max = 200
    if self.velocity:len() > max then
        self.velocity:normalize()
        self.velocity = self.velocity * max
    end

    turn_speed = math.pi
    if love.keyboard.isDown("left") then
        self.rotation = self.rotation - turn_speed * dt
    elseif love.keyboard.isDown("right") then
        self.rotation = self.rotation + turn_speed * dt
    end

    Ship.update(self, dt)
end
