-- game

require("arena")
require("asteroid")
require("label")
require("powerup")
require("ship")
require("ship_ai")
--require("ship_player")
require("util/gamestate")
require("util/resources")
require("world")
require("shipexplosion")

Game = class("Game", GameState)

MAX_MATERIAL = 25
MULTIPLIER_DELAY = 10

function Game:__init()
    self.selectedAsteroid = 2

    self.previewAsteroids = {}
    for i = 1,3 do
        local a = Asteroid(i)
        a.velocity = Vector()
        a.position = Vector(120, love.graphics.getHeight() / 2 + 100 * (i - 2))
        self.previewAsteroids[i] = a
    end

    self.gameOverAlpha = 0

    self.timerLabel = Label("0", Vector(0, -280), {255,255,255,220}, resources.fonts.huge)
    self.timerLabel.maxAlpha = 20
    self.timerLabel.scaleFactor = 0
    self.scoreLabel = Label("0", Vector(), nil, resources.fonts.epic)
    self.scoreLabel.maxAlpha = 50
    self.multiplierLabel = Label("x1", Vector(0, 80), nil, resources.fonts.huge)
    self.multiplierLabel.maxAlpha = 50
    self.powerLabel = Label("power 20", Vector(0, 110), nil, resources.fonts.normal)
    self.powerLabel.maxAlpha = 50
    self.materialLabel = Label("material 0", Vector(0, 136), nil, resources.fonts.normal)
    self.materialLabel.maxAlpha = 50
    self.levelLabel = Label("Level 1", Vector(0, -80), nil, resources.fonts.huge)
    self.levelLabel.scaleFactor = 2
    self.levelLabel.fadeTime = 1

    self.powerupTimer = 0

    self.world = World()
    arena = Arena()
    
    self:reset()
end

function Game:reset()
    self.world:clear()

    for i = 1, math.random(4,6) do
        self:addPowerup(i)
    end

    self.materialAvailable = MAX_MATERIAL

    self.over = false

    self.reset_ship_time = 0

    self.level = 1
    self.score = 0
    self.multiplier = 1
    self.power = 20
    self.multiplierTimer = 0
    self.shake = 0
    self:resetShip()
end

function Game:addScore(score)
    local s = score * self.multiplier
    local f = resources.fonts.toast
    if s > 20 then f = resources.fonts.normal end
    ship:makeToast("+" .. s, {255, 255, 128}, f)
    self.score = self.score + s
end

function Game:addShake(shake)
    self.shake = self.shake + shake
end

function Game:destroyShip()
    for k,e in pairs(self.world:findByType("Asteroid")) do
        self.materialAvailable = self.materialAvailable + materialValue(e.size)
        e:kill()
    end
    if ship then ship:kill() end
end

function Game:resetShip()
    ship = ShipAI()
    --if debug then player_ship = ShipPlayer() end

    --self.world:clear()
    --we draw that on our own
    --self.world:add(arena)

    self.world:add(ship)
    --if debug then self.world:add(player_ship) end

    self:setDifficulty()
    self.timer = 100 * math.pow(0.8, self.level - 1) + 20
end

function Game:start()
    resources.audio.game_music:play()
    resources.audio.game_music:setVolume(1)
end

function Game:stop()
    resources.audio.game_music:stop()
    self.shake = 0
end

function Game:draw()

    love.graphics.setColor(0, 0, 0)
    love.graphics.rectangle("fill", 0, 0, love.graphics.getWidth(), love.graphics.getHeight())

    love.graphics.setColor(255, 255, 255)
    love.graphics.setFont(resources.fonts.normal)

    if debug then
        local fps = love.timer.getFPS() .. " FPS"
        love.graphics.print(fps, love.graphics.getWidth() - love.graphics.getFont():getWidth(fps) - 10, 10)
    end

    love.graphics.setColor(255, 255, 255)
    love.graphics.push()
    -- love.graphics.translate(love.graphics.getWidth() / 2, love.graphics.getHeight() / 2)
    love.graphics.translate(580, 320)
    love.graphics.translate(math.random() * self.shake, math.random() * self.shake)

    --background image
    love.graphics.draw(resources.images.background, -arena.size.x / 2, -arena.size.y / 2)

    --scissor things
    love.graphics.setScissor(580-arena.size.x / 2, 320-arena.size.y / 2, arena.size.x, arena.size.y)

    local m = (self.timer - self.timer % 60) / 60
    local ms = (self.timer - m * 60) % 1
    local s = self.timer - m * 60 - ms
    local cs = ms * 100
    cs = cs - (cs % 1)

    self.timerLabel:setText(string.format("%i:%02i:%02i", m, s, cs))
    self.scoreLabel:setText(tonumber(self.score))
    self.multiplierLabel:setText("x" .. tonumber(self.multiplier))
    self.powerLabel:setText("power " .. tonumber(self.power))
    self.materialLabel:setText("material " .. tonumber(self.materialAvailable))
    self.levelLabel:setText("Level " .. self.level)

    self.scoreLabel:draw()
    self.powerLabel:draw()
    self.materialLabel:draw()
    self.multiplierLabel:draw()
    self.levelLabel:draw()

    self.world:draw(dt)
    --reset scissor
    love.graphics.setScissor()

    self.timerLabel:drawWithStandardColor()

    --draw new asteroids
    self.world:drawNewAsteroids()

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

    local m = Powerup(Vector(20, love.graphics.getHeight() - 40), "multiplier")
    m.dieAt = math.huge
    m:update(self.lifetime)
    for i = 1, self.multiplier do
        m.position.x = m.position.x + 15 
        m:draw()
    end

    for i = 1,3 do
        local a = self.previewAsteroids[i]
        a:draw()

        if i == self.selectedAsteroid then
            love.graphics.setFont(resources.fonts.normal)
            local n = tonumber(math.floor(self.materialAvailable / materialValue(i)))
            love.graphics.print(n, a.position.x + 35 , a.position.y - love.graphics.getFont():getHeight() / 2)
        end

        love.graphics.setFont(resources.fonts.tiny)
        local names = {"small", "medium", "large"}
        local name = names[i]
        love.graphics.print(name, a.position.x - love.graphics.getFont():getWidth(name) - 35 , a.position.y - love.graphics.getFont():getHeight() / 2)

        love.graphics.print(i, a.position.x-94, a.position.y-30)
    end

    local my = self.previewAsteroids[self.selectedAsteroid].position.y
    love.graphics.setColor(255, 255, 255, 30)

    love.graphics.rectangle("fill", 20, my - 40, 170, 80)

    if self.over then
        love.graphics.setColor(0, 0, 0, 150)
        love.graphics.rectangle("fill", 0, 0, love.graphics.getWidth(), love.graphics.getHeight())
        
        love.graphics.setFont(resources.fonts.epic)
        love.graphics.setColor(255, 255, 255)
        love.graphics.print("Game Over",
                            (love.graphics.getWidth() - love.graphics.getFont():getWidth("Game Over")) / 2,
                            (love.graphics.getHeight() - love.graphics.getFont():getHeight()) / 2 - 100)
        
        love.graphics.setFont(resources.fonts.huge)

        local s = self.score .. " points"
        love.graphics.print(s,
                            (love.graphics.getWidth() - love.graphics.getFont():getWidth(s)) / 2,
                            (love.graphics.getHeight() - love.graphics.getFont():getHeight()) / 2)

        love.graphics.setFont(resources.fonts.normal)

        love.graphics.setColor(255, 255, 255, (0.5 + math.abs(math.sin(self.gameOverAlpha * 3)) * 0.5) * 255)
        s = "Press ENTER to continue"
        love.graphics.print(s,
                            (love.graphics.getWidth() - love.graphics.getFont():getWidth(s)) / 2,
                            (love.graphics.getHeight() - love.graphics.getFont():getHeight()) / 2 + 60)
    end
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

function Game:addPowerup(i)
    local rewards = {"material", "multiplier", "power"}
    local p = Powerup(Vector(
        arena.size.x * (math.random() * 0.5 - 0.25), 
        arena.size.y * (math.random() * 0.5 - 0.25)
        ), rewards[math.random(1,3)])
    if i then
        p.dieAt = i * 5
    end
    self.world:add(p)
end

function Game:update(dt)
    if not self:isResettingShip() then
        self.timer = self.timer - dt
        if self.timer <= 0 then
            self.timer = 0
            self.isOver()
        end
    end

    if self.over then
        self.gameOverAlpha = self.gameOverAlpha + dt
        return
    end

    if 0 == math.random(0, 10 / dt) then
        self:addPowerup()
    end

    self.timerLabel:update(dt)
    self.scoreLabel:update(dt)
    self.multiplierLabel:update(dt)
    self.levelLabel:update(dt)
    self.powerLabel:update(dt)
    self.materialLabel:update(dt)

    --ship respawning
    if self.reset_ship_time > 0 then
        self.reset_ship_time = self.reset_ship_time - dt
        if self.reset_ship_time <= 0 then
            self:resetShip()
        end
    end

    local shake_time = 0.4
    self.shake = self.shake * (1 - dt / shake_time)

    if self.powerupTimer > 0 then
        self.powerupTimer = self.powerupTimer - dt
    end
    if self.powerupTimer <= 0 then
        self:addPowerup()
        self.powerupTimer = math.random() + 3 -- 3 to 4
    end

    if self.multiplierTimer > 0 then
        self.multiplierTimer = self.multiplierTimer - dt
    end
    if self.multiplierTimer <= 0 and self.multiplier > 1 then
        self.multiplier = self.multiplier - 1
        self.multiplierTimer = MULTIPLIER_DELAY
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
    if not self.over then
        if k == "escape" then
            if debug then
                stopGame()
            else
                self:transitionTo(menu, "left")
            end
        elseif debug and k == "0" then
            self:isOver()
        elseif k == "1" then
            self.selectedAsteroid = 1
        elseif k == "2" then
            self.selectedAsteroid = 2
        elseif k == "3" then
            self.selectedAsteroid = 3
        end
    else
        if k == "escape" or k == "enter" or k == "return" then
            self:transitionTo(menu, "left")
        end
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
            --player_ship:shoot()
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
    local ship_ai = self.world:findByType("ShipAI")[1]
    self:addScore(self.materialAvailable)
    explosion = ShipExplosion(ship_ai.position, 0.01)
    self.world:add(explosion)

    self:addShake(20)
    self.level = self.level + 1
    --ssshhhhhh, not now
    --self:resetShip()
    self:destroyShip()
    self.reset_ship_time = 2.5
end

function Game:setDifficulty()
    local factor = self.level - 1
    ai_turn_speed = math.pi * (0.2 + factor * 0.4)
    max_ship_speed = 30 + factor * 30
    shoot_delay = math.max(0.8 - factor * 0.15, 0.1)
end

function Game:isOver()
    self.over = true
    self:stop()
end

function Game:isResettingShip()
    return self.reset_ship_time > 0
end
