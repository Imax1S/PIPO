#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;
mutex result_mutex;
unsigned long long result = 1;

void factorial(unsigned long long n, unsigned int start, unsigned int end) {
    unsigned long long partial_result = 1;
    for (unsigned long long i = start; i <= end; i++) {
        partial_result *= i;
    }
    lock_guard<mutex> lock(result_mutex);
    result *= partial_result;
}

int main(int argc, char *argv[]) {
    int num_threads;
    if (argc == 2) {
        num_threads = stoi(argv[1]);
    } else {
        num_threads = thread::hardware_concurrency() + 1;
    }

    unsigned long long n;
    cout << "Enter a non-negative integer: ";
    cin >> n;

    unsigned long long chunk_size = n / num_threads;
    vector<thread> threads;
    for (int i = 0; i < num_threads; i++) {
        unsigned int start = i * chunk_size + 1;
        unsigned int end = (i == num_threads - 1) ? n : (i + 1) * chunk_size;
        threads.emplace_back(factorial, n, start, end);
    }

    for (auto &thread: threads) {
        thread.join();
    }

    cout << n << "! = " << result << endl;

    return 0;
}