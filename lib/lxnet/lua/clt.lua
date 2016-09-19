print(require "lxnet_lib")

for k,v in pairs(Socketer) do
    print(k,v)
end

print("--------------------")

local port = 8081

lxnet.net_init(1,2,3,4,5,6,7,8)
local clt = Socketer.Create()
while not clt:Connect("zcsk18.cn",port) do end

print("connect",port,clt)
clt:SendData("zcs123",6)
while true do
    local data,len = clt:GetData()
    if len>0 then
        print("clt",len,data)
        clt:SendData(data,len)
    end
    
    clt:CheckSend()
    clt:CheckRecv()

    lxnet.net_run()
end
