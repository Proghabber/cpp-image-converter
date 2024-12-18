Конвертер поддерживает конвертацию форматов друг в друга: BMP, JPEG, PPM.

Для запуска необходимо поместить исходный файл рядом с imgconv.exe и запустить из терминала:
imgconv.exe <in_file> <out_file>

Проект необходимо собирать CMake'ом из папки ImgConverter, используя соответствующий CMakeLists.
Также необходимо собрать стороннюю библиотеку LibJPEG и разместить ее в корне проекта под именем libjpeg
(не стоит менять путь до библиотеки или название папки, они забиты в CMakeLists).

Собираем LibJPEG для проекта

Скачать библиотеку LibJPEG https://www.ijg.org/.
Внутри скачанной папки проверить, что есть файл configure, и ввести в консоли "./configure --help" для уточнения.
Отключим сборку динамической библиотеки.

Для отладочной версии:
"mkdir debug" // создадим папку сразу
"cd debug" // переходим в нее
"../configure --enable-shared=no CPPFLAGS=-DDEBUG -g -O0 CFLAGS=-g -O0" // непосредственно сборка без динамической библиотеки
Под компилятор MinGW понадобится дополнительный параметр MAKE="mingw32-make".

Для релиза:
"mkdir release"
"cd release"
"../configure --enable-shared=no CPPFLAGS=-O3 CFLAGS=-O3" // если компилятор MinGW, не забыть MAKE="mingw32-make".

Один из созданных файлов — Makefile. Если он есть, значит, пора запускать утилиту make или mingw32-make.

Создайте пакет LibJPEG, вот его структура (лучше структура видна если открытть в ide или блокноте):
libjpeg/
+-- include/
¦   +-- jconfig.h
¦   +-- jerror.h
¦   +-- jmorecfg.h
¦   L-- jpeglib.h
L-- lib/
    +-- Debug
    ¦   L-- libjpeg.a
    L-- Release
        L-- libjpeg.a 


