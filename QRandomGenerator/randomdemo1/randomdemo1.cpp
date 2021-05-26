// QRandomGenerator class demo and examples.

#include <random>
#include <QDebug>
#include <QRandomGenerator>
#include <QRandomGenerator64>

int main()
{
    quint32 value32;
    quint64 value64;
    int     valueInt;
    double  valueDouble;
    QRandomGenerator::result_type result;

    // Constructors
    QRandomGenerator gen1 =  QRandomGenerator(); // Default, seeded with value of 1.

    QRandomGenerator gen2 =  QRandomGenerator(0x1234); // Default, seeded with value of 0x1234.
 
    const quint32 array[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    QRandomGenerator gen3 =  QRandomGenerator(&array[0], &array[9]); // Generate a number of values.

    QRandomGenerator gen4 =  QRandomGenerator(gen1); // Copying another generator.

    QRandomGenerator gen5 = QRandomGenerator::securelySeeded(); // Securely seeded generator.

    QRandomGenerator *gen6 = QRandomGenerator::system(); // System generator.

    QRandomGenerator *gen7 = QRandomGenerator::global(); // Global generator.

    // Bounded values
    valueDouble = gen1.bounded(100.0);
    qDebug() << "Random double from 0 to 100:" << valueDouble;
    value32 = gen1.bounded(100);
    qDebug() << "Random 32-bit from 0 to 100:" << value32;
    valueInt = gen1.bounded(100);
    qDebug() << "Random int from 0 to 100:" << valueInt;
    value32 = gen1.bounded(50, 100);
    qDebug() << "Random 32-bit from 50 to 100:" << value32;
    valueInt = gen1.bounded(50, 100);
    qDebug() << "Random int from 50 to 100:" << valueInt;

    // Generate methods
    value32 = gen5.generate();
    qDebug() << "Random 32-bit:" << value32;
    value64 = gen5.generate64();
    qDebug() << "Random 64-bit:" << value64;
    valueDouble = gen5.generateDouble();
    qDebug() << "Random double:" << valueDouble;

    // fillRange
    QVector<quint32> vector;
    vector.resize(10);
    gen6->fillRange(vector.data(), vector.size());
    qDebug() << "Random vector of" << vector.size() << "32-bit elements:" << vector;

    // Operator()
    result = gen5();
    qDebug() << "operator() returned" << result;

    // Seeding
    gen2.seed(); // Default seed is 1
    gen2.seed(1234);
    std::seed_seq seq{1,2,3,4,5}; // C++11 standard seed sequence
    gen2.seed(seq);

    // Min/max
    result = gen3.min();
    qDebug() << "Min value:" << result;
    result = gen4.max();
    qDebug() << "Max value:" << result;

    // Discard next 10 values
    gen7->discard(10);

    // Comparison
    if (gen1 == gen2)
        qDebug() << "gen1 and gen2 are the same";
    if (gen1 != gen2)
        qDebug() << "gen1 and gen2 are different";

    // Assignment
    gen2 = gen1;
    if (gen1 == gen2)
        qDebug() << "gen1 and gen2 are the same";
   
    // QRandomGenerator64
    QRandomGenerator64 gen8 = QRandomGenerator64();
    result = gen8();
    qDebug() << "operator() returned" << result;

    return 0;
}
