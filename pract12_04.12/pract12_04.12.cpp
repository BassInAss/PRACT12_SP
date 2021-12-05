// pract12_04.12.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include<Windows.h>
#include "windows.h"
#include<WindowsX.h>
#include <commctrl.h>
#include <commdlg.h>
#include <WinUser.h>
#include <string.h>
#include <Richedit.h>
#include "stdio.h"
#include "pract12_04.12.h"

#define MAX_LOADSTRING 100
#define BUTTON 101
#define BUTTON2 102
#define BUTTON3 103
#define BUTTON4 104
#define BUTTON5 2001
#define EDIT 2003
#define EDIT2 2002
#define BUTTON6 2004
#define BUTTON7 3001


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
OPENFILENAME ofn;
OPENFILENAME ofn1;
HANDLE hFile;
HWND dlg;
HWND dlg2;
DWORD bytes_written = 0;
DWORD dwBytes;
HWND hEdit;
HWND hRead;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PRACT120412, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PRACT120412));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}
BOOL ReadAsync(HANDLE file, LPVOID buf, DWORD offset, DWORD size, LPOVERLAPPED overlap) {
    ZeroMemory(overlap, sizeof(OVERLAPPED));
    overlap->Offset = offset;
    overlap->hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    BOOL ret = ReadFile(file, buf, size, NULL, overlap);
    if (ret == FALSE && GetLastError() != ERROR_IO_PENDING)
        CloseHandle(overlap->hEvent), overlap->hEvent = NULL;
    return ret;
}

BOOL WriteAsync(HANDLE file, LPCVOID buf, DWORD offset, DWORD size, LPOVERLAPPED overlap) {
    ZeroMemory(overlap, sizeof(OVERLAPPED));
    overlap->Offset = offset;
    overlap->hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    BOOL ret = WriteFile(file, buf, size, NULL, overlap);
    if (ret == FALSE && GetLastError() != ERROR_IO_PENDING)
        CloseHandle(overlap->hEvent), overlap->hEvent = NULL;
    return ret;
}

BOOL FinishIo(LPOVERLAPPED overlap) {
    if (overlap->hEvent != NULL) {
        DWORD res = WaitForSingleObject(overlap->hEvent, INFINITE);
        if (res == WAIT_OBJECT_0) {
            CloseHandle(overlap->hEvent), overlap->hEvent = NULL;
            return TRUE;
        }
    }
    return FALSE;
}




//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PRACT120412));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PRACT120412);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       GetPrivateProfileInt(TEXT("General"), TEXT("WINX"), CW_USEDEFAULT,
           TEXT("C:\\ProgramData\\app.ini")), GetPrivateProfileInt(TEXT("General"),
               TEXT("WINY"), 0, TEXT("C:\\ProgramData\\app.ini")),
       GetPrivateProfileInt(TEXT("General"), TEXT("WINW"),
           CW_USEDEFAULT, TEXT("C:\\ProgramData\\app.ini")),
       GetPrivateProfileInt(TEXT("General"), TEXT("WINH"), 0,
           TEXT("C:\\ProgramData\\app.ini")), nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   CreateWindowEx(0, L"Button", L"Создать файл", 
      BS_CENTER | WS_CHILD | WS_VISIBLE, 
       10, 10, 270, 25,
       hWnd, (HMENU)BUTTON, hInst, 0);

   CreateWindowEx(0, L"Button", L"Вывести данные выбранного файла",
       BS_CENTER | WS_CHILD | WS_VISIBLE,
       10, 130, 270, 25,
       hWnd, (HMENU)BUTTON6, hInst, 0);

   CreateWindowEx(0, L"Button", L"Переименовать файл",
       BS_CENTER | WS_CHILD | WS_VISIBLE,
       10, 170, 270, 25,
       hWnd, (HMENU)BUTTON7, hInst, 0);

   hEdit = CreateWindowEx(WS_EX_STATICEDGE, L"Edit", L"To write or rename",
       WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_BORDER,
       10, 50, 270, 25, hWnd, (HMENU)EDIT, NULL, NULL);

   CreateWindowEx(0, L"Button", L"Запись в файл",
       BS_CENTER | WS_CHILD | WS_VISIBLE,
       10, 90, 270, 25,
       hWnd, (HMENU)BUTTON5, hInst, 0);

   CreateWindowEx(0, L"Button", L"Сохранить изменения",
       BS_CENTER | WS_CHILD | WS_VISIBLE,
       670, 320, 160, 25,
       hWnd, (HMENU)BUTTON3, hInst, 0);

   CreateWindowEx(0, L"Button", L"Редактировать файл",
       BS_CENTER | WS_CHILD | WS_VISIBLE,
       330, 320, 160, 25,
       hWnd, (HMENU)BUTTON4, hInst, 0);

   CreateWindowEx(0, L"Static", L"Окно редактирования файла",
       WS_CHILD | WS_VISIBLE, 330, 0, 200, 24, hWnd, 0, 0, 0);

   hRead = CreateWindowEx(WS_EX_STATICEDGE, L"Edit", L"",
       WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_BORDER,
       330, 20, 500, 300, hWnd, (HMENU)EDIT2, NULL, NULL);


   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
        wchar_t szFile[260];
        wchar_t filename[260];
        ZeroMemory(&ofn, sizeof(ofn1));
        ofn1.lStructSize = sizeof(ofn1);
        ofn1.hwndOwner = dlg2;
        ofn1.lpstrFileTitle = filename;
        ofn1.lpstrFile = szFile;
        ofn1.lpstrFile[0] = '\0';
        ofn1.nMaxFile = sizeof(szFile);
        ofn1.lpstrFilter = L"Text files 2003(*.txt)\0*.txt\0";
        ofn1.nFilterIndex = 1;
        ofn1.lpstrFileTitle = NULL;
        ofn1.nMaxFileTitle = 0;
        ofn1.lpstrInitialDir = NULL;
        ofn1.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        OVERLAPPED overlap;
        HANDLE file;
        int n = GetWindowTextLength(hEdit);
        int poz_carret = 0;
        TCHAR* buf = new TCHAR[n + 1]{};

        if (wParam == BUTTON) //Кнопка создания файла
        {
            wchar_t buf[255] = L"";
            wchar_t filter[] = L"Data Files(*.txt)\0*.txt\0All Files\0*.*\0\0";
            char filterExt[][6] = { ".txt" };
            wchar_t cCustomFilter[256] = L"\0\0";
            int nFilterIndex = 0;
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.hwndOwner = dlg;
            ofn.hInstance = hInst;
            ofn.lpstrFilter = filter;
            ofn.lpstrCustomFilter = cCustomFilter;
            ofn.nMaxCustFilter = 256;
            ofn.nFilterIndex = nFilterIndex;
            ofn.lpstrFile = buf;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFile = 255;
            ofn.nMaxFileTitle = 255;
            ofn.lpstrInitialDir = NULL;
            ofn.lpstrTitle = 0;
            ofn.Flags = OFN_FILEMUSTEXIST;
            ofn.nFileOffset = 0;
            ofn.nFileExtension = 0;
            ofn.lpstrDefExt = NULL;
            ofn.lCustData = NULL;
            ofn.lpfnHook = NULL;
            ofn.lpTemplateName = NULL;
            
            if (GetSaveFileName(&ofn))
            {
                DWORD dwFlagsAndAttributes = FILE_FLAG_DELETE_ON_CLOSE | FILE_ATTRIBUTE_TEMPORARY | FILE_ATTRIBUTE_HIDDEN;

                 hFile = CreateFile(buf, GENERIC_READ |
                    GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_NEW,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);

                if (INVALID_HANDLE_VALUE != hFile)
                {
                    MessageBox(0, L"Файл создан", L"MessageBox caption", MB_OK);
                }
            }
        }
        if (wParam == BUTTON3)
        {
            if (GetOpenFileName(&ofn1) == TRUE)
            {
                int n = GetWindowTextLength(hRead);
                WCHAR* buf4 = new WCHAR[n + 1]{};
                char* output_buf2 = new char[n + 1]{};
                GetWindowText(hRead, buf, n + 1);

                WideCharToMultiByte(0, 0, buf, -1, output_buf2, n + 1, 0, 0);
                file = CreateFile(szFile, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
                if (WriteAsync(file, output_buf2, 0, n + 1, &overlap))
                {
                    MessageBox(0, L"Файл с изменениями сохранен", L"MessageBox caption", MB_OK);
                }
                FinishIo(&overlap);
                CloseHandle(file);
            }
        }
        if (wParam == BUTTON4)
        {
            if (GetOpenFileName(&ofn1) == TRUE)
            {
                char* buf2 = new char[MAX_PATH] {};
                file = CreateFile(szFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
                if (ReadAsync(file, buf2, 0, MAX_PATH, &overlap))
                {
                    MessageBox(0, L"Файл прочитан", L"MessageBox caption", MB_OK);
                    MultiByteToWideChar(0, 0, buf2, MAX_PATH, buf, MAX_PATH);
                    SetWindowText(hRead, buf);
                    FinishIo(&overlap);
                    CloseHandle(file);
                }
            }
        }
        if (wParam == BUTTON5) //Запись в файл
        {
            if (GetOpenFileName(&ofn1) == TRUE)
            {
                int n = GetWindowTextLength(hEdit);
                WCHAR* buf4 = new WCHAR[n + 1]{};
                char* output_buf = new char[n + 1]{};
                GetWindowText(hEdit, buf4, n + 1);
                WideCharToMultiByte(0, 0, buf4, -1, output_buf, n, 0, 0);
                file = CreateFile(szFile, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
                SetFilePointer(szFile, 0, NULL, FILE_END);

                if (WriteAsync(file, output_buf, 0, n + 1, &overlap))
                {
                    MessageBox(0, L"Запись прошла", L"MessageBox caption", MB_OK);
                    FinishIo(&overlap);
                    CloseHandle(file);
                }
            }
        }
        if (wParam == BUTTON6)
        {
            if (GetOpenFileName(&ofn1) == TRUE)
            {
                LPCWSTR rez;
                SYSTEMTIME time;
                SYSTEMTIME time2;
                SYSTEMTIME time3;
                WIN32_FIND_DATAW wfd;
                HANDLE const hFind = FindFirstFileW(szFile, &wfd);

                if (INVALID_HANDLE_VALUE != hFind)
                {
                    do
                    {
                        FileTimeToSystemTime(&wfd.ftCreationTime, &time);
                        //std::wcout << wfd.cFileName << ' ' << time.wDay << '-' << time.wMonth << '-' << time.wYear << std::endl;
                    } while (NULL != FindNextFileW(hFind, &wfd));

                    FindClose(hFind);
                }
                FileTimeToSystemTime(&wfd.ftLastWriteTime, &time2);
                FileTimeToSystemTime(&wfd.ftLastAccessTime, &time3);
                
                TCHAR bufx[256] = { 0 };
                _stprintf_s(bufx, L"Имя файла: %s\nДата создания: %u.%u.%u\nАтрибуты файла: %i\nРазмер файла: %u байт\nДата последнего изменения: %u.%u.%u в %u:%u\nДата последнего обращения к файлу: %u.%u.%u в %u:%u", &wfd.cFileName,time.wDay,time.wMonth,time.wYear, wfd.dwFileAttributes, wfd.nFileSizeLow, time2.wDay, time2.wMonth, time2.wYear, time2.wHour, time2.wMinute, time3.wDay, time3.wMonth, time3.wYear, time3.wHour, time3.wMinute);
                MessageBox(0, bufx, L"MessageBox caption", MB_OK);
            }
        }
        if (wParam == BUTTON7)
        {
            if (GetOpenFileName(&ofn1) == TRUE)
            {
                WCHAR* buf4 = new WCHAR[n + 1]{};
                GetWindowText(hEdit, buf4, n + 1);
                MoveFile(szFile, buf4);
            }
        }
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                //DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
