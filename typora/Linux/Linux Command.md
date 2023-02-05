1.apt-file工具  
		简述：搜索APT包的工具  
		常用方法：  
				①sudo apt-get install apt-file   
				②apt-file update  
				③若缺少文件依赖：apt-file search xxxx.so  
					若缺少包：apt-file search libxxx or xxx  
				④找到后，选择正确的：apt install libxxx-dev

