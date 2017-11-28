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

PrintTable(LuaSocket.Family);
PrintTable(LuaSocket.Type);
PrintTable(LuaSocket.Protocal);

function LuaSocket.TCPSocket()
    return assert(
        LuaSocket.newSocket(LuaSocket.Family.IPV4, 
                            LuaSocket.Type.TCP,
                            LuaSocket.Protocal.TCP))
end

function LuaSocket.UDPSocket()
        return assert(
            LuaSocket.newSocket(LuaSocket.Family.IPV4, 
                                LuaSocket.Type.UDP,
                                LuaSocket.Protocal.UDP))
end

tsock = LuaSocket.TCPSocket()
usock = LuaSocket.UDPSocket()

print('\ntry to make sockets')
print('tcp sock', tsock)
print('udp socke', usock)

print('\ncreate two socket and test')
usock1 = assert(LuaSocket.UDPSocket())
usock2 = assert(LuaSocket.UDPSocket())

uaddress1 = assert(LuaSocket.newAddress("127.0.0.1", 8100))
uaddress2 = assert(LuaSocket.newAddress("127.0.0.1", 8101))

assert(LuaSocket.bind(usock1, uaddress1))
assert(LuaSocket.bind(usock2, uaddress2))

gmtx = assert(LuaProcess.newMutex())
-- waitmtx = assert(LuaProcess.newMutex())

gmtx:lock()
-- use another thread to receive data
anotherThread = assert(LuaProcess.newThread(
    function()
        assert(pcall( function()
            gmtx:lock()
            
            print('\n###### anotherThread take over');
            rData = LuaSocket.newNetData(2048);
            
            while true do
                success, msg, code = LuaSocket.recvfrom(usock2, rData, uaddress1)
            
                if not success then
                    print(success, msg, code);
                else
                    print('receive success')
                    print('length:', success)
                    s = rData:toString()
                    print('message:', 'stringlength', string.len(s), s)
                    
                    return
                end -- else
            end -- while
            gmtx:unlock()
        end )) -- assert (pcall( function(()
    end -- function
    )) -- end thread assert

    
anotherThread.SysThread:resume()
gmtx:unlock()

print('usock1 send message')
sData = LuaSocket.newNetData("can you see me?")
success, msg, code = LuaSocket.sendto(usock1, sData, uaddress2)

if not success then
    print(success, msg, code);
else
    print('send success')
    print('length:', success)
end

print('\n start another thread')


assert(LuaSocket.Cleanup())

return true
