# haskell basic syntax
1. 静态类型
2. 惰性
3. 类型推断
## logic operator

1. &&  与
2. ||  或
3. not 非
## compare 比较
1. == 相等,成立返回true

2. /= 不相等，成立返回true

## if
- haskell中if语句的else部分是不可省略
- haskell 中的if语句的另一个特点就是它其实是个表达式  
	**表达式就是返回一个值的一段代码**：5是个表达式，它返回5；4+8是个表达式；x+y也是个表达式，它返 回x+y的结果。正由于else是强制的，if语句一定会返回某个值，所以说if语句也是个表达式。如果要给刚刚定义的函数的结果都加上1
```haskell
doubleSmallNumber x = if x > 100                           
                                     then x                           
                                     else  x*2  // 
doubleSmallNumber' x = (if x > 100 then x else x*2) + 1 // func 101  return 102,去掉括号，返回101
```

## function
因为首字母大写的函数是不允许的

1. 中缀函数  
	用两个参数将它夹在中央.使用``让函数以中缀的方式调用
	
	```
	dav 2 1
	2 `dav` 1
	```
	
2. 前缀函数
    函数名在前，参数在后面
### function call

```haskell
succ 8 //前面事函数名，参数用空格分开
```
### function declaration and definition

- 函数的声明与它的调用形式大体相同，都是先函数名，后跟由空格分隔的参数表。但在声明中一定要在 = 后面定义函数的行为。
```haskell
doubleMe x = x + x 

```
- 无参函数  
	没有参数的函数通常被称作“定义”（或者“名字”），一旦定义，conanO'Brien就与字符串"It's a-me, Conan O'Brien!"完全等价，且它的值不可以修改。
	```
	conanO'Brien = "It's a-me, Conan O'Brien!"
	```
## 调用优先级，priority
函数调用拥有最高的优先级

若要取9乘10的后继，succ 9*10是不行的，程序会先取9的后继，然后再乘以10得100。正确的写法应该是succ(9*10)，得91。如果某函数有两个参数，也可以用 ` 符号将它括起，以中缀函数的形式调用它。例如取两个整数相除所得商的div函数,div 92 10可得9，但这种形式不容易理解：究竟是哪个数是除数，哪个数被除？使用中缀函数的形式 92 `div` 10 就更清晰了。从命令式编程走过来的人们往往会觉得函数调用与括号密不可分，在C中，调用函数必加括号，就像foo(),bar(1),或者baz(3,"haha")。而在haskell中，函数的调用必使用空格，例如bar (bar 3)，它并不表示以bar和3两个参数去调用bar，而是以bar 3所得的结果作为参数去调用bar。在C中，就相当于bar(bar(3))。

```
ghci> succ 9 + max 5 4 + 1   
16   
ghci> (succ 9) + (max 5 4) + 1   
16
```