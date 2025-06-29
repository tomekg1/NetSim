#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP

//#include "config.hpp"
#include "types.hpp"
#include "nodes.hpp"
#include <list>
#include <sstream>

template <class Node>
class NodeCollection{

    using node_list = typename std::list<Node>;

private:
    node_list my_nodes;

public:
    using iterator = typename node_list::iterator;
    using const_iterator = typename node_list::const_iterator;

    void add(Node&& node){
        my_nodes.push_back(std::move(node));
    }

    iterator find_by_id(ElementID id){
        for(auto iter = my_nodes.begin(); iter != my_nodes.end(); iter++){
            if(iter->get_id() == id){
                return iter;
            }
        }
        return my_nodes.end();
    }

    const_iterator find_by_id(ElementID id) const {
        for(auto iter = my_nodes.cbegin(); iter != my_nodes.cend(); iter++){
            if(iter->get_id() == id){
                return iter;
            }
        }
        return my_nodes.cend();
    }

    void remove_by_id(ElementID id){
        auto iter = find_by_id(id);
        if(iter != my_nodes.end()){
            my_nodes.erase(iter);
        }
    }

    const_iterator begin() const { return my_nodes.cbegin(); }
    const_iterator end() const { return my_nodes.cend(); }
    const_iterator cbegin() const { return my_nodes.cbegin(); }
    const_iterator cend() const { return my_nodes.cend(); }
    iterator begin() { return my_nodes.begin(); }
    iterator end() { return my_nodes.end(); }

};


enum kolor_wezla_enum{
    NIEODWIEDZONY, ODWIEDZONY, ZWERYFIKOWANY
};

using kolor_typ = std::map<PackageSender*, kolor_wezla_enum>;

class Factory {
private:
    NodeCollection<Ramp> ramps_;
    NodeCollection<Worker> workers_;
    NodeCollection<Storehouse> storehouses_;

    void remove_receiver(NodeCollection<Worker>& collection, ElementID id);
    void remove_receiver(NodeCollection<Storehouse>& collection, ElementID id);

    //bool czy_nadawca_posiada_osiagalny_magazyn(PackageSender* nadawca, kolor_typ* kolor);

public:

    /* === Ramps === */
    void add_ramp(Ramp&& r) {ramps_.add(std::move(r));};
    void remove_ramp(ElementID id) {ramps_.remove_by_id(id);};
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return ramps_.find_by_id(id);};
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return ramps_.find_by_id(id);};
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const {return ramps_.cbegin();};
    NodeCollection<Ramp>::const_iterator ramp_cend () const {return ramps_.cend();};
    NodeCollection<Ramp>::iterator ramp_begin() {return ramps_.begin();};
    NodeCollection<Ramp>::iterator ramp_end () {return ramps_.end();};

    /* === Workers === */
    void add_worker(Worker&& w) {workers_.add(std::move(w));};
    void remove_worker(ElementID id) {remove_receiver(workers_, id);};
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {return workers_.find_by_id(id);};
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return workers_.find_by_id(id);};
    NodeCollection<Worker>::const_iterator worker_cbegin() const {return workers_.cbegin();};
    NodeCollection<Worker>::const_iterator worker_cend () const {return workers_.cend();};
    NodeCollection<Worker>::iterator worker_begin() {return workers_.begin();};
    NodeCollection<Worker>::iterator worker_end () {return workers_.end();};

    /* === Storehouses === */
    void add_storehouse(Storehouse&& s) {storehouses_.add(std::move(s));};
    void remove_storehouse(ElementID id) {remove_receiver(storehouses_, id);};
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return storehouses_.find_by_id(id);};
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const {return storehouses_.find_by_id(id);};
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const {return storehouses_.cbegin();};
    NodeCollection<Storehouse>::const_iterator storehouse_cend () const {return storehouses_.cend();};
    NodeCollection<Storehouse>::iterator storehouse_begin() {return storehouses_.begin();};
    NodeCollection<Storehouse>::iterator storehouse_end () {return storehouses_.end();};

    /* === Logika biznesowa klasy Factory === */

    bool is_consistent();
    void do_deliveries(Time t);
    void do_package_passing();
    void do_work(Time t);

};

enum class ElementType{
    RAMP,
    WORKER,
    STOREHOUSE,
    LINK
};

struct ParsedLineData{
    ElementType element_type;
    std::map<std::string, std::string> parameters;
};

Factory load_factory_structure(std::istream &is);

void save_factory_structure(Factory& factory, std::ostream& os);

#endif //NETSIM_FACTORY_HPP
