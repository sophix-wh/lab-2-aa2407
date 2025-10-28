#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <limits>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>

using namespace std;

// Класс для логирования
class Logger {
private:
    ofstream logFile;
    
public:
    Logger(const string& filename = "log.txt") {
        logFile.open(filename, ios::app);
    }
    
    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    void log(const string& action) {
        if (logFile.is_open()) {
            time_t now = time(0);
            string timestamp = ctime(&now);
            timestamp.pop_back();
            logFile << "[" << timestamp << "] " << action << endl;
        }
        cout << "[LOG] " << action << endl;
    }
};

// Глобальный логгер
Logger logger;

// Базовый класс для объектов с уникальным ID
class IdentifiableObject {
protected:
    int id;
    
public:
    IdentifiableObject() : id(0) {}
    IdentifiableObject(int specificId) : id(specificId) {}
    
    int getId() const { return id; }
    void setId(int newId) { id = newId; }
    
    virtual ~IdentifiableObject() = default;
};

// Общие функции для работы с вводом
template<typename T>
T getValidInput(const string& prompt, T minVal = numeric_limits<T>::lowest(), T maxVal = numeric_limits<T>::max()) {
    T value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < minVal || value > maxVal) {
            cout << "Ошибка! Введите корректное значение";
            if (minVal != numeric_limits<T>::lowest() || maxVal != numeric_limits<T>::max()) {
                cout << " (";
                if (minVal != numeric_limits<T>::lowest()) cout << "min: " << minVal;
                if (maxVal != numeric_limits<T>::max()) cout << (minVal != numeric_limits<T>::lowest() ? ", " : "") << "max: " << maxVal;
                cout << ")";
            }
            cout << ": ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            break;
        }
    }
    return value;
}

string getStringInput(const string& prompt) {
    cout << prompt;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string value;
    getline(cin, value);
    return value;
}

bool getBoolInput(const string& prompt) {
    return getValidInput<int>(prompt, 0, 1) == 1;
}

// Класс Труба
class Pipe : public IdentifiableObject {
private:
    string name;
    double length;
    int diameter;
    bool underRepair;
    
public:
    Pipe() : length(0), diameter(0), underRepair(false) {}
    
    // Конструктор копирования
    Pipe(const Pipe& other) : IdentifiableObject(other.id), name(other.name), length(other.length), 
                             diameter(other.diameter), underRepair(other.underRepair) {}
    
    // Оператор присваивания
    Pipe& operator=(const Pipe& other) {
        if (this != &other) {
            id = other.id;
            name = other.name;
            length = other.length;
            diameter = other.diameter;
            underRepair = other.underRepair;
        }
        return *this;
    }
    
    void readFromConsole(int newId) {
        id = newId;
        name = getStringInput("Введите название трубы: ");
        length = getValidInput<double>("Введите длину трубы (км): ", 0.01);
        diameter = getValidInput<int>("Введите диаметр трубы (мм): ", 1);
        underRepair = getBoolInput("Труба в ремонте? (0 - нет, 1 - да): ");
        
        logger.log("Создана труба: ID=" + to_string(getId()) + ", название='" + name + "'");
    }
    
    void print() const {
        cout << "Труба ID: " << getId() << "\n  Название: " << name 
             << "\n  Длина: " << length << " км\n  Диаметр: " << diameter << " мм"
             << "\n  В ремонте: " << (underRepair ? "да" : "нет") << "\n";
    }
    
    void toggleRepair() {
        underRepair = !underRepair;
        logger.log("Труба ID=" + to_string(getId()) + " - статус ремонта изменен на: " + 
                  (underRepair ? "в ремонте" : "работает"));
    }
    
    bool matchesNameFilter(const string& nameFilter) const {
        return nameFilter.empty() || name.find(nameFilter) != string::npos;
    }
    
    bool matchesRepairFilter(bool inRepair) const {
        return underRepair == inRepair;
    }
    
    // Геттеры
    string getName() const { return name; }
    double getLength() const { return length; }
    int getDiameter() const { return diameter; }
    bool isUnderRepair() const { return underRepair; }
    
    // Сеттеры
    void setName(const string& newName) { name = newName; }
    void setLength(double newLength) { length = newLength; }
    void setDiameter(int newDiameter) { diameter = newDiameter; }
    void setUnderRepair(bool repair) { underRepair = repair; }
};

// Класс Компрессорная станция
class CompressorStation : public IdentifiableObject {
private:
    string name;
    int totalWorkshops;
    int workingWorkshops;
    string classification;
    
public:
    CompressorStation() : totalWorkshops(0), workingWorkshops(0) {}
    
    // Конструктор копирования
    CompressorStation(const CompressorStation& other) : IdentifiableObject(other.id), name(other.name), 
                                                       totalWorkshops(other.totalWorkshops), 
                                                       workingWorkshops(other.workingWorkshops), 
                                                       classification(other.classification) {}
    
    // Оператор присваивания
    CompressorStation& operator=(const CompressorStation& other) {
        if (this != &other) {
            id = other.id;
            name = other.name;
            totalWorkshops = other.totalWorkshops;
            workingWorkshops = other.workingWorkshops;
            classification = other.classification;
        }
        return *this;
    }
    
    void readFromConsole(int newId) {
        id = newId;
        name = getStringInput("Введите название компрессорной станции: ");
        totalWorkshops = getValidInput<int>("Введите общее количество цехов: ", 1);
        workingWorkshops = getValidInput<int>("Введите количество работающих цехов: ", 0, totalWorkshops);
        classification = getStringInput("Введите классификацию: ");
        
        logger.log("Создана КС: ID=" + to_string(getId()) + ", название='" + name + 
                  "', цехов: " + to_string(workingWorkshops) + "/" + to_string(totalWorkshops));
    }
    
    void print() const {
        cout << "Компрессорная станция ID: " << getId() << "\n  Название: " << name 
             << "\n  Цехи: " << workingWorkshops << "/" << totalWorkshops << " работают"
             << "\n  Незадействовано: " << getUnusedPercentage() << "%"
             << "\n  Классификация: " << classification << "\n";
    }
    
    bool startWorkshop() {
        if (workingWorkshops < totalWorkshops) {
            workingWorkshops++;
            logger.log("КС ID=" + to_string(getId()) + " - запущен цех. Теперь " + 
                      to_string(workingWorkshops) + "/" + to_string(totalWorkshops) + " работают");
            return true;
        }
        return false;
    }
    
    bool stopWorkshop() {
        if (workingWorkshops > 0) {
            workingWorkshops--;
            logger.log("КС ID=" + to_string(getId()) + " - остановлен цех. Теперь " + 
                      to_string(workingWorkshops) + "/" + to_string(totalWorkshops) + " работают");
            return true;
        }
        return false;
    }
    
    double getUnusedPercentage() const {
        return totalWorkshops == 0 ? 0 : ((totalWorkshops - workingWorkshops) * 100.0) / totalWorkshops;
    }
    
    bool matchesNameFilter(const string& nameFilter) const {
        return nameFilter.empty() || name.find(nameFilter) != string::npos;
    }
    
    bool matchesUnusedPercentageFilter(double minPercentage) const {
        return getUnusedPercentage() >= minPercentage;
    }
    
    // Геттеры
    string getName() const { return name; }
    int getTotalWorkshops() const { return totalWorkshops; }
    int getWorkingWorkshops() const { return workingWorkshops; }
    string getClassification() const { return classification; }
    
    // Сеттеры
    void setName(const string& newName) { name = newName; }
    void setTotalWorkshops(int count) { totalWorkshops = count; }
    void setWorkingWorkshops(int count) { workingWorkshops = count; }
    void setClassification(const string& newClassification) { classification = newClassification; }
};

// Базовый класс менеджера
template<typename T>
class BaseManager {
protected:
    map<int, T> objects;
    int nextId;
    
public:
    BaseManager() : nextId(1) {}
    
    virtual void addObject() = 0;
    
    void deleteObject(int id) {
        auto it = objects.find(id);
        if (it != objects.end()) {
            logger.log("Удален объект: ID=" + to_string(id));
            objects.erase(it);
            cout << "Объект удален!\n";
        } else {
            cout << "Объект с ID=" << id << " не найден!\n";
        }
    }
    
    void showAllObjects() const {
        if (objects.empty()) {
            cout << "Объекты отсутствуют.\n";
            return;
        }
        for (const auto& entry : objects) {
            entry.second.print();
            cout << "---\n";
        }
    }
    
    T* getObject(int id) {
        auto it = objects.find(id);
        return it != objects.end() ? &it->second : nullptr;
    }
    
    bool objectExists(int id) const {
        return objects.find(id) != objects.end();
    }
    
    vector<int> getAllObjectIds() const {
        vector<int> ids;
        for (const auto& entry : objects) {
            ids.push_back(entry.first);
        }
        return ids;
    }
    
    bool hasObjects() const {
        return !objects.empty();
    }
    
    int getObjectCount() const {
        return objects.size();
    }
    
    const map<int, T>& getObjects() const {
        return objects;
    }
    
    int getNextId() {
        return nextId;
    }
    
    void updateNextId() {
        if (objects.empty()) {
            nextId = 1;
        } else {
            int maxId = 0;
            for (const auto& entry : objects) {
                if (entry.first > maxId) {
                    maxId = entry.first;
                }
            }
            nextId = maxId + 1;
        }
    }

protected:
    template<typename Predicate>
    set<int> findObjects(Predicate pred) const {
        set<int> result;
        for (const auto& entry : objects) {
            if (pred(entry.second)) {
                result.insert(entry.first);
            }
        }
        return result;
    }
};

// Менеджер для труб
class PipeManager : public BaseManager<Pipe> {
public:
    void addObject() override {
        Pipe pipe;
        pipe.readFromConsole(nextId);
        objects[nextId] = pipe;
        nextId++;
        cout << "Труба создана с ID: " << (nextId - 1) << "\n";
    }
    
    void addObject(const Pipe& pipe) {
        objects[pipe.getId()] = pipe;
        if (pipe.getId() >= nextId) {
            nextId = pipe.getId() + 1;
        }
    }
    
    void editPipe(int id) {
        auto pipe = getObject(id);
        if (pipe) {
            pipe->toggleRepair();
        } else {
            cout << "Труба с ID=" << id << " не найдена!\n";
        }
    }
    
    set<int> findPipesByName(const string& nameFilter) const {
        return findObjects([&](const Pipe& pipe) { return pipe.matchesNameFilter(nameFilter); });
    }
    
    set<int> findPipesByRepair(bool inRepair) const {
        return findObjects([&](const Pipe& pipe) { return pipe.matchesRepairFilter(inRepair); });
    }
    
    void batchEditRepair(const set<int>& pipeIds) {
        for (int id : pipeIds) editPipe(id);
        logger.log("Пакетное редактирование: изменено " + to_string(pipeIds.size()) + " труб");
    }
    
    void loadObjects(const map<int, Pipe>& newObjects) {
        objects = newObjects;
        updateNextId();
    }
};

// Менеджер для станций
class StationManager : public BaseManager<CompressorStation> {
public:
    void addObject() override {
        CompressorStation station;
        station.readFromConsole(nextId);
        objects[nextId] = station;
        nextId++;
        cout << "Компрессорная станция создана с ID: " << (nextId - 1) << "\n";
    }
    
    void addObject(const CompressorStation& station) {
        objects[station.getId()] = station;
        if (station.getId() >= nextId) {
            nextId = station.getId() + 1;
        }
    }
    
    void editStationWorkshops(int id, int action) {
        auto station = getObject(id);
        if (station) {
            bool success = (action == 1) ? station->startWorkshop() : station->stopWorkshop();
            if (!success) cout << (action == 1 ? "Все цехи уже работают!\n" : "Нет работающих цехов!\n");
        } else {
            cout << "Станция с ID=" << id << " не найдена!\n";
        }
    }
    
    set<int> findStationsByName(const string& nameFilter) const {
        return findObjects([&](const CompressorStation& station) { return station.matchesNameFilter(nameFilter); });
    }
    
    set<int> findStationsByUnusedPercentage(double minPercentage) const {
        return findObjects([&](const CompressorStation& station) { return station.matchesUnusedPercentageFilter(minPercentage); });
    }
    
    void loadObjects(const map<int, CompressorStation>& newObjects) {
        objects = newObjects;
        updateNextId();
    }
};

// Отдельные функции сохранения для каждого типа
void savePipes(ofstream& file, const map<int, Pipe>& pipes) {
    file << "Pipes:" << pipes.size() << "\n";
    for (const auto& entry : pipes) {
        const Pipe& pipe = entry.second;
        file << pipe.getId() << "\n" << pipe.getName() << "\n"
             << pipe.getLength() << "\n" << pipe.getDiameter() << "\n" << pipe.isUnderRepair() << "\n";
    }
}

void saveStations(ofstream& file, const map<int, CompressorStation>& stations) {
    file << "Stations:" << stations.size() << "\n";
    for (const auto& entry : stations) {
        const CompressorStation& station = entry.second;
        file << station.getId() << "\n" << station.getName() << "\n"
             << station.getTotalWorkshops() << "\n" << station.getWorkingWorkshops() << "\n" << station.getClassification() << "\n";
    }
}

// Отдельные функции загрузки для каждого типа
void loadPipes(ifstream& file, map<int, Pipe>& pipes) {
    string line;
    getline(file, line);
    if (line.find("Pipes:") != string::npos) {
        int count = stoi(line.substr(6));
        for (int i = 0; i < count; i++) {
            getline(file, line);
            int id = stoi(line);
            Pipe pipe;
            pipe.setId(id);
            
            getline(file, line); pipe.setName(line);
            getline(file, line); pipe.setLength(stod(line));
            getline(file, line); pipe.setDiameter(stoi(line));
            getline(file, line); pipe.setUnderRepair(stoi(line));
            
            pipes[id] = pipe;
        }
    }
}

void loadStations(ifstream& file, map<int, CompressorStation>& stations) {
    string line;
    getline(file, line);
    if (line.find("Stations:") != string::npos) {
        int count = stoi(line.substr(9));
        for (int i = 0; i < count; i++) {
            getline(file, line);
            int id = stoi(line);
            CompressorStation station;
            station.setId(id);
            
            getline(file, line); station.setName(line);
            getline(file, line); station.setTotalWorkshops(stoi(line));
            getline(file, line); station.setWorkingWorkshops(stoi(line));
            getline(file, line); station.setClassification(line);
            
            stations[id] = station;
        }
    }
}

void saveToFile(const PipeManager& pipeManager, const StationManager& stationManager, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка создания файла!\n";
        return;
    }
    
    savePipes(file, pipeManager.getObjects());
    saveStations(file, stationManager.getObjects());
    
    file.close();
    logger.log("Данные сохранены в файл: " + filename);
    cout << "Данные сохранены в файл: " << filename << "\n";
}

void loadFromFile(PipeManager& pipeManager, StationManager& stationManager, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла!\n";
        return;
    }
    
    map<int, Pipe> loadedPipes;
    map<int, CompressorStation> loadedStations;
    
    loadPipes(file, loadedPipes);
    loadStations(file, loadedStations);
    
    file.close();
    
    pipeManager.loadObjects(loadedPipes);
    stationManager.loadObjects(loadedStations);
    
    logger.log("Данные загружены из файла: " + filename);
    cout << "Данные загружены из файла: " << filename << "\n";
    cout << "Загружено труб: " << loadedPipes.size() << ", станций: " << loadedStations.size() << "\n";
}

// Функции пользовательского интерфейса
void showPipeSearchMenu(PipeManager& pipeManager) {
    if (!pipeManager.hasObjects()) {
        cout << "Ошибка! Нет созданных труб.\n";
        return;
    }
    
    cout << "\n=== Поиск труб ===\n";
    cout << "1. Поиск по названию\n";
    cout << "2. Поиск по статусу ремонта\n";
    cout << "0. Назад\n";
    cout << "Выберите тип поиска: ";
    
    int choice = getValidInput<int>("", 0, 2);
    
    switch (choice) {
        case 1: {
            string nameFilter = getStringInput("Введите название или часть названия для поиска: ");
            set<int> foundPipes = pipeManager.findPipesByName(nameFilter);
            cout << "\nНайдено труб: " << foundPipes.size() << "\n";
            for (int id : foundPipes) {
                if (auto pipe = pipeManager.getObject(id)) {
                    pipe->print();
                    cout << "---\n";
                }
            }
            break;
        }
        case 2: {
            cout << "Поиск труб по статусу ремонта:\n";
            cout << "1. Трубы в ремонте\n";
            cout << "2. Трубы не в ремонте\n";
            int repairChoice = getValidInput<int>("Выберите статус: ", 1, 2);
            bool inRepair = (repairChoice == 1);
            
            set<int> foundPipes = pipeManager.findPipesByRepair(inRepair);
            cout << "\nНайдено труб: " << foundPipes.size() << "\n";
            for (int id : foundPipes) {
                if (auto pipe = pipeManager.getObject(id)) {
                    pipe->print();
                    cout << "---\n";
                }
            }
            break;
        }
        case 0:
            return;
    }
}

void showStationSearchMenu(StationManager& stationManager) {
    if (!stationManager.hasObjects()) {
        cout << "Ошибка! Нет созданных компрессорных станций.\n";
        return;
    }
    
    cout << "\n=== Поиск компрессорных станций ===\n";
    cout << "1. Поиск по названию\n";
    cout << "2. Поиск по проценту незадействованных цехов\n";
    cout << "0. Назад\n";
    cout << "Выберите тип поиска: ";
    
    int choice = getValidInput<int>("", 0, 2);
    
    switch (choice) {
        case 1: {
            string nameFilter = getStringInput("Введите название или часть названия для поиска: ");
            set<int> foundStations = stationManager.findStationsByName(nameFilter);
            cout << "\nНайдено станций: " << foundStations.size() << "\n";
            for (int id : foundStations) {
                if (auto station = stationManager.getObject(id)) {
                    station->print();
                    cout << "---\n";
                }
            }
            break;
        }
        case 2: {
            double minPercentage = getValidInput<double>("Введите минимальный процент незадействованных цехов: ", 0.0, 100.0);
            set<int> foundStations = stationManager.findStationsByUnusedPercentage(minPercentage);
            cout << "\nНайдено станций: " << foundStations.size() << "\n";
            for (int id : foundStations) {
                if (auto station = stationManager.getObject(id)) {
                    station->print();
                    cout << "---\n";
                }
            }
            break;
        }
        case 0:
            return;
    }
}

void showBatchEditMenu(PipeManager& pipeManager) {
    if (!pipeManager.hasObjects()) {
        cout << "Ошибка! Нет созданных труб.\n";
        return;
    }
    
    cout << "\n=== Пакетное редактирование труб ===\n";
    
    string nameFilter = getStringInput("Фильтр по названию (оставьте пустым чтобы пропустить): ");
    char repairChoice;
    cout << "Фильтр по ремонту? (y/n): ";
    cin >> repairChoice;
    
    bool useRepairFilter = (repairChoice == 'y' || repairChoice == 'Y');
    bool repairFilter = false;
    if (useRepairFilter) {
        repairFilter = getBoolInput("Трубы в ремонте? (0 - нет, 1 - да): ");
    }
    
    set<int> foundPipes;
    if (useRepairFilter) {
        foundPipes = pipeManager.findPipesByRepair(repairFilter);
        if (!nameFilter.empty()) {
            set<int> nameFiltered = pipeManager.findPipesByName(nameFilter);
            set<int> intersection;
            set_intersection(foundPipes.begin(), foundPipes.end(),
                           nameFiltered.begin(), nameFiltered.end(),
                           inserter(intersection, intersection.begin()));
            foundPipes = intersection;
        }
    } else {
        foundPipes = pipeManager.findPipesByName(nameFilter);
    }
    
    if (foundPipes.empty()) {
        cout << "Трубы по заданному фильтру не найдены.\n";
        return;
    }
    
    cout << "\nНайдено труб: " << foundPipes.size() << "\n";
    for (int id : foundPipes) {
        if (auto pipe = pipeManager.getObject(id)) {
            pipe->print();
        }
    }
    
    cout << "\nВыберите действие:\n";
    cout << "1. Изменить статус ремонта ВСЕХ найденных труб\n";
    cout << "2. Выбрать конкретные трубы для редактирования\n";
    cout << "0. Отмена\n";
    
    int choice = getValidInput<int>("", 0, 2);
    
    if (choice == 1) {
        pipeManager.batchEditRepair(foundPipes);
        cout << "Статус ремонта изменен для всех " << foundPipes.size() << " труб.\n";
    } else if (choice == 2) {
        set<int> selectedPipes;
        cout << "Введите ID труб для редактирования (0 для завершения):\n";
        
        int selectedId;
        while (true) {
            selectedId = getValidInput<int>("");
            if (selectedId == 0) break;
            
            if (foundPipes.count(selectedId)) {
                selectedPipes.insert(selectedId);
                cout << "Труба ID=" << selectedId << " добавлена в выборку.\n";
            } else {
                cout << "ID=" << selectedId << " не найден в результатах поиска.\n";
            }
        }
        
        if (!selectedPipes.empty()) {
            pipeManager.batchEditRepair(selectedPipes);
            cout << "Статус ремонта изменен для " << selectedPipes.size() << " труб.\n";
        }
    }
}

void showSearchMenu(PipeManager& pipeManager, StationManager& stationManager) {
    cout << "\n=== Поиск объектов ===\n";
    cout << "1. Поиск труб";
    if (!pipeManager.hasObjects()) cout << " (недоступно - нет труб)";
    cout << "\n";
    cout << "2. Поиск компрессорных станций";
    if (!stationManager.hasObjects()) cout << " (недоступно - нет станций)";
    cout << "\n";
    cout << "0. Назад\n";
    
    int choice = getValidInput<int>("Выберите действие: ", 0, 2);
    
    if (choice == 1 && pipeManager.hasObjects()) {
        showPipeSearchMenu(pipeManager);
    } else if (choice == 2 && stationManager.hasObjects()) {
        showStationSearchMenu(stationManager);
    } else if (choice != 0) {
        cout << "Ошибка! Нет созданных объектов для поиска.\n";
    }
}

// Главное меню
void showMainMenu(PipeManager& pipeManager, StationManager& stationManager) {
    while (true) {
        cout << "\n=== Газотранспортная система ===\n";
        cout << "1. Добавить трубу\n";
        cout << "2. Добавить компрессорную станцию\n";
        cout << "3. Просмотр всех объектов\n";
        
        cout << "4. Редактировать трубу";
        if (!pipeManager.hasObjects()) cout << " (недоступно - нет труб)";
        cout << "\n";
        
        cout << "5. Редактировать компрессорную станцию";
        if (!stationManager.hasObjects()) cout << " (недоступно - нет станций)";
        cout << "\n";
        
        cout << "6. Удалить объект";
        if (!pipeManager.hasObjects() && !stationManager.hasObjects()) cout << " (недоступно - нет объектов)";
        cout << "\n";
        
        cout << "7. Поиск объектов";
        if (!pipeManager.hasObjects() && !stationManager.hasObjects()) cout << " (недоступно - нет объектов)";
        cout << "\n";
        
        cout << "8. Пакетное редактирование труб";
        if (!pipeManager.hasObjects()) cout << " (недоступно - нет труб)";
        cout << "\n";
        
        cout << "9. Сохранить данные";
        if (!pipeManager.hasObjects() && !stationManager.hasObjects()) cout << " (недоступно - нет данных)";
        cout << "\n";
        
        cout << "10. Загрузить данные\n";
        cout << "0. Выход\n";
        
        int choice = getValidInput<int>("Выберите действие: ", 0, 10);
        
        switch (choice) {
            case 0:
                cout << "Выход из программы.\n";
                logger.log("Завершение работы программы");
                return;
            case 1:
                pipeManager.addObject();
                break;
            case 2:
                stationManager.addObject();
                break;
            case 3:
                cout << "\n=== Трубы ===\n";
                pipeManager.showAllObjects();
                cout << "\n=== Компрессорные станции ===\n";
                stationManager.showAllObjects();
                break;
            case 4:
                if (!pipeManager.hasObjects()) {
                    cout << "Ошибка! Нет созданных труб.\n";
                    break;
                }
                pipeManager.editPipe(getValidInput<int>("Введите ID трубы для редактирования: "));
                break;
            case 5:
                if (!stationManager.hasObjects()) {
                    cout << "Ошибка! Нет созданных станций.\n";
                    break;
                }
                stationManager.editStationWorkshops(
                    getValidInput<int>("Введите ID станции для редактирования: "),
                    getValidInput<int>("Выберите действие: 1 - запустить цех, 2 - остановить цех: ", 1, 2)
                );
                break;
            case 6: {
                if (!pipeManager.hasObjects() && !stationManager.hasObjects()) {
                    cout << "Ошибка! Нет созданных объектов.\n";
                    break;
                }
                int type = getValidInput<int>("Удалить: 1 - трубу, 2 - станцию: ", 1, 2);
                int id = getValidInput<int>("Введите ID: ");
                (type == 1) ? pipeManager.deleteObject(id) : stationManager.deleteObject(id);
                break;
            }
            case 7:
                if (!pipeManager.hasObjects() && !stationManager.hasObjects()) {
                    cout << "Ошибка! Нет созданных объектов.\n";
                    break;
                }
                showSearchMenu(pipeManager, stationManager);
                break;
            case 8:
                if (!pipeManager.hasObjects()) {
                    cout << "Ошибка! Нет созданных труб.\n";
                    break;
                }
                showBatchEditMenu(pipeManager);
                break;
            case 9:
                if (!pipeManager.hasObjects() && !stationManager.hasObjects()) {
                    cout << "Ошибка! Нет данных для сохранения.\n";
                    break;
                }
                saveToFile(pipeManager, stationManager, getStringInput("Введите имя файла для сохранения: "));
                break;
            case 10:
                loadFromFile(pipeManager, stationManager, getStringInput("Введите имя файла для загрузки: "));
                break;
        }
    }
}

int main() {
    cout << "Газотранспортная система - управление трубами и компрессорными станциями\n";
    logger.log("Запуск программы");
    
    PipeManager pipeManager;
    StationManager stationManager;
    
    showMainMenu(pipeManager, stationManager);
    
    return 0;
}