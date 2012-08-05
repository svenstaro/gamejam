-- game

require("arena")
require("util/gamestate")
require("util/resources")

require("asteroid")
require("powerup")
require("ship")
require("ship_ai")
require("ship_player")
require("world")

Game = class("Game", GameState)

MAX_MATERIAL = 25

function Game:__init()
    self.selectedAsteroid = 2

    self.previewAsteroids = {}
    for i = 1,3 do
        local a = Asteroid(i)
        a.velocity = Vector()
        a.position = Vector(120, love.graphics.getHeight() / 2 + 100 * (i - 2))
        self.previewAsteroids[i] = a
    end

    self.world = World()
    arena = Arena()
    self:reset()
end

function Game:reset()
    self.score = 0
    self:resetShip()
end

function Game:resetShip()
    self.materialAvailable = MAX_MATERIAL
    
    ship = ShipAI()
    if debug then player_ship = ShipPlayer() end

    self.world:clear()
    --we draw that on our own
    --self.world:add(arena)
    self.world:add(ship)
    if debug then self.world:add(player_ship) end

end

function Game:start()
    resources.audio.game_music:play()
    resources.audio.game_music:setVolume(1)
end

function Game:stop()
    resources.audio.game_music:stop()
end

function Game:draw()
    love.graphics.setColor(0, 0, 0)
    love.graphics.rectangle("fill", 0, 0, love.graphics.getWidth(), love.graphics.getHeight())

    love.graphics.setColor(255, 255, 255)
    love.graphics.setFont(resources.fonts.normal)

    local s = love.timer.getFPS() .. " FPS"
    love.graphics.print(s, love.graphics.getWidth() - love.graphics.getFont():getWidth(s) - 10, 40)

    s = tonumber(self.score)
    love.graphics.print(s, love.graphics.getWidth() - love.graphics.getFont():getWidth(s) - 10, 10)

    love.graphics.push()
    -- love.graphics.translate(love.graphics.getWidth() / 2, love.graphics.getHeight() / 2)
    love.graphics.translate(580, 320)

    --scissor things
    love.graphics.setScissor(580-arena.size.x / 2, 320-arena.size.y / 2, arena.size.x, arena.size.y)
        love.graphics.draw(resources.images.background, -arena.size.x / 2, -arena.size.y / 2)
        self.world:draw(dt)
    love.graphics.setScissor()
    arena:draw()
    love.graphics.pop()

    -- draw material bar
    
    local bar_b = 2
    local bar_h = 10 
    local bar_w = 140 - bar_b * 4
    local bar_x = 10
    local bar_y = love.graphics.getHeight() - 10 - bar_h
    local bar_v = self.materialAvailable / MAX_MATERIAL * bar_w
    love.graphics.setColor(255, 255, 255, 128)
    love.graphics.rectangle("fill", bar_x, bar_y, bar_w, bar_h)
    love.graphics.setColor(255, 255, 255)
    love.graphics.rectangle("fill", bar_x, bar_y, bar_v, bar_h)

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

function Game:addPowerup()
    local rewards = {"material", "multiplier", "power"}
    self.world:add(Powerup(Vector(
        arena.size.x * (math.random() * 0.5 - 0.25), 
        arena.size.y * (math.random() * 0.5 - 0.25)
        ), rewards[math.random(1,3)]))
end

function Game:update(dt)
    if 0 == math.random(0, 10 / dt) then
        self:addPowerup()
    end

    self.world:update(dt)
    arena:update(dt)

    for i = 1,3 do
        self.previewAsteroids[i]:update(dt)
    end

    if self.transition then
        resources.audio.game_music:setVolume(1 - self.transition)
    end
end

function Game:keypressed(k, u)
    if k == "escape" then
        if debug then
            stopGame()
        else
            self:transitionTo(menu, "left")
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
        elseif k == " " then
            local a = Asteroid(math.random(1,3))
            a.position = getMouseVector()
            a.velocity = Vector(math.random(-20, 20), math.random(-20, 20))
            self.world:add(a)
        elseif k == "lshift" then
            player_ship:shoot()
        elseif k == "b" then
            for k,e in pairs(self.world:findByType("Asteroid")) do
                e:crush()
            end
        elseif k == "p" then
            self:addPowerup()
        end
    end
end

function Game:shipCrashed()
    resources.audio.explosion_player:play()
    self:reset()
end
