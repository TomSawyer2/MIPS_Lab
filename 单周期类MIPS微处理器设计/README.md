# 单周期类MIPS微处理器设计

## 运行

直接把整个文件夹内的所有文件（除了`/utils`文件夹）在Vivado中打开即可，然后要对`irom_IP`模块进行IP_Catalog处理，具体处理方法参考学习通，同时将`irom_IP.v`文件内的`locate`地址替换为`/utils`文件夹下的`machine_code.coe`的地址 **（注意：路径不能有中文名）**

## Q&A

Q:`name.tcl`文件干啥用的？
A:生成比特流的时候用的，如果不烧到板子上没用

Q:`machine_code.coe`文件怎么生成？
A:在生成机器码后直接按照格式填写机器码就可以，具体格式如下，注意机器码的大小顺序，别填反了

```coe
memory_initialization_radix=16;
memory_initialization_vector=机器码1 机器码2 机器码3;
```

## 参考

- 学习通相关内容
- [类MIPS单周期微处理器设计（单周期CPU）](https://blog.csdn.net/antonioxv/article/details/116379458)