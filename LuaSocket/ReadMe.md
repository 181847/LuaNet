
# 主要模块函数
* Startup
    - 启动网络环境
    - 接受两个数字，用于调用WSAStartup
* Cleanup
    - 关闭网络环境，调用WSACleanup
* newAddress
    - 传入一个字符串形式的IP地址和数字类型的端口号，创建一个地址对象
* newSocket
    - 创建一个套接字，这个套接字尚未绑定任何地址
* closeSocket
    - 关闭某个套接字
* bind
    - 接受一个套接字对象和地址对象，绑定套接字和地址
* listen
    - 接受一个套接字对象
* accept
    - 接受远端连接，并返回远程主机对应的套接字
* connect
    - 接受一个套接字对象和地址对象，尝试连接地址对象指定的主机
* newNetData
    - 传入一个整形数字，创建对应长度的缓冲区，用于发送网络数据
* send
    - 有连接的发送数据
    - 接受一个套接字对象和NetData对象，发送数据
* recv
    - 有连接的接收数据
    - 接受一个套接字对象和NetData对象，接收的数据将会存储在这个NetData中
* sendto"
    - 无连接的发送数据，使用方法和**send**一样
* recvfrom
    - 无连接的接收数据，使用方法和**recv**一样

# NetData对象
这是用于记录网络数据的对象，其实就是个固定长度的缓冲空间，包含以下方法，使用object : func()的形式进行调用。
* size
    - 返回NetData的缓冲区长度
* toString
    - 将NetData转换为Lua中的字符串并返回，直接将缓冲区域的头指针作为字符串的指针返回。
* fill
    - 传入字符串，使用字符串来填充缓冲区域。

# Socket对象
包含以下方法：
* recvAll
    - 设定当前Socket进入抓包模式，能够监听所有IP数据包
