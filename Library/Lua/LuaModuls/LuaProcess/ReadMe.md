## LuaProcess
相关函数：
函数名 |   解释
------|-------
newThread  | 创建一个新的线程，默认不立即执行，第一个参数为function,返回一个table，里面有两个域，一个Thread对应Lua中的线程对象，一个ThreadInfo对应Userdata，作为resume函数的参数。
newMutex  | 生成一个线程锁

## newThread
newThread返回一个table，table中包含两个域
域   |   解释
---|----
LuaThread   |   Lua中的线程对象  
SysThread   |   系统中的线程对象,拥有一个resume()成员函数，调用之后开始执行线程。

## newMutex
返回一个userdata，mutex
方法  |   使用
-----|-----
lock  |    锁住线程，传递一个整数来等待具体的毫秒时间，如果不传参表示等待无限时间
unlock  |  释放锁  
