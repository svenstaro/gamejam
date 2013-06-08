require("util/resources")
require("util/gamestack")

require("states/intro")
require("states/game")

-- global subsystems
resources = Resources("data/")

-- global variables
SIZE = Vector(love.graphics.getWidth(), love.graphics.getHeight())
HALFSIZE = SIZE / 2
GENERATE_AHEAD = SIZE.x * 4
MAX_HEIGHT = 1500
LIGHT_CANVAS = nil
DEBUG = false
LIGHTRANGE = 200
WINDOWDATA = "a29ujosdkkqjwebanldkjsnldkfmnlasjnd1fkl2qawkcsmnxlkj2ma2"

function reset()
    -- start game
    states = {}
    states.intro = Intro()
    states.game = Game()
    stack = GameStack()
    stack:push(states.intro)
end

function love.load()
    math.randomseed(os.time())

    LIGHT_CANVAS = love.graphics.newCanvas(SIZE:unpack())

    -- load images
    resources:addImage("particle", "particle.png")
    resources:addImage("particle_square", "particle-square.png")
    resources:addImage("sparkle", "sparkle.png")
    resources:addImage("ring", "ring.png")
    resources:addImage("lantern", "lantern.png")
    resources:addImage("streetlamp1", "streetlamp1.png")
    resources:addImage("chainlamp", "chainlamp.png")
    resources:addImage("chain0", "chain0.png")
    resources:addImage("chain1", "chain1.png")
    resources:addImage("key_arrow", "key-arrow.png")
    resources:addImage("key_esc", "key-esc.png")
    resources:addImage("key_space", "key-space.png")

    resources:makeGradientImage("left", {0, 0, 0, 255}, {0, 0, 0, 0}, true)
    resources:makeGradientImage("sky",  {10, 20, 40}, {120, 150, 255})

    -- load fonts
    resources:addFont("normal", "DejaVuSans.ttf", 12)

    -- load music
    -- resources:addMusic("background", "background.mp3")

    resources:load()

    reset()
end

function love.update(dt)
    stack:update(dt)
end

function love.draw()
    stack:draw()

    -- love.graphics.setFont(resources.fonts.tiny)
    -- love.graphics.print("FPS: " .. love.timer.getFPS(), 5, 5)
end

function love.keypressed(k, u)
    stack:keypressed(k, u)
end

function love.mousepressed( x, y, button )
    stack:mousepressed(x, y, button)
end

function love.quit()
end
