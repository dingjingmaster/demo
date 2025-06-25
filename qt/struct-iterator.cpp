#include <QtCore>
#include <QCoreApplication>
#include <QList>
#include <QVector>
#include <QString>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

// 定义一个示例结构体
struct Person {
    QString name;
    int age;
    QString city;
    double salary;
    
    // 默认构造函数
    Person() : age(0), salary(0.0) {}
    
    // 带参数构造函数
    Person(const QString& n, int a, const QString& c, double s)
        : name(n), age(a), city(c), salary(s) {}
    
    // 转换为字符串的方法
    QString toString() const {
        return QString("Person{姓名: %1, 年龄: %2, 城市: %3, 薪资: %4}")
            .arg(name).arg(age).arg(city).arg(salary, 0, 'f', 2);
    }
    
    // 转换为JSON字符串
    QString toJsonString() const {
        QJsonObject obj;
        obj["name"] = name;
        obj["age"] = age;
        obj["city"] = city;
        obj["salary"] = salary;
        
        QJsonDocument doc(obj);
        return doc.toJson(QJsonDocument::Compact);
    }
    
    // 转换为CSV格式
    QString toCsvString() const {
        return QString("%1,%2,%3,%4").arg(name).arg(age).arg(city).arg(salary, 0, 'f', 2);
    }
};

// 自定义迭代器类模板
template<typename Container, typename T>
class StructStringIterator {
public:
    // 定义字符串转换函数类型
    using StringConverter = std::function<QString(const T&)>;
    
private:
    typename Container::const_iterator current;
    typename Container::const_iterator end;
    StringConverter converter;
    
public:
    // 构造函数
    StructStringIterator(const Container& container, StringConverter conv)
        : current(container.begin()), end(container.end()), converter(conv) {}
    
    // 构造函数重载 - 使用默认toString方法
    StructStringIterator(const Container& container)
        : current(container.begin()), end(container.end()),
          converter([](const T& item) { return item.toString(); }) {}
    
    // 检查是否有下一个元素
    bool hasNext() const {
        return current != end;
    }
    
    // 获取下一个元素的字符串表示
    QString next() {
        if (current != end) {
            QString result = converter(*current);
            ++current;
            return result;
        }
        return QString();
    }
    
    // 重置迭代器到开始位置
    void reset(const Container& container) {
        current = container.begin();
        end = container.end();
    }
    
    // 获取剩余元素数量
    int remaining() const {
        return std::distance(current, end);
    }
    
    // 跳过指定数量的元素
    void skip(int count) {
        for (int i = 0; i < count && current != end; ++i) {
            ++current;
        }
    }
    
    // 获取所有剩余元素的字符串表示
    QStringList getAllRemaining() {
        QStringList result;
        while (hasNext()) {
            result.append(next());
        }
        return result;
    }
};

// 便捷的工厂函数
template<typename Container, typename T>
StructStringIterator<Container, T> createStructIterator(
    const Container& container,
    std::function<QString(const T&)> converter = nullptr) {
    
    if (converter) {
        return StructStringIterator<Container, T>(container, converter);
    } else {
        return StructStringIterator<Container, T>(container);
    }
}

// 过滤迭代器 - 只处理满足条件的元素
template<typename Container, typename T>
class FilteredStructStringIterator {
private:
    typename Container::const_iterator current;
    typename Container::const_iterator end;
    std::function<QString(const T&)> converter;
    std::function<bool(const T&)> filter;
    
    // 移动到下一个满足条件的元素
    void moveToNext() {
        while (current != end && !filter(*current)) {
            ++current;
        }
    }
    
public:
    FilteredStructStringIterator(const Container& container,
                                std::function<bool(const T&)> f,
                                std::function<QString(const T&)> conv = nullptr)
        : current(container.begin()), end(container.end()), filter(f) {
        
        if (conv) {
            converter = conv;
        } else {
            converter = [](const T& item) { return item.toString(); };
        }
        
        moveToNext(); // 移动到第一个满足条件的元素
    }
    
    bool hasNext() const {
        return current != end;
    }
    
    QString next() {
        if (current != end) {
            QString result = converter(*current);
            ++current;
            moveToNext();
            return result;
        }
        return QString();
    }
    
    QStringList getAllRemaining() {
        QStringList result;
        while (hasNext()) {
            result.append(next());
        }
        return result;
    }
};

// 演示函数
void demonstrateStructIterator() {
    qDebug() << "=== 结构体迭代器演示 ===\n";
    
    // 创建测试数据
    QList<Person> people;
    people.append(Person("张三", 25, "北京", 8000.50));
    people.append(Person("李四", 30, "上海", 12000.75));
    people.append(Person("王五", 28, "广州", 9500.00));
    people.append(Person("赵六", 35, "深圳", 15000.25));
    people.append(Person("钱七", 22, "杭州", 7000.00));
    
    qDebug() << "原始数据:";
    for (const auto& person : people) {
        qDebug() << "  " << person.toString();
    }
    qDebug() << "";
    
    // 1. 使用默认toString方法的迭代器
    qDebug() << "1. 使用默认toString迭代器:";
    auto iterator1 = createStructIterator<QList<Person>, Person>(people);
    int count = 1;
    while (iterator1.hasNext()) {
        qDebug() << QString("  [%1] %2").arg(count++).arg(iterator1.next());
    }
    qDebug() << "";
    
    // 2. 使用JSON格式转换的迭代器
    qDebug() << "2. 使用JSON格式迭代器:";
    auto jsonConverter = [](const Person& p) { return p.toJsonString(); };
    auto iterator2 = createStructIterator<QList<Person>, Person>(people, jsonConverter);
    count = 1;
    while (iterator2.hasNext()) {
        qDebug() << QString("  [%1] %2").arg(count++).arg(iterator2.next());
    }
    qDebug() << "";
    
    // 3. 使用CSV格式转换的迭代器
    qDebug() << "3. 使用CSV格式迭代器:";
    auto csvConverter = [](const Person& p) { return p.toCsvString(); };
    auto iterator3 = createStructIterator<QList<Person>, Person>(people, csvConverter);
    qDebug() << "  CSV头: 姓名,年龄,城市,薪资";
    count = 1;
    while (iterator3.hasNext()) {
        qDebug() << QString("  [%1] %2").arg(count++).arg(iterator3.next());
    }
    qDebug() << "";
    
    // 4. 使用自定义格式转换的迭代器
    qDebug() << "4. 使用自定义格式迭代器:";
    auto customConverter = [](const Person& p) {
        return QString("【%1】来自%2，%3岁，月薪￥%4")
            .arg(p.name).arg(p.city).arg(p.age).arg(p.salary, 0, 'f', 0);
    };
    auto iterator4 = createStructIterator<QList<Person>, Person>(people, customConverter);
    count = 1;
    while (iterator4.hasNext()) {
        qDebug() << QString("  [%1] %2").arg(count++).arg(iterator4.next());
    }
    qDebug() << "";
    
    // 5. 使用过滤迭代器 - 只显示薪资大于8000的人员
    qDebug() << "5. 过滤迭代器 (薪资 > 8000):";
    auto salaryFilter = [](const Person& p) { return p.salary > 8000; };
    FilteredStructStringIterator<QList<Person>, Person> filteredIterator(
        people, salaryFilter, customConverter);
    count = 1;
    while (filteredIterator.hasNext()) {
        qDebug() << QString("  [%1] %2").arg(count++).arg(filteredIterator.next());
    }
    qDebug() << "";
    
    // 6. 使用过滤迭代器 - 只显示年龄小于30的人员
    qDebug() << "6. 过滤迭代器 (年龄 < 30):";
    auto ageFilter = [](const Person& p) { return p.age < 30; };
    FilteredStructStringIterator<QList<Person>, Person> youngIterator(
        people, ageFilter);
    QStringList youngPeople = youngIterator.getAllRemaining();
    for (int i = 0; i < youngPeople.size(); ++i) {
        qDebug() << QString("  [%1] %2").arg(i+1).arg(youngPeople[i]);
    }
    qDebug() << "";
    
    // 7. 迭代器状态管理演示
    qDebug() << "7. 迭代器状态管理演示:";
    auto iterator5 = createStructIterator<QList<Person>, Person>(people);
    qDebug() << "  剩余元素数量:" << iterator5.remaining();
    
    qDebug() << "  获取前2个元素:";
    for (int i = 0; i < 2 && iterator5.hasNext(); ++i) {
        qDebug() << QString("    [%1] %2").arg(i+1).arg(iterator5.next());
    }
    
    qDebug() << "  跳过1个元素...";
    iterator5.skip(1);
    qDebug() << "  剩余元素数量:" << iterator5.remaining();
    
    qDebug() << "  获取所有剩余元素:";
    QStringList remaining = iterator5.getAllRemaining();
    for (int i = 0; i < remaining.size(); ++i) {
        qDebug() << QString("    [%1] %2").arg(i+1).arg(remaining[i]);
    }
}

// 演示其他容器类型
void demonstrateWithDifferentContainers() {
    qDebug() << "\n=== 不同容器类型演示 ===\n";
    
    // 使用QVector
    QVector<Person> vectorPeople;
    vectorPeople.append(Person("Vector张", 26, "成都", 8500.00));
    vectorPeople.append(Person("Vector李", 31, "重庆", 11000.00));
    
    qDebug() << "QVector容器:";
    auto vectorIterator = createStructIterator<QVector<Person>, Person>(vectorPeople);
    int count = 1;
    while (vectorIterator.hasNext()) {
        qDebug() << QString("  [%1] %2").arg(count++).arg(vectorIterator.next());
    }
    qDebug() << "";
    
    // 使用std::vector (需要确保与Qt容器兼容)
    std::vector<Person> stdVectorPeople;
    stdVectorPeople.push_back(Person("Std张", 27, "西安", 9000.00));
    stdVectorPeople.push_back(Person("Std李", 32, "武汉", 10500.00));
    
    qDebug() << "std::vector容器:";
    auto stdIterator = createStructIterator<std::vector<Person>, Person>(stdVectorPeople);
    count = 1;
    while (stdIterator.hasNext()) {
        qDebug() << QString("  [%1] %2").arg(count++).arg(stdIterator.next());
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    qDebug() << "=== Qt 结构体字符串迭代器示例 ===\n";
    
    // 演示基本功能
    demonstrateStructIterator();
    
    // 演示不同容器类型
    demonstrateWithDifferentContainers();
    
    qDebug() << "\n=== 程序完成 ===";
    qDebug() << "此示例展示了如何创建自定义迭代器来遍历容器中的结构体";
    qDebug() << "并将结构体数据转换为不同格式的字符串表示。";
    
    return 0;
} 