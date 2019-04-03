# 编译器优化
## RVO (return value optimization) 和NRVO (named return value optimization)
是C++在处理一个函数返回类对象并将返回值赋给另一个对象时，为了减少拷贝构造次数以及析构次数而采用的一种编译器优化技术。 
