# linux
## gcc
### 链接库
- -L  
	-L指定的是链接时的库路径
- LD_LIBRARY_PATH环境变量  
	export LD_LIBRARY_PATH=/home/liyihai/documents  
	生成的可执行文件在运行时库的路径由LD_LIBRARY_PATH环境变量指定
- -rpath
	运行时库查找路径设置。这路径被写入到生成目标文件中  
	并非指定-rpath参数后，就抛弃LD_LIBRARY_PATH环境变量，只是多了个可选的依赖库路径而已。
- -rpath-link
	-rpath和-rpath-link都可以在链接时指定库的路径；但是运行可执行文件时，-rpath-link指定的路径就不再有效(链接器没有将库的路径包含进可执行文件中)，而-rpath指定的路径还有效(因为链接器已经将库的路径包含在可执行文件中了。)