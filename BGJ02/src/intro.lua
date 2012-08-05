-- intro

require("util/gamestate")
require("util/resources")

Intro = class("Intro", GameState)

function centerText(text, x, y, font)
    if font then
        love.graphics.setFont(font)
    else
        font = love.graphics.getFont()
    end

    love.graphics.print(text,
        x - font:getWidth(text) / 2,
        y - font:getHeight() / 2)
end

function Intro:__init()
    self.asteroid = Asteroid(3)
    self.asteroid.velocity = Vector()
    self.ship = Ship()

    self.powerupPower = Powerup(Vector(), "power")
    self.powerupMaterial = Powerup(Vector(), "material")
    self.powerupMultiplier = Powerup(Vector(), "multiplier")
end

function Intro:update(dt)
    GameState.update(self, dt)

    self.asteroid:update(dt)

    self.powerupPower:update(dt)
    self.powerupMaterial:update(dt)
    self.powerupMultiplier:update(dt)

    self.powerupPower.dieAt = self.lifetime + 10000
    self.powerupMaterial.dieAt = self.lifetime + 10000
    self.powerupMultiplier.dieAt = self.lifetime + 10000
end

function Intro:draw()
    love.graphics.setColor(255, 255, 255)

    local c = love.graphics.getWidth() / 2
    local h = 2 - self.lifetime
    if h < 1 then h = 1 end

    centerText("Welcome to", c, 100 * h, resources.fonts.normal)
    centerText("S.D.I.O.R.E.T.S.A", c, 160 * h, resources.fonts.mega)

    love.graphics.setColor(255, 255, 255)

    centerText("Your OBJECTIVE is to hit the SHIP with the ASTEROIDS.", c, 250 * h, resources.fonts.tiny)
    self.asteroid.position = Vector(c - 70, 300 * h)
    self.asteroid:draw()

    love.graphics.setColor(255, 255, 255)
    centerText(">", c, 300 * h)

    self.ship.position = Vector(c + 50, 300 * h)
    self.ship:draw()

    love.graphics.setColor(255, 255, 255)
    centerText("Click the border of the arena and DRAW outwards to shoot an asteroid.", c, 350 * h)

    centerText("You only have limited amount of TIME to hit the ship, and limited number of rock MATERIAL.", c, 380 * h)

    centerText("Collect powerups to get more material, shot power or points", c, 450 * h)

    self.powerupPower.position = Vector(c - 200, 500 * h)
    self.powerupMaterial.position = Vector(c, 500 * h)
    self.powerupMultiplier.position = Vector(c + 200, 500 * h)
    self.powerupPower:draw()
    centerText("Power", c - 200, 520 * h)
    centerText("Shoot at higher speed", c - 200, 540 * h, resources.fonts.toast)
    self.powerupMaterial:draw()
    centerText("Material", c, 520 * h)
    centerText("Get more rocks to shoot", c, 540 * h)
    self.powerupMultiplier:draw()
    centerText("Score Multiplier", c + 200, 520 * h)
    centerText("More points for a limited time", c + 200, 540 * h)

    love.graphics.setColor(255, 255, 255, (math.abs(math.sin(self.lifetime * 6)) * 0.5 + 0.5) * 255)
    centerText("Press ANY KEY to continue", c, 600 * h, resources.fonts.normal)

    local a = self.lifetime
    if a < 0.5 then 
        a = 1
    elseif a < 1.5 then
        a = 1.5 - a
    else
        a = 0
    end
    love.graphics.setColor(0, 0, 0, a * 255)
    love.graphics.rectangle("fill", 0, 0, love.graphics.getWidth(), love.graphics.getHeight())
end

function Intro:skip()
    self:transitionTo(menu)
end

function Intro:keypressed(k, u)
    self:skip()
end

