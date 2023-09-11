# kiran-manual 系统帮助手册
## 安装依赖
```bash
dnf install kiran-log-qt5 kiranwidgets-qt5 kiran-qt5-integration
```

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

## 文档库目录说明
```bash
.
├── data                 # 数据目录，存放配置文件及文档库
│   ├── documents        # 文档库主目录，存放文档和图片
│   │   ├── images       # 文档库图片
│   │   │   ├── docs     # 文档中的图片
│   │   │   │   ├── template-document-img.png
│   │   │   │   └── ...
│   │   │   └── nav      # 文档导航图片
│   │   │       ├── template-icon.png
│   │   │       └── ...
│   │   └── markdowns    # 文档源 Markdown 文件，包含不同语言
│   │       ├── en_US    # 默认英文
│   │       │   ├── template-app1.md
│   │       │   ├── template-app2.md
│   │       │   └── ...
│   │       ├── zh_CN    # 中文
│   │       │   ├── template-app1.md
│   │       │   ├── template-app2.md
│   │       │   └── ...
│   │       └── ...
│   ├── km.ini           #文档库配置文件
│   └── ...
```
