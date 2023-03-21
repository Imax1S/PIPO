#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <thread>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    int num_processes = 0;
    int num = 0;

    // Проверяем, задано ли количество процессов в аргументах командной строки
    if (argc > 1) {
        num_processes = std::stoi(argv[1]);
    } else {
        // Если не задано, то количество процессов равно количеству логических ядер плюс один
        num_processes = std::thread::hardware_concurrency() + 1;
//        num_processes = 1;
    }

    // Считываем число, для которого нужно вычислить факториал
    std::cin >> num;

    // Создаем процессы и распределяем задачи между ними
    int start = 1;
    int end = 1;
    int chunk_size = num / num_processes;
    int remainder = num % num_processes;
    for (int i = 0; i < num_processes; ++i) {
        if (i < remainder) {
            end = start + chunk_size;
        } else {
            end = start + chunk_size - 1;
        }

        pid_t pid = fork();
        if (pid == 0) {
            // Это дочерний процесс, вычисляем часть факториала
            long result = 1;
            for (int j = start; j <= end; ++j) {
                result *= j;
            }
            std::cout << result << std::endl;
            exit(0);
        } else if (pid < 0) {
            // Ошибка при создании процесса
            std::cerr << "Error creating process" << std::endl;
            exit(1);
        } else {
            // Это родительский процесс, переходим к следующей части
            start = end + 1;
        }
    }

    // Дожидаемся завершения всех дочерних процессов
    for (int i = 0; i < num_processes; ++i) {
        int status;
        wait(&status);
    }

    // Выводим результат
    long result = 1;
    for (int i = 1; i <= num; ++i) {
        result *= i;
    }
    std::cout << num << "! = " << result << std::endl;

    return 0;
}