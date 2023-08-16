# CodeOnly

Bad Code:

```
printf("%s", hello);
printf("%s", world);
```

Good Code:
```
[root@ke-cloud ~]# kubectl get node
NAME       STATUS   ROLES        AGE   VERSION
ke-cloud   Ready    master       13h   v1.20.2
ke-edge1   Ready    agent,edge   64s   v1.19.3-kubeedge-v1.8.0
说明：本文接下来的验证将使用边缘节点ke-edge1进行，如果你参考本文进行相关验证，后续边缘节点名称的配置需要根据你的实际情况进行更改。
```


**1）本示例要求KubeEdge版本必须是v1.2.1+，此次选择最新版的KubeEdge v1.8.0**

```
[root@ke-cloud ~]# kubectl get node
NAME       STATUS   ROLES        AGE   VERSION
ke-cloud   Ready    master       13h   v1.20.2
ke-edge1   Ready    agent,edge   64s   v1.19.3-kubeedge-v1.8.0
说明：本文接下来的验证将使用边缘节点ke-edge1进行，如果你参考本文进行相关验证，后续边缘节点名称的配置需要根据你的实际情况进行更改。
```

这些函数对于文件不需要读写权限，只需要最基本的可执行权限：`---x--x--x`。
## 区别
+ `lstat`：和`stat`不同之处在于如果`pathname`指向的一个符号连接，那么`lstat`返回的就是符号连接本身，而不是其引用的文件。
+ `fstat`：和`stat`不同之处在于获取文件描述符`fd`的状态信息。

超链接：[#获取文件状态函数](#获取文件状态函数)
图片：

![MOVIE](Movie.png)
