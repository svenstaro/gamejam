-- credits

require("core/gamestate")
require("core/resources")

CreditsState = class("CreditsState", GameState)

function CreditsState:__init()
    self.lifetime = 0
    self.canvas = nil
end

function CreditsState:update(dt)
    self.lifetime = self.lifetime + dt
end

function CreditsState:draw()
    local w, h = love.graphics.getWidth(), love.graphics.getHeight()
    if not self.canvas or self.canvas:getWidth() ~= w or self.canvas:getHeight() ~= h then
        self.canvas = love.graphics.newCanvas(w, h)
    end
    love.graphics.setCanvas(self.canvas)
    love.graphics.setBackgroundColor(17, 17, 17)
    love.graphics.clear()

    love.graphics.setColor(200, 200, 200)

    love.graphics.setFont(resources.fonts.handHuge)
    love.graphics.setColor(120, 0, 0)
    love.graphics.print("Credits", 200, 100)

    love.graphics.setFont(resources.fonts.handBig)

    local team = {
        "Arne Beer",
        "Fabian Stegemann",
        "Hans Ole Hatzel",
        "Julian Tobergte",
        "Marvin Arnold",
        "Paul Bienkowski",
        "Rafael Epplee",
        "Sven-Hendrik Haase",
        "Tönjes Peters"
    }

    for k, v in pairs(team) do
        love.graphics.setColor(255, 255, 255, math.max(0, math.min(1, - k * 0.1 + self.lifetime)) * 255)
        love.graphics.print(v, 185 + 15 * k, 200 + 35 * k)
    end

    -- distortion shader
    resources:sendShaderValue("distort", "lifetime", self.lifetime)
    resources:sendShaderValue("distort", "distortion", 1)
    resources:sendShaderValue("distort", "danger", 0) -- TODO determine danger
    love.graphics.setPixelEffect(resources.shaders.distort)
    love.graphics.setColor(255, 255, 255)
    love.graphics.setCanvas()
    love.graphics.draw(self.canvas, 0, 0)
    love.graphics.setPixelEffect()
end

function CreditsState:keypressed(k, u)
    stack:pop()
end
