# head
## List
```
[4,8,15,16,23,48]
[[1,2,3,4],[5,3,3,3],[1,2,2,3,4],[1,2,3]]
```
- list中的元素类型要一样
- List中的List可以是不同长度，但必须得是相同的类型。
- 
### 字符串
- 字符串实际上就是一组字符的List，"Hello"只是['h','e','l','l','o']的语法糖而已

### common operator
- 连接  
    ```
    ghci> [1,2,3,4] ++ [9,10,11,12]    
    [1,2,3,4,9,10,11,12]   
    ghci> "hello" ++ " " ++ "world"   
    "hello world"   
    ghci> ['w','o'] ++ ['o','t']   
    "woot"
    ```
	> 在使用++运算符处理长字符串时要格外小心(对长List也是同样)，Haskell会遍历整个的List(++符号左边的那个)在处理较短的字符串时问题还不大

- 插入  
    ```
    ghci> 'A':" SMALL CAT"   
    "A SMALL CAT"   
    ghci> 5:[1,2,3,4,5]  
    [5,1,2,3,4,5]
    ```
    > :运算符可以连接一个元素到一个List或者字符串之中.[1,2,3]实际上是1:2:3:[]的语法糖。[]表示一个空List,若要从前端插入3，它就成了[3],再插入2，它就成了[2,3]，以此类推。

- 访问，下标运算  
	> 若是要按照索引取得List中的元素，可以使用!!运算符，索引的下标为0。
    ```
    ghci> "Steve Buscemi" !! 6   
    'B'   
    ghci> [9.4,33.2,96.2,11.2,23.25] !! 1   
    33.2
    ```
- 比较，compare  
	> 当List内装有可比较的元素时，使用 > 和 >=可以比较List的大小。它会先比较第一个元素，若它们的值相等，则比较下一个，以此类推
	```
    ghci> [3,2,1] > [2,1,0]   
    True   
    ghci> [3,2,1] > [2,10,100]   
    True   
    ghci> [3,4,2] > [3,4]   
    True   
    ghci> [3,4,2] > [2,4]   
    True   
    ghci> [3,4,2] == [3,4,2]   
    True 
  ```
- common function  
	1. head,head返回一个List的头部，也就是List的首个元素
	2. tail,tail返回一个LIst的尾部，也就是List除去头部之后的部分。
	3. last,last返回一个LIst的最后一个元素。
	4. init,init返回一个LIst除去最后一个元素的部分。
	> 上面这些函数遇到[]空LIST运行时会报错，编译能通过
	5. length,length返回一个List的长度
	6. null,null检查一个List是否为空。如果是，则返回True，否则返回False。应当避免使用xs==[]之类的语句来判断List是否为空，使用null会更好。
	7. reverse,reverse将一个List反转
	8. take,take返回一个List的前几个元素.若是图取超过List长度的元素个数，只能得到原List。若take 0个元素，则会得到一个空List  
		```
		take 3 [1,2,3]
		```
	9. drop,drop与take的用法大体相同，它会删除一个List中的前几个元素。
	10. manximum和minimum,maximum返回一个List中最大的那个元素。minimum返回最小的
	11. sum返回一个List中所有元素的和。product返回一个List中所有元素的积。
	12. elem判断一个元素是否在包含于一个List，通常以中缀函数的形式调用它  
	```
	ghci> 4 `elem` [3,4,5,6]   
    True   
    ghci> 10 `elem` [3,4,5,6]   
    False
	```