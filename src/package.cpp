#include "package.hpp"
#include <algorithm>

/* === PRIVATE === */

std::list<ElementID> Package::assigned_IDs = {};
std::list<ElementID> Package::freed_IDs = {};


void Package::assign_ID(ElementID id) {
    // usuń id z wolnych, jeśli tam jest
    auto it = std::find(freed_IDs.cbegin(), freed_IDs.cend(), id);
    if (it != freed_IDs.cend()) freed_IDs.erase(it);

    // dodaj id do zajętych, NIEZALEŻNIE od tego, czy on tam jest
    // w ten sposób informujemy, czy są duplikaty
    assigned_IDs.insert(assigned_IDs.end(), id);
}


void Package::free_ID(ElementID id) {
    // usuń id z zajętych, jeśli tam jest
    auto it = std::find(assigned_IDs.cbegin(), assigned_IDs.cend(), id);
    if (it != assigned_IDs.cend()) assigned_IDs.erase(it);

    // dodaj id do wolnych, jeśli jeszcze go tam nie ma ORAZ nie ma duplikatu w zajętych ID
    it = std::find(assigned_IDs.cbegin(), assigned_IDs.cend(), id);
    if (it == assigned_IDs.cend()) {
        // nie ma duplikatu, szukam wolne ID
        auto it2 = std::find(freed_IDs.cbegin(), freed_IDs.cend(), id);
        if (it2 == freed_IDs.cend()) freed_IDs.insert(freed_IDs.end(), id);
    }
}


ElementID Package::get_new_ID() {
    // == nadawanie indeksu ==
    // sprawdź wolne indeksy
    if (freed_IDs.size() > 0){
        // zwróć najmniejszy wolny indeks
        auto it = std::min_element(freed_IDs.cbegin(), freed_IDs.cend());
        return *it;
    }
    else {
        // jeśli nie przydzielono do tej pory indeksów, zwróć wartość first_index
        if (assigned_IDs.size() == 0)
            return first_index;
        else {
            // znajdź największy zajęty indeks
            auto it = std::max_element(assigned_IDs.cbegin(), assigned_IDs.cend());
            // zwróć indeks większy o 1
            return (*it) + 1;
        }
    }
}


/* === PUBLIC === */

Package::Package() {
    ElementID id = get_new_ID();
    assign_ID(id);
    my_ID_ = id;
}


Package::Package(ElementID id) {
    assign_ID(id);
    my_ID_ = id;
}


Package::Package(Package &&p) {
    assign_ID(p.get_id());
    my_ID_ = p.get_id();
}

Package::~Package() {
    free_ID(get_id());
}


Package &Package::operator=(Package && p){
    assign_ID(p.get_id());
    my_ID_ = p.get_id();
    return *this;
}
