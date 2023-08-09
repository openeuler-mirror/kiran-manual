# kiran-manual 系统帮助手册

## 编译安装
下列两种方式选择其一即可。

方式一：
```bash
# mkdir build; cd build
# cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=debug ..
# make -j4
# sudo make install
```
方式二：
```bash
# cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=debug
# cmake --build build  -j4 -v
# sudo cmake --install build
```

## 运行
```bash
# /usr/bin/kiran-manual
```
