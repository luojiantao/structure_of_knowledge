# 函数式语言
没有深入研究过，只学了一点点的haskell
## 特点
1. 所有都是函数（可以调用）
2. 调用同一个函数，无论什么情况下相同的输入由相同的输出
3. 函数可以组成函数（递归）
4. 惰性执行，只有在需要结果的时候去执行函数（其他语言也可以模拟，但是不是天然支持）
## haskell
1. 模式匹配
2. 递归
3. 惰性执行
## example
1. func1(a, b), func2(c,d)
	func3 = func1 . fun2 c d
	func3 是一个新的函数 需要入参 b
	
# 和面对对象的区别
1. 对象语言，对象主体是数据集合和方法，通过方法（函数）改变数据集合。就是改变对象的状态
2. 函数式，主体是函数，通过过函数的组合返回一个你想要的函数（结果），在真正要使用这个函数的时候，去执行上面的函数组合。一些基本类型，也是一种函数，可以被调用。世界观是，关注执行方法和步骤，不关注函数入参（数据本身）
3. 