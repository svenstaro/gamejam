love = {}
require "src/conf"
conf = {}
conf.modules = {}
conf.screen = {}
love.conf(conf)
print(conf.author .. "-" .. conf.identity)
