
#include <iostream>
#include <string>
#include <thread>
#include <mutex>

struct someData {
    std::string name;
    std::string surname;
    std::string address;
    int age;
};

class exchangePerson {
public:
    someData data;
    std::mutex m;

    static void JohnDoe(exchangePerson& obj) {
        std::lock_guard<std::mutex> lock(obj.m);
        obj.data.name = "John";
        obj.data.surname = "Doe";
        obj.data.address = "Unknown";
        obj.data.age = 120;
    }

    static void JacobSmith(exchangePerson& obj) {
        std::lock_guard<std::mutex> lock(obj.m);
        obj.data.name = "Jacob";
        obj.data.surname = "Smith";
        obj.data.address = "Known";
        obj.data.age = 1;
    }

    static void Swap(exchangePerson& p1, exchangePerson& p2) {
        if (&p1 == &p2) return;

        // Конструюємо unique_lock без негайного захоплення м'ютексів
        std::unique_lock<std::mutex> lock1(p1.m, std::defer_lock);
        std::unique_lock<std::mutex> lock2(p2.m, std::defer_lock);

        // Безпечно захоплюємо обидва об'єкти unique_lock
        std::lock(lock1, lock2);

        std::swap(p1.data, p2.data);
    }

    void Print() const {
        std::cout << "Name: " << data.name << ", Surname: " << data.surname
                  << ", Address: " << data.address << ", Age: " << data.age << std::endl;
    }
};

int main() {
    exchangePerson p1, p2;

    std::thread t1(exchangePerson::JohnDoe, std::ref(p1));
    std::thread t2(exchangePerson::JacobSmith, std::ref(p2));

    t1.detach();
    t2.detach();

    std::thread tSwap([&]() {
        exchangePerson::Swap(p1, p2);
    });

    tSwap.join();

    std::cout << "--- Result after Swap (using unique_lock) ---" << std::endl;
    std::cout << "p1: "; p1.Print();
    std::cout << "p2: "; p2.Print();

    return 0;
}
