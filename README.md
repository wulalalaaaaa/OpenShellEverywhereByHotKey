How to use:

download the exe or compile your own exe.
put it at anyplace.(when directry is invalid,shell will cd the position of exe.)
click 2,run exe!
now hotkey worked.


When you download the release, the hotkey is "Ctrl+Shift+Alt+[".
if you want change another key, please download main.cpp and 
change RegisterHotKey() by https://learn.microsoft.com/zh-cn/windows/win32/inputdev/virtual-key-codes .
compile and get the exe.

When you want to close it.
open the ......任务管理器！.,,【Ctrl + Shift + Esc】，find the newShellbyHotkey, close it.
