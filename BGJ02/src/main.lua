require("game")
require("intro")
require("menu")
require("util/resources")

debug = false

tween = require("externals/tween/tween")

resources = Resources("data/")
currentState = nil

function reset()
    -- start game
    intro = Intro()
    menu = Menu()
    game = Game()

    if debug then
        currentState = game
    else
        currentState = intro
    end
end

function love.load()
    math.randomseed(os.time())

    -- load images
    resources:addImage("test", "test.jpg")
    resources:addImage("rock", "rock.png")
    resources:addImage("particle", "particle.png")

    -- load fonts
    resources:addFont("tiny", "DejaVuSans.ttf", 14)
    resources:addFont("normal", "DejaVuSans.ttf", 20)
    resources:addFont("huge", "DejaVuSans.ttf", 30)

    -- load music / sounds
    resources:addAudio("swipe", "sound/swipe.ogg", false) -- no stream
    resources:addAudio("background", "THA-emptychasms.mp3", true)
    
    -- load shaders
    resources:addShader("pixelate", "shader/pixelate.glsl")

    resources:load()

    reset()
end

function love.update(dt)
    tween.update(dt)
    if currentState then
        currentState:doUpdate(dt)
    else
        love.event.quit()
    end
end

function love.draw()
    if currentState then currentState:doDraw() end

    -- love.graphics.setFont(resources.fonts.tiny)
    -- love.graphics.print("FPS: " .. love.timer.getFPS(), 5, 5)
end

function love.keypressed(k, u)
    if currentState and currentState.transition == 0 then currentState:keypressed(k, u) end
end

function love.mousepressed(x, y, mb)
    if currentState and currentState.transition == 0 then currentState:mousepressed(x, y, mb) end
end

function love.keypressed(k, u)
    if currentState and currentState.transition == 0 then currentState:keypressed(k, u) end
end
