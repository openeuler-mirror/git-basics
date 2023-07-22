#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

int main() {
    // 将文件名设置成Gitee ID的文件将置于该源代码文件
    // 所生成可执行文件的上级目录。
    DIR *dir = opendir("..");
    if (dir == NULL) {
        perror("Unable to open parent directory");
        return 1;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        // 只处理常规文件（排除目录、软链接等）。
        // 另外，排除“.”和“..”目录项。并且排除
        // 带有扩展名的文件（其中包含“.”），因为
        // 该程序要求将文件名设置成Gitee ID，因此
        // 其中不可能包含“.”。
        if (entry->d_type == DT_REG && 
            strchr(entry->d_name, '.') == NULL) {
            printf("Greetings from %s\n", entry->d_name);
        }
    }

    closedir(dir);

    printf(":)\n");
    return 0;
}
