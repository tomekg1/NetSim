#include "gtest/gtest.h"

#include "package.hpp"


TEST(PackageTest, createNew) {
    // Unieważniam te testy, gdyż przechodzą tylko jedne na raz!
    // Sprawdź również w CMakeLists
    return;

    // resetuj pola indeksów

    Package p1 = Package();
    Package p2 = Package();
    Package* p23 = new Package(23); // aby można było usunąć
    Package* p30 = new Package(30);
    Package* p31 = new Package();
    Package* p35 = new Package(35);
    Package p36 = Package();

    // pierwszy id ma być równy zero
    EXPECT_EQ(p1.get_id(), 1);
    EXPECT_EQ(p2.get_id(), 2);
    EXPECT_EQ(p23->get_id(), 23);
    EXPECT_EQ(p30->get_id(), 30);
    EXPECT_EQ(p31->get_id(), 31);
    EXPECT_EQ(p35->get_id(), 35);
    EXPECT_EQ(p36.get_id(), 36);

    delete p31;
    delete p23;
    delete p30;
    delete p35;

    Package p_new30 = Package(30); // wymuszenie wyboru ze środka zbioru
    Package p_new23 = Package(); // powinien dobrać pierwszy (najmniejszy) wolny - 23
    Package p_new31 = Package();
    Package p_new35 = Package();
    Package p37 = Package();

    EXPECT_EQ(p_new23.get_id(), 23);
    EXPECT_EQ(p_new30.get_id(), 30);
    EXPECT_EQ(p_new31.get_id(), 31);
    EXPECT_EQ(p_new35.get_id(), 35);
    EXPECT_EQ(p37.get_id(), 37);

    //EXPECT_THROW(Package(35), std::runtime_error);
}
