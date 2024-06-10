/*  Принимает изобр из буФЕРА
 g++ _.cpp -o _.exe -lgdi32
 lgdi32 - ссылка на библиотеку.
 библиотека gdi32.dll является встроенной библиотекой Windows и предоставляет 
 функциональность для работы с графическими устройствами интерфейса 
 (GDI - Graphics Device Interface) в операционной системе Windows. 
 Она содержит множество функций для рисования графики, работы с шрифтами, 
 цветами, палитрами и другими элементами графического интерфейса пользователя.

 */

#include <Windows.h>

// Функция для получения изображения из буфера обмена
HBITMAP GetImageFromClipboard()
{
    if (OpenClipboard(NULL)) // Открываем буфер обмена
    {
        HBITMAP hBitmap = NULL;
        HANDLE hData = GetClipboardData(CF_DIB); // Получаем данные из буфера обмена

        if (hData != NULL)
        {
            LPVOID pData = GlobalLock(hData); // Блокируем данные
            if (pData != NULL)
            {
                BITMAPINFO* pBitmapInfo = (BITMAPINFO*)pData;
                BYTE* pBits = (BYTE*)pBitmapInfo + pBitmapInfo->bmiHeader.biSize;

                HDC hdc = GetDC(NULL);
                hBitmap = CreateDIBitmap(hdc, &pBitmapInfo->bmiHeader, CBM_INIT, pBits, pBitmapInfo, DIB_RGB_COLORS); // Создаем битмап из данных
                ReleaseDC(NULL, hdc);

                GlobalUnlock(hData); // Разблокируем данные
            }
        }

        CloseClipboard(); // Закрываем буфер обмена
        return hBitmap;
    }

    return NULL;
}

// Функция для отображения битмапа на экране
void ShowBitmap(HBITMAP hBitmap) // Объявление функции ShowBitmap с параметром hBitmap типа HBITMAP
{
    HWND hwnd = GetDesktopWindow(); // Получение дескриптора окна рабочего стола
    HDC hdc = GetDC(hwnd); // Получение контекста устройства для окна

    BITMAP bitmap; // Создание структуры BITMAP для хранения информации о битмапе
    GetObject(hBitmap, sizeof(BITMAP), &bitmap); // Получение информации о битмапе

    HDC hdcMem = CreateCompatibleDC(hdc); // Создание совместимого контекста устройства
    SelectObject(hdcMem, hBitmap); // Выбор битмапа в контексте устройства

    BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY); // Копирование изображения на экран

    DeleteDC(hdcMem); // Удаление контекста устройства
    ReleaseDC(hwnd, hdc); // Освобождение контекста устройства
}

int main()
{

    HBITMAP hImage = GetImageFromClipboard(); // Получаем изображение из буфера обмена

    if (hImage)
    {
        ShowBitmap(hImage); // Отображаем изображение на экране

        Sleep(5000); // Подождать 5 секунд перед закрытием окна

        DeleteObject(hImage); // Удаляем объект битмапа
    }

    return 0;
}
