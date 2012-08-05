require("game")
require("intro")
require("menu")
require("util/resources")

tween = require("externals/tween/tween")

-- debug = false

resources = Resources("data/")
currentState = nil

function stopGame()
    setState(nil)
end

function setState(state)
    if currentState == state then return end

    if currentState then currentState:stop() end
    currentState = state

    if currentState then currentState:start() end
end

function reset()
    -- start game
    intro = Intro()
    menu = Menu()
    game = Game()

    if debug then
        setState(game)
    else
        setState(intro)
    end
end

function love.load()
    math.randomseed(os.time())

    -- load images
    resources:addImage("test", "test.jpg")
    resources:addImage("rock", "rock.png")
    resources:addImage("particle", "particle.png")
    resources:addImage("background", "background.png")
    resources:addImage("ring", "ring.png")

    -- load fonts
    resources:addFont("toast", "DejaVuSans.ttf", 10)
    resources:addFont("tiny", "DejaVuSans.ttf", 14)
    resources:addFont("normal", "DejaVuSans.ttf", 20)
    resources:addFont("huge", "DejaVuSans.ttf", 30)
    resources:addFont("mega", "DejaVuSans.ttf", 70)
    resources:addFont("epic", "DejaVuSans.ttf", 140)

    -- load music / sounds
    resources:addAudio("click", "sound/click.ogg", false)
    resources:addAudio("swipe", "sound/swipe.ogg", false)
    resources:addAudio("explosion_player", "sound/explosion_player.ogg", false)
    resources:addAudio("explosion_asteroid", "sound/explosion_asteroid.ogg", false)
    resources:addAudio("shot", "sound/shot.ogg", false)
    resources:addAudio("sling", "sound/shot.ogg", false)
    resources:addAudio("game_music", "sound/THA-emptychasms.ogg", true)
    resources:addAudio("menu_music", "sound/THA-simplythink.ogg", true)
    
    -- load shaders
    if love.graphics.isSupported("pixeleffect") then
        resources:addShader("pixelate", "shader/pixelate.glsl")
    end

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
