/*********
 * main.c
 *
 * This program runs on Linux and UNIX-like macOS.
 *
 * How to compile and run the program:
 * $ gcc this-file
 * $ ./a.out
 *
 *********/ 

#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

int main() {
    // 其名称设置成Gitee-ID的文件将置于该源代码文件的上级目录
    DIR *dir = opendir("..");
    if (dir == NULL) {
        perror("Unable to open parent directory");
        return 1;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        // 0. Gitee-ID命名规则：只允许字母、数字或者下划线（_）、
        //    中划线（-），至少 2 个字符，必须以字母开头，
        //    不能以特殊字符结尾。
        // 1. 本程序只处理常规文件（排除目录、软链接等）。
        // 2. 本程序会排除“.”和“..”目录项，也会排除带有扩展名的文件。
        if (entry->d_type == DT_REG && 
            strchr(entry->d_name, '.') == NULL) {
            printf("Greetings from %s!\n", entry->d_name);
        }
    }

    closedir(dir);

    printf(":)\n");
    return 0;
}
