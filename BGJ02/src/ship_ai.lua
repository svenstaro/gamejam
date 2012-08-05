require("ship")

ShipAI = class("ShipAI", Ship)

function ShipAI:__init()
    Ship.__init(self)
    self.directionVector = Vector(0, 0)
    
    self.idleTime = 0
end

function ShipAI:update(dt)
    local asteroids = self.world:findByType("Asteroid")

    local v = Vector(0,0)
    local shortestDistance = math.huge
    for i, a in pairs(asteroids) do
        local distance = a.position - self.position
        if distance:len2() > 0 and distance:len() < 200 then
            local dist = distance / distance:len2()
            --fly away
            v = v - dist
            --adapt to asteroid movement
            v = v + a.velocity / distance:len2()
            --find shortest distance
            shortestDistance = math.min(shortestDistance, distance:len())
        end
    end

    -- fear the mouse
    local distance = getMouseVector() - self.position
    local dist = distance / distance:len2()
    v = v - dist * 2

    if v:len2() > 0 then
        --go towards center
        v = v - self.position:normalized() * v:len()

        local rotatedVector = Vector(1,0):rotated(self.rotation)
        local angle = rotatedVector:angleTo(v)
        if angle then
            local clockwise = rotatedVector:rotated(0.0001):angleTo(v) < angle
            local factor = 1
            if not clockwise then
                factor = -1
            end
            --self.rotation = self.rotation + math.min(turn_speed, angle) * factor * dt
            if angle >= math.pi / 6 then
                self.rotation = self.rotation + turn_speed * 3 * factor * dt
            end
        end

        if angle < math.pi / 2 then
            speed = 1
        else
            speed = -1
        end

        if shortestDistance > 100 then shortestDistance = 1 end
        self:move(speed / math.max((shortestDistance * 0.01) -1, 1), dt)

        self.idleTime = 0
    else
        self.idleTime = self.idleTime + dt
        if self.idleTime >= 3 then
            local speed = self.velocity:len()
            local rot = Vector(1,0):rotated(self.rotation):angleTo(Vector(1,0))
            local angle = math.abs(Vector(1,0):angleTo(-self.velocity) - rot)
            if speed > 5 then
                if angle > 0.1 then
                    local rotate = math.min(angle, turn_speed * dt)
                    self.rotation = self.rotation + rotate
                else
                    self:move(math.min(1,speed), dt)
                end
            elseif shortestDistance < 400 then
                self.rotation = self.rotation + turn_speed * dt
            end
        end
    end

    --for debug purposes
    self.directionVector = v

    Ship.update(self, dt)

    if shortestDistance < 700 then
        self:shoot()
    end
end

function ShipAI:draw()
    Ship.draw(self)
    if debug then
        love.graphics.setColor(0,255,0)
        local to = self.position + self.directionVector:normalized() * 30
        love.graphics.line(self.position.x, self.position.y, to.x, to.y)

        love.graphics.setColor(255,0,0)
        to = self.position + self.directionVector * 5000
        love.graphics.line(self.position.x, self.position.y, to.x, to.y)
    end
end

function ShipAI:hitByAsteroid(asteroid)
    self.crashScheduled = true
end
