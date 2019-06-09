# 基础类
1. package 目录中查找   方法名
```python
import sublime
import sublime_plugin


class ExampleCommand(sublime_plugin.TextCommand):
	def run(self, edit):
		self.view.insert(edit, 0, "Hello, World!")
```
view.run_command("example")
## 注意
ExampleCommand类名字改为你想要的插件名字，如： definereplaceCommand(将Command之前的名称先全部小写，当然还有其他方式)，然后就可以开发该插件对应的功能了,当然这时候输入的命令也变成了view.run_command('definereplace'),这样执行才有效。

注意:command命令的名称是根据大写字符进行拆分的，如果定义的是class DefineRePlaceCommand,那么输入的命令是view.run_command('define_re_place'),拆分出来的单词需要全部小写，中间以下划线相连接。
## api接口
[http://www.sublimetext.com/docs/3/api_reference.html](http://www.sublimetext.com/docs/3/api_reference.html)