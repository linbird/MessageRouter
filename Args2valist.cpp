#include <iostream>
#include <cstdarg>

// 辅助函数，递归展开参数列表
void printVariadicArgs(const char* format, va_list args) {
    while (*format != '\0') {
        if (*format == 'd') {
            int value = va_arg(args, int);
            std::cout << value << ", ";
        } else if (*format == 'f') {
            double value = va_arg(args, double);
            std::cout << value << ", ";
        } else if (*format == 's') {
            const char* value = va_arg(args, const char*);
            std::cout << value << ", ";
        }

        ++format;
    }
}

// 辅助函数，获取最后一个固定参数的地址
template <typename T>
T getLastArg(T arg) {
    return arg;
}

// 模板函数，将变长参数转为va_list
template<typename... Args>
void printArgs(const char* format, Args... args) {
    va_list argList;
    va_start(argList, format); // 获取最后一个固定参数的地址

    printVariadicArgs(format, argList);

    va_end(argList);
}

int main() {
    // 使用变长参数模板处理参数，并转为va_list
    printArgs("dfs", 42, 3.14, "world");

    return 0;
}
