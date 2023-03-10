#!/usr/bin/env lua
--项目名
package.path = "/usr/lib/lua/?.lua;" .. package.path;
module("pppoeCheck", package.seeall);
cjson = require "dkjson"
uci = require("uci")
ubus = require "ubus"
tool = require "dev.tools.common"
lib_debug = require "utils.debug"
console = require "console"

lib_debug.debug_init("/tmp/debug/rp-pppoe/", "rp-pppoe_check.log", 300, lib_debug.INFO, "1");

local g_pppoe_path          = "/tmp/debug/rp-pppoe"
local g_check_file          = "/tmp/debug/rp-pppoe/pppoe_check_C"
local PPPOE_CHECK_LOCK_FILE = "/tmp/debug/rp-pppoe/pppoe_check_single_lock"
local g_pppoe_success_file  = "/tmp/pppoe_success_"
local g_timeout = 20

local g_pppoe_err           = {"pppoe param err"               , "参数错误"             , "-1"}
local g_pppoe_checking      = {"pppoe checking"                , "已有账号在检测中"      , "0"}
local g_disc_failed         = {"discovery failed"              , "未查找到pppoe服务器"   , "1"}
local g_auth_success        = {"account authentication passed" ,"账号认证成功"           , "3"}
local g_auth_failed         = {"account authentication failed" ,"账号认证失败"           , "4"}
local g_ipcp_success        = {"ipcp success"                  , "ip协商成功"            , "5"}
local g_ipcp_failed         = {"ipcp failed"                   , "ip协商失败"            , "6"}
local g_pppoe_check         = {"pppoe拨号成功"                  , "pppoe拨号失败"         , "7"}
local g_time_expired        = {"connect time expired"          ,"连接时间过期"           , "8"}
local g_conn_terminat       = {"connection terminated"         ,"连接终止"               , "9"}
local g_fail_disconn        = {"failed to disconnect pppoe socket" ,"中断连接失败"       , "10"}
local g_pppoe_disconnect    = {"disconnect device"              ,"断开pppoe服务器"       , "11"}
local g_pppoe_connect       = {"connect device"                 ,"开始连接pppoe服务器"    , "12"}
local g_check_timeout       = {"connect timeout"                ,"拨号超时"              , "13"}
local g_intf_nofind         = {"intf no find"                   ,"接口不存在"            , "14"}

--读取指定路径文件并返回文件内容
function file_read(conf_file)
    local file = io.open(conf_file, "r")
    local data = file:read("*a")
    file:close()
    return data
end

--判断文件是否存在,false=不存在，true=存在
function file_exists(file_path)
    local file=io.open(file_path,"r")
    if (file ~= nil) then
        io.close(file)
        return true
    else
        return false
    end
end

--单实例检查
local function single_instance_check(input_data, lockfile)
    local input_json = cjson.decode(input_data)

    --防止死锁
    if (tool.file_exist(lockfile) == true) then
        local lfs = require "lfs"
        local attr = lfs.attributes(lockfile)
        assert (type(attr) == "table")
        local change_time = tonumber(attr.change);
        local current_time = tonumber(os.time());
        local lifetime = current_time - change_time
        if (lifetime > 120) then
            os.remove(lockfile);
        end
    end

    if (tool.file_exist(lockfile) == true) then
        lib_debug.lua_debug(lib_debug.DEBUG, "Error: There is instance process running");
        tool.file_write(IPTV_JSON_FILE, input_data, "w");
        return false;
    else
        lib_debug.lua_debug(lib_debug.DEBUG, "ok: There no instance process running");
        tool.file_write(lockfile, "running", "w");
        return true;
    end
end

--[[
    dev_sta set -m pppoeCheck '{"intf_name": "wan","proto": "pppoe","mtu": "1480","macaddr": "f0:74:8d:00:47:54","password": "768999","username": "0591917886788","service": "ruijie"}'
返回值：
    {
    "code": [
        "1"
    ],
    "data": [
        "查找到pppoe服务器"
    ],
    "res": "failed/success/checking/none"
]]
function module_set(param)
    lib_debug.lua_debug(lib_debug.DEBUG, "************** start pppoeCheck **************\n")
    local result = {}
    result["data"] = {}
    result["code"] = {}
    local data
    local ret

    --入参判空
    if (param == nil) then
        lib_debug.lua_debug(lib_debug.ERROR, "Input is empty.");
        table.insert(result["data"], g_pppoe_err[2])
        table.insert(result["code"], g_pppoe_err[3])
        result["res"] = "failed"
        return cjson.encode(result)
    end
    --参数判错
    data = cjson.decode(param)
    if (data == nil or data == "") then
        lib_debug.lua_debug(lib_debug.ERROR, "Input param is not JSON format.");
        table.insert(result["data"], g_pppoe_err[2])
        table.insert(result["code"], g_pppoe_err[3])
        result["res"] = "failed"
        return cjson.encode(result)
    end

    --单实例检测
    ret = single_instance_check(param, PPPOE_CHECK_LOCK_FILE)
    if (ret == false) then
        table.insert(result["data"], g_pppoe_checking[2])
        table.insert(result["code"], g_pppoe_checking[3])
        result["res"] = "failed"
        return cjson.encode(result)
    end

    --检查数据
    if (data["intf_name"] ~= nil) then
        lib_debug.lua_debug(lib_debug.INFO, "intf_name:%s\n", data["intf_name"])
        if (data["intf_name"] == "wan") then
            data["ifname"] = "eth0.1"
        elseif (data["intf_name"] == "wan1") then
            data["ifname"] = "eth0.4094"
        elseif (data["intf_name"] == "wan2") then
            data["ifname"] = "eth0.4093"
        elseif (data["intf_name"] == "wan3") then
            data["ifname"] = "eth0.4092"
        else
            table.insert(result["data"], g_pppoe_err[2])
            table.insert(result["code"], g_pppoe_err[3])
            result["res"] = "failed"
            lib_debug.lua_debug(lib_debug.ERROR, "Parameter error\n")
            return cjson.encode(result)
        end
        lib_debug.lua_debug(lib_debug.INFO, "ifname:%s\n", data["ifname"])
    else
        table.insert(result["data"], g_pppoe_err[2])
        table.insert(result["code"], g_pppoe_err[3])
        result["res"] = "failed"
        lib_debug.lua_debug(lib_debug.ERROR, "Parameter error\n")
        return cjson.encode(result)
    end
    if (data["username"] == nil or data["password"] == nil) then
        table.insert(result["data"], g_pppoe_err[2])
        table.insert(result["code"], g_pppoe_err[3])
        result["res"] = "failed"
        lib_debug.lua_debug(lib_debug.ERROR, "Parameter error\n")
        return cjson.encode(result)
    else
        if (data["username"] ~= nil) then
            lib_debug.lua_debug(lib_debug.INFO, "username:%s\n", data["username"])
        end
        if (data["password"] ~= nil) then
            lib_debug.lua_debug(lib_debug.INFO, "password:%s\n", data["password"])
        end
    end
    if (data["service"] ~= nil) then
        lib_debug.lua_debug(lib_debug.INFO, "service:%s\n", data["service"])
    else
        table.insert(result["data"], g_pppoe_err[2])
        table.insert(result["code"], g_pppoe_err[3])
        result["res"] = "failed"
        lib_debug.lua_debug(lib_debug.ERROR, "Parameter error\n")
        return cjson.encode(result)
    end
    if (data["mtu"] ~= nil) then
        lib_debug.lua_debug(lib_debug.INFO, "mtu:%s\n", data["mtu"])
    else
        data["mtu"] = "1492"
    end
    if (data["metric"] ~= nil) then
        lib_debug.lua_debug(lib_debug.INFO, "metric:%s\n", data["metric"])
    else
        data["metric"] = "0"
    end

    console.forkExec("rm "..g_pppoe_success_file..data["intf_name"])
    lib_debug.lua_debug(lib_debug.INFO, "cmd: %s\n", "rm "..g_pppoe_success_file..data["intf_name"])
    --获取对应wan口协议
    local x = uci.cursor()
    local proto = x:get("network",data["intf_name"],"proto")
    if (proto == nil) then
        table.insert(result["data"], g_pppoe_err[2])
        table.insert(result["code"], g_pppoe_err[3])
        result["res"] = "failed"
        lib_debug.lua_debug(lib_debug.ERROR, "no find intf:%s\n", data["intf_name"])
        return cjson.encode(result)
    end
    --判断pppoe逻辑口是否起来
    local link_cmd = "ifconfig |grep pppoe-"..data["intf_name"]
    local rsfile = io.popen(link_cmd)
    local link_s = rsfile:read("*all")
    if (link_s == "") then
        lib_debug.lua_debug(lib_debug.DEBUG, "%s is unlink\n", data["intf_name"])
    else
        lib_debug.lua_debug(lib_debug.DEBUG, "%s is link\n", data["intf_name"])
    end

    --sh /lib/netifd/proto/ppp.sh pppoe setup wan '{"ifname":"br-wan","ifname":"eth0.1","proto":"pppoe","metric":0,"username":"111","password":"111","ppp_mtu":1492}' br-wan
    --组装数据
    local logfile = g_pppoe_path.."/".."C"..data["intf_name"]..".log"
    local cmd_down = "ps |grep /usr/sbin/pppd |grep -v grep|awk '{print $1}'| xargs kill >/dev/null 2>&1"
    local cmd_teardown = "sh /lib/netifd/proto/ppp.sh pppoe teardown "..data["intf_name"].." '{\"ifname\":\"br-"..data["intf_name"].."\",\"ifname\":\""..data["ifname"].."\",\"proto\":\"pppoe\",\"metric\":"..data["metric"]..",\"username\":\""..data["username"].."\",\"password\":\""..data["password"].."\",\"ppp_mtu\":"..data["mtu"].."}' br-"..data["intf_name"].." >/dev/null 2>&1"
    local cmd_up = "/usr/sbin/pppd nodetach ipparam "..data["intf_name"].." ifname pppoe-"..data["intf_name"].." +ipv6 nodefaultroute usepeerdns persist maxfail 1 user "..data["username"].." password "..data["password"].." ip-up-script /lib/netifd/ppp-up ipv6-up-script /lib/netifd/ppp-up ip-down-script /lib/netifd/ppp-down ipv6-down-script /lib/netifd/ppp-down mtu "..data["mtu"].." mru 1492 plugin rp-pppoe.so nic-br-wan logfile "..logfile.."  >/dev/null 2>&1 &"
    lib_debug.lua_debug(lib_debug.INFO, "cmd_teardown:%s\n", cmd_teardown)
    lib_debug.lua_debug(lib_debug.INFO, "cmd_up:%s\n", cmd_up)
    lib_debug.lua_debug(lib_debug.INFO, "cmd_down:%s\n", cmd_down)

    --若是dhcp或static则只需执行up否则为pppoe则需判断逻辑口是否起来，若没起来则需sleep 3s
    if (proto == "dhcp" or proto == "static") then
        console.forkExec(cmd_up)
    else
        if (link_s ~= "") then
            console.forkExec(cmd_teardown..";sleep 2;"..cmd_up)
        else
            os.execute("sleep 3")
            console.forkExec(cmd_teardown..";sleep 2;"..cmd_up)
        end
    end

    lib_debug.lua_debug(lib_debug.INFO, "pppoe dialing.\n")

    local success = 0
    local disc_fail = 0
    local auth_fail = 0
    local g_check_file_tmp = g_check_file..data["intf_name"]..".log"
    local existed = file_exists(g_check_file_tmp)
    if (existed == false) then
        os.execute("touch "..g_check_file_tmp)
    end
    --不断读取log文件，直到超时或成功才返回
    local sysTime = os.time();
    lib_debug.lua_debug(lib_debug.DEBUG, "begin sysTime:%s.\n",sysTime)
    while (true) do
        for line in  io.lines(g_check_file_tmp) do
            lib_debug.lua_debug(lib_debug.DEBUG, "file line:%s.\n",line)
            if (line == g_disc_failed[1]) then
                lib_debug.lua_debug(lib_debug.INFO, "disc fail\n")
                disc_fail = 1
            elseif(line == g_auth_success[1]) then
                lib_debug.lua_debug(lib_debug.INFO, "auth success\n")
                success = 1
            elseif(line == g_auth_failed[1]) then
                lib_debug.lua_debug(lib_debug.INFO, "auth failed\n")
                auth_fail = 1
            elseif(line == g_ipcp_failed[1]) then
                lib_debug.lua_debug(lib_debug.INFO, "ipcp ailed\n")
                auth_fail = 1
            elseif(line == g_ipcp_success[1]) then
                lib_debug.lua_debug(lib_debug.INFO, "ipcp success\n")
                success = 1
            end
        end
        --若密码验证成功则直接退出
        if (success == 1) then
            lib_debug.lua_debug(lib_debug.INFO, "pppoe successful and break.\n")
            break
        end

        --若pppd进程结束则重启,未结束则输出pppd pid
        local pppd_pid = tool.exec("pidof pppd")
        if (pppd_pid == nil or pppd_pid == "") then
            lib_debug.lua_debug(lib_debug.INFO, "pppoe restart\n")
            os.execute("sleep 3")
            console.forkExec(cmd_up)
        else
            lib_debug.lua_debug(lib_debug.DEBUG, "pppd pid:%s.\n",pppd_pid)
        end

        --延时1s继续循环，若超时则直接退出
        os.execute("sleep 1")
        used_time = os.time() - sysTime;
        lib_debug.lua_debug(lib_debug.DEBUG, "used_time:%s.\n",used_time)
        if (used_time > g_timeout) then
            lib_debug.lua_debug(lib_debug.INFO, "time out and break.\n")
            break;
        end
    end
    lib_debug.lua_debug(lib_debug.DEBUG, "pppoe read file is over\n")

    --删除本次pppoeCheck文件并杀死pppd进程，删除单实例文件
    os.remove(g_check_file_tmp)
    os.execute(cmd_down)
    os.execute("/etc/init.d/network_wan_monitor restart >/dev/null 2>&1")
    os.remove(PPPOE_CHECK_LOCK_FILE);

    --若出现成功则表示密码验证正确直接杀死进程并直接验证成功
    if (success == 1) then
        result["res"] = "success"
        table.insert(result["data"],g_auth_success[2])
        table.insert(result["code"],g_auth_success[3])
        lib_debug.lua_debug(lib_debug.INFO, "return g_auth_success.\n")
        lib_debug.lua_debug(lib_debug.INFO, "cmd: %s\n", "touch "..g_pppoe_success_file..data["intf_name"])
        os.execute("touch "..g_pppoe_success_file..data["intf_name"])
        return cjson.encode(result)
    end
    if (auth_fail == 1) then--密码验证失败
        result["res"] = "failed"
        table.insert(result["data"],g_auth_failed[2])
        table.insert(result["code"],g_auth_failed[3])
        lib_debug.lua_debug(lib_debug.INFO, "return g_auth_fail.\n")
        return cjson.encode(result)
    end
    if (disc_fail == 1) then--未找到服务器
        result["res"] = "failed"
        table.insert(result["data"],g_disc_failed[2])
        table.insert(result["code"],g_disc_failed[3])
        lib_debug.lua_debug(lib_debug.INFO, "return g_disc_fail.\n")
        return cjson.encode(result)
    end
end
