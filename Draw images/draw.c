#include <stdio.h>  // for puts, putchar, getc, fclose, fopen, printf
#include <string.h>

int draw(char picture_name[]){
    int c; // Переменная для помещения считываемых байтов символа
    FILE *file; // переменная в которую помещен указатель
    file = fopen(picture_name, "r");
    if (file) {
        while ((c = getc(file)) != EOF) // EOF при конце файла или при ошибке
            putchar(c); // преобразуем байты в символ
        fclose(file); // закрываем файл
    }
    return 0;
}

int main(int argc, char *argv[]){
    char help[] = "--help";
    char human[] = "-h", car[] = "-c", animal[] = "-a", rose[] = "-r", gun[] = "-g";
    for (int i = 0; i < argc; i++)
    {
        if (strcmp (human, argv[i])==0){
            draw("human.txt");
        }
        else if (strcmp (help, argv[i])==0){
            printf("-a --\e[1;31m print\e[1;37m animal\n-h --\e[1;31m print\e[1;37m human\n");
            printf("-c --\e[1;31m print\e[1;37m car\n-r --\e[1;31m print\e[1;37m rose\n-g --\e[1;31m print\e[1;37m gun\n");
        }
        else if (strcmp (car, argv[i])==0){
            draw("car.txt");
        }
        else if (strcmp (animal, argv[i])==0){
            draw("animal.txt");
        }
        else if (strcmp (rose, argv[i])==0){
            draw("rose.txt");
        }       
        else if (strcmp (gun, argv[i])==0){
            draw("gun.txt");
        }
    }
    return 0;
}