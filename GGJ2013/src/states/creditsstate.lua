-- credits

require("core/gamestate")
require("core/resources")

CreditsState = class("CreditsState", GameState)

function CreditsState:__init()
    self.lifetime = 0
end

function CreditsState:update(dt)
    self.lifetime = self.lifetime + dt
end

function CreditsState:draw()
    love.graphics.setBackgroundColor(17, 17, 17)
    love.graphics.setColor(200, 200, 200)

    love.graphics.setFont(resources.fonts.huge)
    love.graphics.print("Credits", 200, 200)

    love.graphics.setFont(resources.fonts.normal)
    love.graphics.print("Team:", 200, 300)

    local team = {"Arne Beer", "Fabian Stegemann", "Hans-Ole Hatzel",
                  "Julian Tobergte", "Marvin Arnold", "Rafael Epplee",
                  "Paul Bienkowski", "Sven-Hendrik Haase", "Tönjes Peters"}

    for i,v in ipairs(team) do
        love.graphics.print(v, 200, 300 + 30 * i)
    end
end

function CreditsState:keypressed(k, u)
    stack:pop()
end
