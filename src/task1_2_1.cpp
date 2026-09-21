
#include <iostream>
#include <thread>

void Thread1() {
    std::cout << "1 ";
}

void Thread2() {
    std::cout << "2 ";
}

int main() {
    // Невизначена поведінка (Undefined Behavior):
    // Потоки знищуються без join() або detach()
    std::thread t1(Thread1);
    std::thread t2(Thread2);

    return 0; // Додаток впаде з помилкою std::terminate
}
