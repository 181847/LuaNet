LuaSocket = require("LuaSocket")
LuaProcess = require("LuaProcess")

assert(LuaSocket.Startup(2, 2))

nt = LuaSocket.newNetData(45)
PrintTable(getmetatable(nt))
print("sizs:", nt:size())
print("tostring:", nt:toString())
assert(nt:fill("abc"))
print("after filling")
print("tostring:", nt:toString())

assert(LuaSocket.Cleanup())
