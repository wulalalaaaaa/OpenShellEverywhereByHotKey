#include <windows.h>
#include <shellapi.h>

#define DEBUG 0

#if DEBUG
#include<iostream>
#include<assert.h>
#endif


    HINSTANCE hInstance;

inline wchar_t* GetActiveWindowPath(){
    // �õ�����
    HWND hwnd = GetForegroundWindow();
    if (hwnd){
        //��Ϊ���е�command������Ī�������bug������ջ�ڴ治���󣬿�new������
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
    //�ϳ�����
#if DEBUG
    std::wcout << command; //����������������������ʵ�ʵ�ֵ���ǲ�һ��������ʲôͷ����
#endif
    //��shell
    ShellExecute(NULL,L"open",L"powershell.exe",command,NULL,SW_SHOW);
}

void ExecuteTask(){
    wchar_t* currentPath = GetActiveWindowPath();
    if (currentPath != nullptr && currentPath[0] != '\0'){
        OpenPowerShell(currentPath);
    }
#if DEBUG
    else{
        std::cout << "currentPath�յģ�\n";
        assert(0);
    }
#endif
    free(currentPath);//ǰ��new��һ�£���ô�㶫���ò���������ָ��
#if DEBUG
    std::cout << "\n ok!\n";
#endif
}

LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam){
    switch (uMsg){
        case WM_HOTKEY://���߷�
            ExecuteTask();
            break;
        case WM_DESTROY://�ر�
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd,uMsg,wParam,lParam);//������ϢĬ�ϴ���
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd){
    
    // ��������
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"hotkey_openShellEverywhere";
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName,L"Hidden",0,0,0,0,0,NULL,NULL,wc.hInstance,NULL);

// ע���ȼ�
    if (!RegisterHotKey(hwnd, 1, MOD_CONTROL | MOD_SHIFT | MOD_ALT, VK_OEM_4)){
#if DEBUG
        std::cerr << "��ERROR ע���ݼ���"<< GetLastError()<<"\n";
        assert(0);
#endif
        return 1;
    }
    
    MSG msg;
    while (GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);//�����ַ�
        DispatchMessage(&msg);//��֪Procedure
    }
    //ɾ����ݼ�
    UnregisterHotKey(NULL,1);
#if DEBUG
    std::cout << "exit.\n";
#endif
    return 0;
}
