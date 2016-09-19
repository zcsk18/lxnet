rm -rf *.so
g++ lxnet_lua.cpp -fPIC -shared -o lxnet_lib.so -I"/usr/include/lua5.1/" -L"/usr/local/lib/" -I"./../" -I"./../../../base" -L"./../" -llxnet -lpthread -lrt
