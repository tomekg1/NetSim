#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include <vector>
#include <numeric>
#include <ostream>
#include <list>
#include "types.hpp"

class Package {
private:
    /**
     * Pierwszy indeks do przydzielenia pakietom.
     */
    static const size_type first_index = 1;

    /**
     * Listy przydzielonych i zwolnionych ID, używane przez metody assign_ID(), free_ID() oraz get_new_ID().
     */
    static std::list<ElementID> assigned_IDs;
    static std::list<ElementID> freed_IDs;

    /**
     * Obsługa zapamiętywania przydzielonych ID.
     * @param id ElementID - ID do zapamiętania
     */
    static void assign_ID(ElementID id);

    /**
     * Obsługa zwalniania przydzielonych ID.
     * @param id ElementID - ID do zwolnienia
     */
    static void free_ID(ElementID id);

    /**
     * Znajdowanie nowego ID do przydzielenia.
     * @return ElementID - nowe, nieużywane ID
     */
    static ElementID get_new_ID();

    /**
     * ID tego pakietu.
     */
    ElementID my_ID_;


public:
    /**
     * Konstruktor domyślny, przydziela pierwsze wolne ID.
     */
    Package();
    /**
     * Przydziela wskazane ID.
     * @param id
     */
    Package(ElementID id);
    /**
     * Konstruktor kopiujący - kopiuje dane z dostarczonego pakietu.
     * @param p Package&& - obiekt do kopiowania.
     */
    Package(Package&& p);

    /**
     * Operator przeniesienia.
     * @return Package& - przeniesiony obiekt Package.
     */
    Package& operator=(Package&&);

    /**
     * Zwraca ID pakietu.
     * @return ElementID - ID tego pakietu.
     */
    ElementID get_id() const { return my_ID_; }

    /**
     * Destruktor. Zapewnia zwalnianie ID.
     */
    ~Package();
};

#endif //NETSIM_PACKAGE_HPP
