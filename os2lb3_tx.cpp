/* 
Копирует изображение в буфер обмена.

 g++ _.cpp -o _.exe -lgdi32
 lgdi32 - ссылка на библиотеку.
 библиотека gdi32.dll является встроенной библиотекой Windows и предоставляет 
 функциональность для работы с графическими устройствами интерфейса 
 (GDI - Graphics Device Interface) в операционной системе Windows. 
 Она содержит множество функций для рисования графики, работы с шрифтами, 
 цветами, палитрами и другими элементами графического интерфейса пользователя.

 */

#include <Windows.h>

// Функция для копирования изображения в буфер обмена
HGLOBAL CopyImageToClipboard(const char* imagePath)
{
    HGLOBAL hGl = NULL; // Обнуляем дескриптор глобальной памяти

    HBITMAP hImage = (HBITMAP)LoadImageA(NULL, imagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); // Загружаем изображение из файла в формате BMP

    if (hImage)
    {
        BITMAP bm;
        GetObject(hImage, sizeof(bm), &bm); // Получаем информацию об изображении

        BITMAPINFO bmi;
        ZeroMemory(&bmi, sizeof(bmi)); // Обнуляем структуру BITMAPINFO
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = bm.bmWidth; // Ширина изображения
        bmi.bmiHeader.biHeight = bm.bmHeight; // Высота изображения
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 16; // Количество бит на пиксель для цвета
        bmi.bmiHeader.biCompression = BI_RGB; // Без сжатия

        HDC hdc = GetDC(NULL); // Получаем контекст устройства для экрана
        HDC hdcMem = CreateCompatibleDC(hdc); // Создаем совместимый контекст устройства

        hGl = GlobalAlloc(GMEM_MOVEABLE, sizeof(BITMAPINFO) + bm.bmWidth * bm.bmHeight * 3); // Выделяем память для хранения данных изображения (16-битный RGB)
        if (hGl)
        {
            LPVOID buffer = GlobalLock(hGl); // Блокируем память для записи
            if (buffer)
            {
                memcpy(buffer, &bmi, sizeof(BITMAPINFO)); // Копируем информацию о битмапе

                // Копируем данные изображения в блок памяти
                GetDIBits(hdcMem, hImage, 0, bm.bmHeight, (LPBYTE)buffer + sizeof(BITMAPINFO), &bmi, DIB_PAL_COLORS);

                GlobalUnlock(hGl); // Разблокируем память
            }
        }

        DeleteDC(hdcMem); // Удаляем контекст устройства
        ReleaseDC(NULL, hdc); // Освобождаем контекст устройства
        DeleteObject(hImage); // Удаляем объект изображения
    }

    return hGl; // Возвращаем дескриптор глобальной памяти
}

int main()
{
    HGLOBAL hData = CopyImageToClipboard("fl.bmp"); // Копируем изображение в буфер обмена (укажите путь к изображению)

    if (hData)
    {
        if (OpenClipboard(NULL)) // Открываем буфер обмена
        {
            EmptyClipboard(); // Очищаем буфер обмена
            SetClipboardData(CF_DIB, hData); // Устанавливаем данные изображения в буфер обмена в формате CF_DIB
            CloseClipboard(); // Закрываем буфер обмена
        }
    }

    return 0;
}