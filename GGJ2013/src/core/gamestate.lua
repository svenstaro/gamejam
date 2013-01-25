-- game state

require("core/helper")

GameState = class("GameState")

GameState.drawBelow = false

function GameState:__init()end

function GameState:update(dt)end
function GameState:draw()end
function GameState:start()end
function GameState:stop()end
function GameState:keypressed(k, u)end
function GameState:keyreleased(k, u)end
function GameState:mousepressed(x, y, button)end
function GameState:mousereleased(x, y, button)end
