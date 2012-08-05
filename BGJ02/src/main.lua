require("game")
require("intro")
require("menu")
require("util/gamestack")
require("util/resources")

resources = Resources("data/")

function reset()
    -- start game
    intro = Intro()
    menu = Menu()
    game = Game()
    stack = GameStack()

    if debug then
        stack:push(game)
    else
        stack:push(intro)
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

    -- load music
     resources:addMusic("background", "THA-emptychasms.mp3", true)
    
    -- load shaders
    resources:addShader("pixelate", "shader/pixelate.glsl")

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

function love.mousepressed(x, y, mb)
    stack:mousepressed(x, y, mb)
end

function love.keypressed(k, u)
    stack:keypressed(k, u)
end

function love.quit()
end
