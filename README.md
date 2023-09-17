# MyWebserver

Learning from 30dayMakeCppServer1.

#### day1

今天学习了客户端和服务器端的实现基础，前段时间只是仅仅看视频学习，发现自己动手写的时候还是相差甚远，本次学习知识：

###### 1、浅析通用地址和专用地址区别：

答： socket通用地址与socket专用地址的区别，通用地址用于bind之后的操作，但是因其需要根据协议族的不同进行位操作所以很麻烦所以我们会用专用地址来进行地址编写，然后强制转化。

###### 2、shell操作：

答：我们若是clr + c的话会杀死进程，若是clr + z只是挂起进程。

###### 3、bind，listen，accept的是否将信息写入内核文件描述符：

答：对的，在一开始socket创建描述符时就是在内核中创建的，返回的是文件描述符，我们可以利用这个文件描述符来去操作内核，这3个函数正好可以对其进行操作。

#### day2

今天学习了IO多路复用的知识。

###### 1、什么是IO多路复用：

答：我们首先得从阻塞I/O和非阻塞I/O谈起，阻塞I/O是当你调用read函数或其他发起进程调用的函数时，如果内部没有消息，这个时候该进程将会挂起（阻塞）这样就不能运行第二第三个进程了，这个时候我们可以用多线程来去解决，但是多线程会造成过多的系统资源浪费，比如多线程的切换会造成一些多余的开销，所以引入非阻塞I/O（不会挂起，每次调用及返回）但是非阻塞智能读取一次,当文件过多时也会造成浪费，这个时候就引入了I/O多路复用：一次系统调用轮询检查多个文件描述符，这样就解决了非阻塞1此系统调用查看1个文件描述符的问题

###### 2、IO多路复用和多线程的对比：

答：多线程：建立销毁线程时需要消耗内存资源以及占用CPU时间，如果要管理多线程，要使用锁，这样会增大编程的复杂难度，I/O多路复用：单线程轮询，降低时间复杂度，将查询交给内核，降低编程难度。

###### 3、epoll和select对比：

答：select：是将所有文件描述符都进行查询看是否活跃，而且储存有限最大1024，而epoll存储是个红黑树的存储结构，利用内核的I/O通知机制，将活跃的文件描述符添加到链表中，我们只需要对链表进行操作这样就降低了时间复杂度。对于select而言，效率是个抛物线增长，所以适用于少量文件描述符，而epoll效率则是一个直线，使用于大量的文件描述符。

###### 4、epoll两种触发模式的特点以及对比：

答：有两种触发模式，LT是水平触发（不断的监听读取），ET是边缘触发（每次只查询一次即返回）所以要用到非阻塞I/O。

###### 5、浅谈epoll的工作原理：

答：而epoll存储是个红黑树的存储结构，利用内核的I/O通知机制，将活跃的文件描述符添加到链表中，我们只需要对链表进行操作这样就降低了时间复杂度。

###### 6、什么是文件描述符：

答：Linux一切皆为文件，不管是socket还是其他文件，如果创建进程，则会有很多文件来表示这个进程，每个文件有一个代号，及文件描述符。

#### day3

今天的代码重构中遇到了很多问题，将C程序封装成C++面向对象的程序，这次重构让我再一次感受到OOP的强大之处，但是也遇到了许多的BUG：

###### 1、epoll_wait：是阻塞的只有当有u客户端连接上服务器时（遇到相应）才会解除阻塞。

###### 2、对文件描述符的理解需要更加深刻。

###### 3、对于重构后的版本，要分清InetAddress不是真正的socket地址，所以在这个转换上我陷入了一个大坑。

###### 4、bind函数如果返回-1的话有可能你的ip或者port写错了（注意下细节hh）。

###### 5、今后需要学习一下对于多线程代码，服务器代码该如何调试。

#### day4

今天我们添加了Channel类，原因：我们不知道接收到的文件描述符具体是什么协议操作，有可能是http,有可能是ftp，不同的连接类型处理不同的实物。所以但仅仅从文件描述符不能判断到底需要怎样处理
所以我们用Channel类来代替fd,其中包括了：1.希望监听这个文件描述符的哪些事件（创建时用）。2.epoll返回该Channel时文件描述符正在发生的事件（接收时用）。
（具体原理还不是特别明了，我们在day5的学习中再好好研究。）

###### 1、前向声明：用于解决环状头文件引入。
