#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;
class Patient
{
private:
    uint32_t cardNumber; // Номер карты
    uint32_t codeOfAChronicDisease; // Код хронического заболевания
    string attendingPhysician; // Лечащий врач
public:
    Patient() // Конструктор по-умолчанию
    {
        this->cardNumber = 0;
        this->codeOfAChronicDisease = 0;
        this->attendingPhysician = "";
    }
    Patient(uint32_t cardNumber, uint32_t codeOfAChronicDisease, string attendingPhysician) // Конструктор с 3-мя параметрами
    {
        this->cardNumber = cardNumber;
        this->codeOfAChronicDisease = codeOfAChronicDisease;
        this->attendingPhysician = attendingPhysician;
    }
    // Геттеры и сеттеры дл каждого поля
    uint32_t getCardNumber() const{ return this->cardNumber; }
    uint32_t getCodeOfAChronicDisease() const { return this->codeOfAChronicDisease; }
    string getAttendingPhysician() const { return this->attendingPhysician; }
    void setCardNumber(uint32_t cardNumber) { this->cardNumber = cardNumber; }
    void setCodeOfAChronicDisease(uint32_t codeOfAChronicDisease) { this->codeOfAChronicDisease = codeOfAChronicDisease; }
    void setAttendingPhysician(string attendingPhysician) { this->attendingPhysician = attendingPhysician; }
    void write(ostream& os) // Запись объекта в бинарный файл
    {
        os.write((char*)&cardNumber, sizeof(cardNumber)); // Запись cardNumber
        os.write((char*)&codeOfAChronicDisease, sizeof(codeOfAChronicDisease)); // Запись codeOfAChronicDisease
        uint32_t len = attendingPhysician.length() + 1; // Длина с нулевым байтом
        os.write((char*)&len, sizeof(len)); // Запись длины
        os.write((char*)attendingPhysician.c_str(), len); // Запись данных
    }
    bool read(istream& in) // Чтение объекта из бинарного файла
    {
        if (!in.good()) return false;
        in.read((char*)&cardNumber, sizeof(cardNumber));   // Чтение cardNumber
        in.read((char*)&codeOfAChronicDisease, sizeof(codeOfAChronicDisease)); // Чтение codeOfAChronicDisease
        if (!in.good()) return false;
        size_t len;// Переменная для длины
        in.read((char*)&len, sizeof(len)); // Чтение длины записанной строки
        if (!in.good()) return false;
        char* buf = new char[len]; // Выделение буфера для чтения
        in.read(buf, len); // Чтение (с нулевым байтом)
        attendingPhysician = buf; // Присвоение считанной строки члену
        delete[]buf; // Освобождение памяти
        return true;
    }
};
ostream& operator<<(ostream& os, const Patient& patient) // Перепределение оператора вывода для объектов класса Patient
{
    os << "Card number: " << patient.getCardNumber() 
        << "\nCode of a chronic disease: " << patient.getCodeOfAChronicDisease() 
        << "\nAttending physician: " << patient.getAttendingPhysician();
    return os;
}
class Patients
{
private:
    vector<Patient> patients; // Вектор с пациентами
    string path; // Путь до бинарного файла
    void updateBinaryFile() // Обновление бинарного файла
    {
        ofstream fout(path, ofstream::binary);
        for (Patient patient : patients)
            patient.write(fout);
        fout.close();
    }
    void fillPatients() // Чтение из бинарного файла и запись в вектор
    {
        patients.clear();
        ifstream fin(path, ifstream::binary);
        Patient patient;
        while (patient.read(fin))
            patients.push_back(patient);
        fin.close();
    }
public:
    Patients(string path) // Констуктор
    {
        this->path = path;
        fillPatients();
    }
    void showPatients() // Вывод всех пациентов
    {
        for (uint32_t i = 0; i < patients.size(); i++)
            cout << "====Patient====\n" << patients[i] <<  "\n===============\n";
    }
    void addPatient(Patient patient) // Добавить пациента
    {
        patients.push_back(patient);
        updateBinaryFile();
    }
    void deletePatient(uint32_t cardNumber) // Удалить пациента
    {
        auto reqPatient = find_if(begin(patients), end(patients),
            [cardNumber](const Patient& patient) { return patient.getCardNumber() == cardNumber; });
        if (reqPatient != end(patients))
        {
            if (patients.size() <= 1) patients.clear();
            else
            {
                iter_swap(reqPatient, end(patients) - 1);
                patients.erase(end(patients) - 1);
            }
        }
        updateBinaryFile();
    }
    void changeAttendingPhysician(vector<uint32_t>cardNumbers, string newAttendingPhysician) // Изменить у массива пациентов лечащего врача
    {
        for (uint32_t i = 0; i < cardNumbers.size(); i++)
        {
            auto reqPatient = find_if(begin(patients), end(patients),
                [cardNumbers, i](const Patient& patient) { return patient.getCardNumber() == cardNumbers[i]; });
            (*reqPatient).setAttendingPhysician(newAttendingPhysician);
        }
        updateBinaryFile();
    }
};
int main()
{
    Patients patients("patients.txt"); // Создаем объект класса Patients
    /*
    * Добавляем пациентов
    * (При повторном запуске можно убрать 4 следующих строчки,
    * т.к пациенты уже будут записаны в файл)
    */
    patients.addPatient(Patient(0, 333, "Smirnov"));
    patients.addPatient(Patient(1, 444, "Smirnov"));
    patients.addPatient(Patient(2, 555, "Yanovsky"));
    patients.addPatient(Patient(3, 666, "Smirnov"));
    patients.deletePatient(2);
    patients.changeAttendingPhysician({ 1,3 }, "Yanovsky");
    patients.showPatients();
    system("pause");
    return 0;
}
