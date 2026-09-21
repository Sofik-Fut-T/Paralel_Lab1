
#include <iostream>
#include <list>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::list<int> l;
std::mutex m;

void AddToList(int value) {
    std::lock_guard<std::mutex> lock(m);
    l.push_back(value);
    std::cout << "[Add] Added: " << value << std::endl;
}

void ListContains(int searchVal) {
    std::lock_guard<std::mutex> lock(m);
    bool found = false;
    for (int val : l) {
        if (val == searchVal) {
            found = true;
            break;
        }
    }
    std::cout << "[Contains] Search " << searchVal << ": " 
              << (found ? "Found" : "Not found") << std::endl;
}

int main() {
    int initialVal = 10;

    for (int i = 0; i < 10; ++i) {
        std::thread tAdd(AddToList, initialVal + i);
        std::thread tCheck(ListContains, initialVal);
        
        tAdd.detach();
        tCheck.detach();
    }

    // Невелика пауза, щоб від’єднані потоки встигли виконатися до завершення main
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return 0;
}
