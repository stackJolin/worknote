## work list

- bank需求
- csc-mac客服头像本地化
- csc-mac强制更新
  - 



#### 记录

------

###### zip压缩命令问题

zip命令`zip -r CSC.zip CSC`默认会将CSC目录下所有的软连接/符号替换成真实内容，如果不想被替换的话加参数'y'，`zip -ry CSC.zip CSC`



###### Sparkle缓存问题

Sparkle默认会将指定路径下的包，copy一份到cache里面，然后所有的操作都会在这个cache里面进行。所以，他对这个文件要求很严格。一般不希望我们引入不正确的东西。如果引入了他不需要的文件，会报错。并且每次编译都不会清理这个文件。导致，接下来的generate_appcast一直报错，永远不会成功

