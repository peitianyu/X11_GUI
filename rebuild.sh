# 如果没有build文件夹, 新建一个
if [ ! -d "build" ]; then
    mkdir build
fi

cd build && cmake .. && make -j6

./x11_gui

cd ..