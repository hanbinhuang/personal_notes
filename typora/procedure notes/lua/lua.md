

1.判断文件是否存在

```lua
local function file_exit(path)
    debug("***************Enter file_exit()**********************")

    local f = io.open(path,"rb")
    if (f) then
      f:close()
      return 0
    end

    debug("the file [%s] isn't exist.",path)
    return -1
end
```



2.执行系统命令

```lua
local function my_exec(cmd)
    debug("***************Enter my_exec()**********************")
    
    local ret
    if (cmd and cmd:len() > 0) then
        ret = os.execute(cmd)
        if (ret == 0)  then
            debug(string.format("ret [%s],cmd [%s] execute successfully!",ret,cmd))
            return 0
        else
            debug(string.format("ret [%s],cmd [%s] execute failure!",ret,cmd))
            return ret
        end
    end
end
```

