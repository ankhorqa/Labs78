#include <iostream>
#include <string>

class Person {
private:
    std::string name;
    int age;
    std::string email;
    std::string address;

public:

    std::string getName() const {
        return name;
    }

    int getAge() const {
        return age;
    }

    std::string getEmail() const {
        return email;
    }

    std::string getAddress() const {
        return address;
    }

    void setName(const std::string& newName) {
        if (!newName.empty()) {
            name = newName;
        }
        else {
            std::cerr << "Ошибка: Имя не может быть пустым." << std::endl;
        }
    }

    void setAge(int newAge) {
        if (newAge >= 0 && newAge <= 120) {
            age = newAge;
        }
        else {
            std::cerr << "Ошибка: Некорректный возраст." << std::endl;
        }
    }

    void setEmail(const std::string& newEmail) {
        if (newEmail.find('@') != std::string::npos) {
            email = newEmail;
        }
        else {
            std::cerr << "Ошибка: Некорректный email." << std::endl;
        }
    }

    void setAddress(const std::string& newAddress)
    {
        if (!newAddress.empty()) {
            address = newAddress;
        }
        else {
            std::cerr << "Error: Address cannot be empty!" << std::endl;
        }
    }

    void displayInfo() const {
        std::cout << "Имя: " << name << ", Возраст: " << age << ", Email: "
            << email << ", Адрес: " << address << std::endl;
    }
};

int main() {
    Person person;

    person.setName("Игорь Мышляев");
    person.setAge(19);
    person.setEmail("igormyslaev@example.com");
    person.setAddress("улица Лермонтова, 19");

    std::cout << "Name: " << person.getName() << std::endl;
    std::cout << "Age: " << person.getAge() << std::endl;
    std::cout << "Email: " << person.getEmail() << std::endl;
    std::cout << "Address: " << person.getAddress() << std::endl;

    person.setName("");
    person.setAge(140);
    person.setEmail("invalid-gmail");
    person.setAddress("");

    person.displayInfo();

    return 0;
}
