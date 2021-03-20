##XMPP介绍

#### XMPP协议

------

XMPP（ExtensibleMessageing and Presence Protocol：可扩展消息与存在协议）是目前主流的四种IM（IM：instantmessaging,即时消息）协议之一，其他三种分别为：即时信息和空间协议(IMPP)、空间和即时信息协议(PRIM)、针对即时通讯和空间平衡扩充的进程开始协议SIP(SIMPLE)。在这四种协议中，XMPP是最灵活的。XMPP是一种基于XML的协议，它继承了在XML环境中灵活的发展性。因此，基于XMPP的应用具有超强的可扩展性。经过扩展以后的XMPP可以通过发送扩展的信息来处理用户的需求，以及在XMPP的顶端建立如内容发布系统和基于地址的服务等应用程序。而且，XMPP包含了针对服务器端的软件协议，使之能与另一个进行通话，这使得开发者更容易建立客户应用程序或给一个配好系统添加功能。



#### XMPP网络

------

   XMPP中定义了三个角色，客户端，服务器，网关。通信能够在这三者的任意两个之间双向发生。服务器同时承担了客户端信息记录，连接管理和信息的路由功能。网关承担着与异构即时通信系统的互联互通，异构系统可以包括SMS（短信），MSN，ICQ，Gtalk等。基本的网络形式是单客户端通过TCP/IP连接到单服务器，然后在通道上传输XML。




Openfire

Smack

SASL：Simple Authentication and Security Layer



`*Openfire是基于XMPP(Jabber)的开源实时协作(RTC)协议。Openfire安装和使用都非常简单，并利用Web进行管理。单台服务器可支持上万并发用户。Openfire是用Java语言编写的，并且支持插件开发，如：查找联系人的服务就是以插件的形式提供的*`



#### Openfire防火墙配置

------------------

systemctl stop firewalld

-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 9090 -j ACCEPT
-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 9091 -j ACCEPT

`chkconfig iptables on` not `service iptables save`

systemctl start iptables

####资料

------

- [官网]("https://xmpp.org/")
- [XMPP中文]("[http://wiki.jabbercn.org/%E9%A6%96%E9%A1%B5](http://wiki.jabbercn.org/首页)")
- [XMPP基本概念]("https://www.jianshu.com/p/a94749385755")
- [Openfire官网]("https://www.igniterealtime.org/projects/openfire/index.jsp")
- [Openfire Github地址]("https://github.com/igniterealtime/Openfire")
- [Openfire中文教程]("http://myopenfire.com/article/getarticle/3")
- [Smack]("https://github.com/igniterealtime/Smack")

Tvc4Dpa