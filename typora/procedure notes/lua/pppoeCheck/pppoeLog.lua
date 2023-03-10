#!/usr/bin/env lua
--项目名
package.path = "/usr/lib/lua/?.lua;" .. package.path;
module("pppoeLog", package.seeall);
cjson = require "dkjson"
tool = require "dev.tools.common"

local g_run_file    = "/tmp/debug/rp-pppoe/pppoe_running_"
local g_run_file_old    = "/tmp/debug/rp-pppoe/pppoe_running_"
local g_pppoe_success_file  = "/tmp/pppoe_success_"

local g_disc_failed   = {"discovery failed", "未查找到pppoe服务器", "1"}
local g_disc_success  = {"discovery success", "查找到pppoe服务器", "2"}
local g_auth_success  = {"account authentication passed" ,"账号认证成功", "3"}
local g_auth_failed  = {"account authentication failed" ,"账号认证失败", "4"}
local g_ipcp_success  = {"ipcp success" ,"ip协商成功", "5"}
local g_ipcp_failed   = {"ipcp failed" ,"ip协商失败", "6"}
local g_pppoe_check   = {"pppoe拨号成功" ,"pppoe拨号失败", "7"}

local g_time_expired  = {"connect time expired" ,"连接时间过期", "8"}
local g_conn_terminat = {"connection terminated" ,"连接终止", "9"}
local g_fail_disconn  = {"failed to disconnect pppoe socket" ,"中断连接失败", "10"}
local g_pppoe_disconnect    = {"disconnect device" ,"断开pppoe服务器", "11"}
local g_pppoe_connect       = {"connect device" ,"开始连接pppoe服务器", "12"}


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

function intf_log_get(intf_name)
    local file = {}
    local file_num = 1
    local result = {}

    local run_file = g_run_file..intf_name..".log"
    local run_file_old = g_run_file_old..intf_name..".log.old"

    local fd = io.open(run_file_old)
    if (fd ~= nil) then
        file[file_num] = run_file_old
        file_num = file_num + 1
        io.close(fd)
    end

    fd = io.open(run_file)
    if (fd ~= nil) then
        file[file_num] = run_file
        io.close(fd)
    end

    local successtime = ""
    i = 1
    for j=1,#file do
        for line in io.lines(file[j]) do
        repeat
        --[2020-12-23 19:25:22]:connect device
            local len = string.find(line, "]:", 1)
            if (len == nil) then
                break
            end
            result[i] = {}
            --[2020-12-23 19:25:22]
            result[i]["time"] = string.sub(line, 2, len - 1)
            --connect device
            result[i]["msg"] = string.sub(line, len + 2, string.len(line))

            if (result[i]["msg"] == g_disc_failed[1]) then
                result[i]["code"] =  g_disc_failed[3]
            elseif(result[i]["msg"] == g_disc_success[1]) then
                result[i]["code"] =  g_disc_success[3]
                successtime = result[i]["time"]
            elseif(result[i]["msg"] == g_auth_success[1]) then
                result[i]["code"] =  g_auth_success[3]
                successtime = result[i]["time"]
            elseif(result[i]["msg"] == g_auth_failed[1]) then
                result[i]["code"] =  g_auth_failed[3]
            elseif(result[i]["msg"] == g_ipcp_failed[1]) then
                result[i]["code"] =  g_ipcp_failed[3]
            elseif(result[i]["msg"] == g_ipcp_success[1]) then
                result[i]["code"] =  g_ipcp_success[3]
                successtime = result[i]["time"]
            elseif(result[i]["msg"] == g_time_expired[1]) then
                result[i]["code"] =  g_time_expired[3]
            elseif(result[i]["msg"] == g_conn_terminat[1]) then
                result[i]["code"] =  g_conn_terminat[3]
            elseif(result[i]["msg"] == g_fail_disconn[1]) then
                result[i]["code"] =  g_fail_disconn[3]
            elseif(result[i]["msg"] == g_pppoe_disconnect[1]) then
                result[i]["code"] =  g_pppoe_disconnect[3]
            elseif(result[i]["msg"] == g_pppoe_connect[1]) then
                result[i]["code"] =  g_pppoe_connect[3]
                successtime = result[i]["time"]
            end
            result[i]["order"] = string.format([[%s]], i)
            i = i + 1
        until true
        end
    end

    local result_success = {}
    g_pppoe_success_file = g_pppoe_success_file..intf_name
    local exised = file_exists(g_pppoe_success_file)
    if (exised == true) then
        result_success[1] = {}
        if successtime == ""  then
            result_success[1]["time"] = os.date()
        else
            result_success[1]["time"] =  successtime
        end
        result_success[1]["msg"] = g_auth_success[1]
        result_success[1]["code"] =  g_auth_success[3]
        result_success[1]["order"] = "1"
        return result_success
    end

    return result
end

--[[
dev_sta get  -m pppoeLog '{"intf_name":["wan","wan1"]}'

{
  "data": [
    {
      "order": "1",                              --第几个对象
      "msg": "discovery success",                --信息
      "time": "[2020-12-23 19:25:22]",           --时间戳
      "code": "2"                                --code码
    },
    {
      "order": "2",
      "msg": "account authentication passed",
      "time": "[2020-12-23 19:25:22]",
      "code": "3"
    }
  ],
  "count": "2"                                  --总数量
}

]]--
function module_get(param)
    local intf = cjson.decode(param)
    local buf = {}

    --for j=1,#intf["intf_name"] do
    for key, val in pairs(intf["intf_name"]) do
        buf[val] = intf_log_get(val)

    end
    return cjson.encode(buf)
end



