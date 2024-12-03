add_executable(filetree
        filetree/main.cpp
        fsmon-linux/main.cpp
        fsmon-linux/FileWatcher.h
)

add_executable(fsmon-linux
        fsmon-linux/main.cpp
        fsmon-linux/FileDesc.h
        fsmon-linux/FileWatcher.h
)