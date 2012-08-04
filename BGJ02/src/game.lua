-- game

require("arena")
require("util/gamestate")
require("util/resources")

require("ship")
require("ship_player")
require("ship_ai")
require("asteroid")
require("world")

Game = class("Game", GameState)

MAX_MATERIAL = 80

function Game:__init()
    self.selectedAsteroid = 3

    self.previewAsteroids = {}
    for i = 1,3 do
        local a = Asteroid(i)
        a.velocity = Vector()
        a.position = Vector(120, love.graphics.getHeight() / 2 + 100 * (i - 2))
        self.previewAsteroids[i] = a
    end

    self.world = World()
    self:reset()

    if debug then
        ship = ShipPlayer()
        self.world:add(ship)
    end
    self.world:add(ShipAI())

    arena = Arena()
    self.world:add(arena)
end

function Game:reset()
    self.materialAvailable = MAX_MATERIAL
    self.world:clear()
end

function Game:draw()
    love.graphics.setBackgroundColor(0, 0, 0)
    love.graphics.clear()

    love.graphics.setColor(255, 255, 255)
    love.graphics.setFont(resources.fonts.normal)

    local s = love.timer.getFPS() .. " FPS"
    love.graphics.print(s, love.graphics.getWidth() - love.graphics.getFont():getWidth(s) - 10, 10)

    love.graphics.push()
    love.graphics.translate(love.graphics.getWidth() / 2, love.graphics.getHeight() / 2)
    self.world:draw(dt)
    love.graphics.pop()

    -- draw material bar
    
    local bar_h = 400
    local bar_w = 20
    local bar_b = 2
    local bar_x = love.graphics.getWidth() - 50 - bar_w / 2
    local bar_y = love.graphics.getHeight() / 2
    local bar_v = self.materialAvailable / MAX_MATERIAL * bar_h
    love.graphics.setColor(255, 255, 255)
    love.graphics.rectangle("line", bar_x - bar_w / 2 - bar_b * 2, 
        bar_y - bar_h / 2 - bar_b * 2,
        bar_w + bar_b * 4,
        bar_h + bar_b * 4)
    love.graphics.rectangle("fill", bar_x - bar_w / 2, bar_y - bar_v + bar_h / 2, bar_w, bar_v)

    for i = 1,3 do
        local a = self.previewAsteroids[i]
        a:draw()

        love.graphics.setFont(resources.fonts.normal)
        local n = tonumber(math.floor(self.materialAvailable / materialValue(i)))
        love.graphics.print(n, a.position.x - love.graphics.getFont():getWidth(n) - 35 , a.position.y - love.graphics.getFont():getHeight() / 2 - 10)

        love.graphics.setFont(resources.fonts.tiny)
        local names = {"small", "medium", "large"}
        local name = names[i]
        love.graphics.print(name, a.position.x - love.graphics.getFont():getWidth(name) - 35 , a.position.y - love.graphics.getFont():getHeight() / 2 + 10)
    end

    local my = self.previewAsteroids[self.selectedAsteroid].position.y
    love.graphics.setColor(255, 255, 255, 30)
    love.graphics.rectangle("fill", 20, my - 40, 140, 80)
end

function Game:mousepressed(x, y, mb)
    if mb == "wu" then
        self.selectedAsteroid = self.selectedAsteroid - 1
    elseif mb == "wd" then
        self.selectedAsteroid = self.selectedAsteroid + 1
    end

    if self.selectedAsteroid > 3 then 
        self.selectedAsteroid = 1
    elseif self.selectedAsteroid < 1 then 
        self.selectedAsteroid = 3
    end
end

function Game:update(dt)
    self.world:update(dt)
end

function Game:keypressed(k, u)
    if k == "escape" then
        stack:pop() 
    elseif debug then
        if k == " " then
            local a = Asteroid(math.random(1,3))
            a.position = ship.position
            a.velocity = Vector(math.random(-20, 20), math.random(-20, 20))
            self.world:add(a)
        elseif k == "lshift" then
            ship:shoot()
        elseif k == "b" then
            for k,e in pairs(self.world:findByType("Asteroid")) do
                e:crush()
            end
        end
    elseif k == "1" then
        self.selectedAsteroid = 1
    elseif k == "2" then
        self.selectedAsteroid = 2
    elseif k == "3" then
        self.selectedAsteroid = 3
    end

    -- DEBUG CONTROLS
    if debug then
        if k == "r" then
            self.materialAvailable = 80
        end
    end
end

