print(require "lxnet_lib")

for k,v in pairs(Listener) do
    print(k,v)
end

print("--------------------")

local port = 8081

lxnet.net_init(1,2,3,4,5,6,7,8)
local lis = Listener.Create()
lis:Listen(port,10)
print("listen",port,lis)
local net = {}
while true do
    if lis:CanAccept() then
        local clt = lis:Accept()
        print("accept",clt)
        table.insert(net,clt)
    end
    for _,sock in pairs(net) do
        local data,len = sock:GetData()
        if len>0 then
            print(len,data)
            sock:SendData(data,len)
        end
    end
    
    for _,sock in pairs(net) do
        sock:CheckSend()
        sock:CheckRecv()
    end

    lxnet.net_run()
end
