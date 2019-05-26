# haskell basic syntax
1. 静态类型
2. 惰性
3. 类型推断

## 类型和类型类
可以使用ghci来检测表达式的类型。使用:t命令后跟任何可用的表达式，即可得到该表达式的类型
```
ghci> :t 'a'   
'a' :: Char   
ghci> :t True   
True :: Bool   
ghci> :t "HELLO!"   
"HELLO!" :: [Char]   
ghci> :t (True, 'a')   
(True, 'a') :: (Bool, Char)   
ghci> :t 4 == 5   
4 == 5 :: Bool
```
以看出，:t命令处理一个表达式的输出结果为表达式后跟::及其类型，::读作“它的类型为”。凡是明确的类型，其首字母必为大写。'a',如它的样子，是Char类型，易知是个字符（character）。True是Bool类型，也靠谱。不过这又是啥，检测"hello"得一个[Char]？这方括号表示一个List，所以我们可以将其读作“一组字符的List”。而与List不同，每个Tuple都是独立的类型，于是(True,"a")的类型是(Bool,Char)，而('a','b','c')的类型为(Char,Char,Char)。4==5一定返回 False，所以它的类型为Bool
### 函数类型声明，function type define
```
removeNonUppercase :: [Char] -> [Char]   
removeNonUppercase st = [ c | c  st, c `elem` ['A'..'Z']]  
```
removeNonUppercase的类型为[Char]->[Char]，从它的参数和返回值的类型上可以看出，它将一个字符串映射为另一个字符串。[Char]与String是等价的，但使用String会更清晰：removeNonUppercase :: String -> String。编译器会自动检测出它的类型，我们还是标明了它的类型声明。要是多个参数的函数该怎样？如下便是一个将三个整数相加的简单函数
```
addThree :: Int -> Int -> Int -> Int   
addThree x y z = x + y + z
```
### 常见类型
1. Int表示整数。7可以是Int，但7.2不可以。Int是有界的，也就是说它由上限和下限。对32位的机器而言，上限一般是214748364，下限是-214748364。
2. Integer表示...厄...也是整数，但它是无界的。这就意味着可以用它存放非常非常大的数，我是说非常大。它的效率不如Int高。
3. Float表示单精度的浮点数
4. Double表示双精度的浮点数
5. Bool表示布尔值，它只有两种值：True和False。
6. Char表示一个字符。一个字符由单引号括起，一组字符的List即为字符串。
7. Tuple的类型取决于它的长度及其中项的类型。注意，空Tuple同样也是个类型，它只有一种值：()。
### 类型变量
```
ghci> :t head   
head :: [a] -> a
```
a 为类型变量。head函数的类型声明里标明了它可以取任意类型的List并返回其中的第一个元素。
在命名上，类型变量使用多个字符是合法的，不过约定俗成，通常都是使用单个字符，如a,b,c,d...
### 类型类
```
ghci> :t (==)   
(==) :: (Eq a) => a -> a -> Bool
```
在这里我们见到个新东西：=>符号。它左边的部分叫做类型约束。我们可以这样阅读这段类型声明：“相等函数取两个相同类型的值作为参数并返回一个布尔值，而这两个参数的类型同在Eq类之中（即类型约束）”
> 基本类型类
- Eq这一类型类提供了判断相等性的接口，凡是可比较相等性的类型必属于Eq类。
- Eq包含可判断相等性的类型。提供实现的函数是==和/=。所以，只要一个函数有Eq类的类型限制，那么它就必定在定义中用到了==和/=。刚才说了，除函数意外的所有类型都属于Eq，所以它们都可以判断相等性。
- Ord包含可比较大小的类型。除了函数以外，我们目前所谈到的所有类型都属于Ord类。Ord包中包含了,=之类用于比较大小的函数。compare函数取两个Ord类中的相同类型的值作参数，返回比较的结果。这个结果是如下三种类型之一：GT,LT,EQ。
	```
	ghci> "Abrakadabra" < "Zebra"   
    True   
    ghci> "Abrakadabra" `compare` "Zebra"   
    LT   
    ghci> 5 >= 2   
    True   
    ghci> 5 `compare` 3   
    GT
	```
- Show的成员为可用字符串表示的类型。目前为止，除函数以外的所有类型都是Show的成员。操作Show类型类，最常用的函数表示show。它可以取任一Show的成员类型并将其转为字符串。
- Read是与Show相反的类型类。read函数可以将一个字符串转为Read的某成员类型
	```
	ghci> read "5" :: Int   
    5   
    ghci> read "5" :: Float   
    5.0   
    ghci> (read "5" :: Float) * 4   
    20.0   
    ghci> read "[1,2,3,4]" :: [Int]   
    [1,2,3,4]   
    ghci> read "(3, 'a')" :: (Int, Char)   
    (3, 'a')
	```
	编译器可以辨认出大部分表达式的类型，但遇到read "5"的时候它就搞不清楚究竟该是Int还是Float了。只有经过运算，haskell才会明确其类型；同时由于haskell是静态的，它还必须得在 编译前搞清楚所有值的类型。所以我们就最好提前给它打声招呼：“嘿，这个表达式应该是这个类型，省的你认不出来！”
- enum，Enum的成员都是连续的类型--也就是可枚举。Enum类存在的主要好处就在于我们可以在Range中用到它的成员类型：每个值都有后继子(successer)和前置子(predecesor)，分别可以通过succ函数和pred函数得到。该类型类包含的类型有：(),Bool,Char,Ordering,Int,Integer,Float和Double。
	```
	ghci> ['a'..'e']   
    "abcde"   
    ghci> [LT .. GT]   
    [LT,EQ,GT]   
    ghci> [3 .. 5]   
    [3,4,5]   
    ghci> succ 'B'   
    'C'	
	```
- Bounded的成员都有一个上限和下限
	```
	ghci> minBound :: Int   
    -2147483648   
    ghci> maxBound :: Char   
    '\1114111'   
    ghci> maxBound :: Bool   
    True   
    ghci> minBound :: Bool   
    False
	```
	minBound和maxBound函数很有趣，它们的类型都是(Bounded a) => a。可以说，它们都是多态常量。

	如果其中的项都属于Bounded类型类
	
## NUM
Num是表示数字的类型类，它的成员类型都具有数字的特征。检查一个数字的类型
```
ghci> :t 20   
20 :: (Num t) => t
```
看样子所有的数字都是多态常量，它可以作为所有Num类型类中的成员类型。以上便是Num类型类中包含的所有类型，检测*运算符的类型，可以发现它可以处理一切的数字
```
ghci> :t (*)   
(*) :: (Num a) => a -> a -> a
```
- 它只取两个相同类型的参数。所以(5 :: Int) * (6 :: Integer)会引发一个类型错误，而5 * (6 :: Integer)就不会有问题
- 类型只有亲近Show和Eq，才可以加入Num。
- Integral同样是表示数字的类型类。Num包含所有的数字：实数和整数。而Intgral仅包含整数，其中的成员类型有Int和Integer
- Floating仅包含浮点类型：Float和Double。

有个函数在处理数字时会非常有用，它便是fromIntegral。其类型声明为：fromIntegral :: (Num b, Integral a) => a -> b。从中可以看出，它取一个整数做参数并返回一个更加通用的数字，这在同时处理整数和浮点时会尤为有用。举例来说，length函数的类型声明为：length :: [a] -> Int，而非更通用的形式，如(Num b) => length :: [a] -> b。这应该时历史原因吧，反正我觉得挺蠢。如果取了一个List长度的值再给它加3.2就会报错，因为这是将浮点数和整数相加。面对这种情况，我们就用fromIntegral (length [1,2,3,4]) + 3.2来解决。

注意到，`fromIntegral`的类型声明中用到了多个类型约束。如你所见，只要将多个类型约束放到括号里用逗号隔开即可。

## logic operator

1. &&  与
2. ||  或
3. not 非
## compare 比较
1. == 相等,成立返回true

### 函数类型声明，function type define
```
removeNonUppercase st = [ c | c  st, c `elem` ['A'..'Z']]

```
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
## varialbe
### definition
在ghci下执行let a =1与在脚本中编写a=1是等价的
```
ghci> let lostNumbers = [4,8,15,16,23,48]   
ghci> lostNumbers    
[4,8,15,16,23,48]
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