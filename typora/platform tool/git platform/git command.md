Git pull --rebase 会将自己的代码放在最上面

回退：git reset --soft HEAD^

打最新的补丁：git format-patch HEAD^

查看某文件路径下的Git提交记录：git log --all --full-history ./

切换分支：git checkout xxx 

git switch xxx

查看分支： git branch -a

查看当前分支： git branch 

注意：make 型号并不会复制feeds.config 文件，需要手动复制该文件。

可以通过:git log /git config --list查看当前分支对应的远程分支

若当前组件分支切不了，需要将feeds文件删除，然后讲feeds.config文件删除并且重新进行make rg_pre进行重新下载。

git commit --amend 

Pick 表示合并，f表示不合并

 git log -p -30 .config 查看配置宏的更改记录