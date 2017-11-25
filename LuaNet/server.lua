luaSock = require("LuaSocket")

localSocket = assert(luaSock.TCPSocket())
serverAddr = assert(luaSock.newAddress("127.0.0.1", 8000))
assert(luaSock.bind(localSocket, serverAddr))
assert(luaSock.listen(localSocket, 1))

print('waiting for client...')
clientSocket = assert(luaSock.accept(localSocket))
print('one client has come')

function recieveCor()
    local nd = luaSock.newNetData(5)
    while true do
        -- io.write('<<<')
        success, msg, code = assert(luaSock.recv(clientSocket, nd))
        
        --print('###debug success:', success)
        
        if not success then
            print("## error", msg, "Error Code:", code)
            return
        elseif success == 0 then
            print('connection breaked!')
            return
        else
            line = nd:toString()
            
            if line == 'end' then
                return
            else
                io.write(line)
            end -- if
        end -- if
    end -- while
end -- function recieveCor
recieveCor()
assert(luaSock.closeSocket(clientSocket))
assert(luaSock.closeSocket(localSocket))
