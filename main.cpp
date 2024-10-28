#include <windows.h>
#include <shellapi.h>

#define DEBUG 0

#if DEBUG
#include<iostream>
#include<assert.h>
#endif


    HINSTANCE hInstance;

inline wchar_t* GetActiveWindowPath(){
    // 拿到标题
    HWND hwnd = GetForegroundWindow();
    if (hwnd){
        //因为运行到command那里有莫名其妙的bug，怀疑栈内存不够大，开new正常了
        wchar_t* title = new wchar_t[512];
        GetWindowTextW(hwnd,title,512);
        return title;
    }
    return nullptr;
}


inline void OpenPowerShell(wchar_t* path){
    wchar_t command[512]= L"-noexit -command Set-Location -Path \"";
    wcsncat_s(command,path,450);
    wcsncat_s(command,L"\"",5);
    //合成神龙
#if DEBUG
    std::wcout << command; //测试神龙，这个测试输出和实际的值老是不一样你们有什么头绪吗（
#endif
    //开shell
    ShellExecute(NULL,L"open",L"powershell.exe",command,NULL,SW_SHOW);
}

void ExecuteTask(){
    wchar_t* currentPath = GetActiveWindowPath();
    if (currentPath != nullptr && currentPath[0] != '\0'){
        OpenPowerShell(currentPath);
    }
#if DEBUG
    else{
        std::cout << "currentPath空的！\n";
        assert(0);
    }
#endif
    free(currentPath);//前面new了一下，这么点东西用不着上智能指针
#if DEBUG
    std::cout << "\n ok!\n";
#endif
}

LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam){
    switch (uMsg){
        case WM_HOTKEY://出具法
            ExecuteTask();
            break;
        case WM_DESTROY://关闭
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd,uMsg,wParam,lParam);//其他消息默认处理
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd){
    
    // 创建窗口
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"hotkey_openShellEverywhere";
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName,L"Hidden",0,0,0,0,0,NULL,NULL,wc.hInstance,NULL);

// 注册热键
    if (!RegisterHotKey(hwnd, 1, MOD_CONTROL | MOD_SHIFT | MOD_ALT, VK_OEM_4)){
#if DEBUG
        std::cerr << "【ERROR 注册快捷键】"<< GetLastError()<<"\n";
        assert(0);
#endif
        return 1;
    }
    
    MSG msg;
    while (GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);//换成字符
        DispatchMessage(&msg);//告知Procedure
    }
    //删除快捷键
    UnregisterHotKey(NULL,1);
#if DEBUG
    std::cout << "exit.\n";
#endif
    return 0;
}
