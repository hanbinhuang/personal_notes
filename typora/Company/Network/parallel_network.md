1.获取并联邻居信息：  
			`dev_sta get -m "parallelNeighborPair" |jq .`  
			实际上就是调用：`ubus -t 2 call opendisc show_neighbor`（parallel_neighbors.lua）



2.设置并联组网

dev_sta set -m parallelNeighborPair '{"paralleltype":"PARALLEL","
list":[{"sn":"MACCEW3000S01"},{"sn":"MACCEW3000A01"}]}'



![image-20230308171150851](C:\Users\acer\AppData\Roaming\Typora\typora-user-images\image-20230308171150851.png)



