require("ship")

ShipAI = class("ShipAI", Ship)

function ShipAI:__init()
    Ship.__init(self)
    self.directionVector = Vector(0, 0)
    
    self.idleTime = 0
end

function ShipAI:update(dt)
    --local arena = self.world:findByType("Arena")[1]
    local asteroids = self.world:findByType("Asteroid")

    local v = Vector(0,0)
    local shortestDistance = math.huge
    for x = -1, 1 do
        for y = -1, 1 do
            v, shortestDistance = self:checkAsteroids(v, shortestDistance, asteroids, x * arena.size.x, y * arena.size.y)
        end
    end

    -- fear the mouse if its dragging shit
    if arena.dragging then
        local distance = arena:ref() - self.position
        if distance:len() < 150 then
            local dist = distance / distance:len2()
            v = v - dist * 2
        end
    end

    if shortestDistance < 200 then
        if v:len2() > 0 then
            --go towards center
            --v = v - self.position:normalized() * v:len()

            local rotatedVector = Vector(1,0):rotated(self.rotation)
            local angle = rotatedVector:angleTo(v)
            if angle > 0.01 then --???
                local clockwise = rotatedVector:rotated(0.0001):angleTo(v) < angle
                local factor = 1
                if not clockwise then
                    factor = -1
                end
                --
                if angle >= math.pi / 6 then
                    --self.rotation = self.rotation + ai_turn_speed * factor * dt
                    self.rotation = self.rotation + math.min(ai_turn_speed * dt, angle) * factor
                end
            end

            --angeblich hat sascha das raus genommen gehabt. dunno
            if angle < math.pi / 2 then
                speed = 1
            else
                speed = -1
            end

            local sd = shortestDistance
            if sd > 100 then sd = 1 end
            self:move(speed / math.max((sd * 0.01) -1, 1), dt)

            --shoot if theres something
            if shortestDistance < math.huge then
                local target = Vector(500, 0):rotated(self.rotation) + self.position
                self.world.physicsWorld:rayCast(self.position.x, self.position.y, target.x, target.y, 
                                                function(fixture, x, y, xn, yn, fraction)
                                                    return self:wouldHitAsteroid(fixture, x, y, xn, yn, fraction)
                                                end)
            end
        end
    else
        if shortestDistance < math.huge then
            local asteroid_position = asteroids[1].position
            local asteroid_direction = asteroid_position - self.position
            asteroid_position = asteroid_position + (asteroids[1].velocity * asteroid_direction:len() * 0.01)

            local direction = asteroid_position - self.position
--self.directionVector = direction
            local rotatedVector = Vector(1,0):rotated(self.rotation)
--self.directionVector = rotatedVector * 300
            local angle = rotatedVector:angleTo(direction)
            if angle > 0 then
                local clockwise = rotatedVector:rotated(0.001):angleTo(direction) < angle
                local factor = 1
                if not clockwise then
                    factor = -1
                end
                self.rotation = self.rotation + math.min(angle, ai_turn_speed * dt) * factor
            end
            if angle < 0.1 then
                self:shoot()
            end
        end
    end

    --for debug purposes
    self.directionVector = v

    Ship.update(self, dt)
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

function ShipAI:checkAsteroids(v, shortestDistance, asteroids, x, y)
    local offset = Vector(x, y)
    for i, a in pairs(asteroids) do
        local distance = (a.position + offset) - self.position
        if distance:len2() > 0 and distance:len() < 200 then
            local dist = (distance / distance:len2())-- / distance:len2()
            --fly away
            v = v - dist
            --adapt to asteroid movement
            v = v + a.velocity / distance:len2()
        end
        --find shortest distance
        shortestDistance = math.min(shortestDistance, distance:len())
    end
    return v, shortestDistance
end

function ShipAI:wouldHitAsteroid(fixture, x, y, xn, yn, fraction)
    if fixture:getUserData().__name == "Asteroid" then
        self:shoot()
    end

    return 0
end
