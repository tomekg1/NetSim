#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include <vector>
#include <numeric>
#include <ostream>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include "helpers.hpp"
#include "types.hpp"
#include "package.hpp"
#include "storage_types.hpp"


enum ReceiverType { //typ wyliczeniowy
    WORKER,
    STOREHOUSE
};



class IPackageReceiver{
public:
    using const_iterator = IPackageStockpile::const_iterator; //publiczny alias

    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;

    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

    virtual ReceiverType get_receiver_type() const = 0;

    virtual ~IPackageReceiver() = default; //domyślny wirtualny destruktor
};



class ReceiverPreferences {

    using preferences_t = std::map<IPackageReceiver*, double>; // wartość - prawdopodobieństwo wyboru odbiorcy
    using const_iterator = preferences_t::const_iterator;
    using iterator = preferences_t::iterator;
private:

    ProbabilityGenerator my_pg;
    preferences_t my_pref;

public:

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : my_pg(pg){}; // uchwyt do funckji zwracającej losową wartość
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver(); // zwraca uchwyt do wybranego odbiorcy
    const preferences_t& get_preferences() const { return my_pref; }

    iterator begin() { return my_pref.begin(); }
    iterator end() { return my_pref.end(); }
    const_iterator cbegin() const { return my_pref.cbegin(); }
    const_iterator cend() const { return my_pref.cend(); }

};



class PackageSender {
public:
    ReceiverPreferences receiver_preferences_;
    std::optional<Package> send_buffer; // pole realizujące funkcjonalność bufora półproduktu gotowego do wysłania

    PackageSender(): send_buffer(std::nullopt) {};
    PackageSender(PackageSender&& other) = default;
    void send_package();
    const std::optional<Package>& get_sending_buffer() const { return send_buffer; };


protected:
    void push_package(Package&& package) { send_buffer.emplace(std::move(package)); };
};



class Storehouse: public IPackageReceiver{
private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
public:
    Storehouse(ElementID id, std::unique_ptr<PackageQueue> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)) : id_(id), d_(std::move(d)) {};

    void receive_package(Package&& p) override {d_->push(std::move(p));};
    ElementID get_id() const override {return id_;};

    const_iterator cbegin() const override {return d_->cbegin();};
    const_iterator cend() const override {return d_->cend();};

    ReceiverType get_receiver_type() const override {return ReceiverType::STOREHOUSE;};
};



class Ramp : public PackageSender {
private:
    ElementID my_ID_;

    /**
     * Okres czasu pomiędzy generowaniem paczek.
     */
    TimeOffset delivery_interval_;

public:
    /**
     * Konstruktor rampy.
     * @param id - ID rampy
     * @param di - okres czasu pomiędzy generowaniem paczek
     */
    Ramp(ElementID id, TimeOffset di) : my_ID_(id), delivery_interval_(di) { };

    /**
     * Funkcja pętli symulacji - obsługuje ładowanie paczki do bufora klasy nadrzędnej.
     * @param t - czas symulacji
     */
    void deliver_goods(Time t);

    /**
     * Zwraca okres czasu pomiędzy generowaniem paczek.
     * @return okres czasu pomiędzy generowaniem paczek.
     */
    TimeOffset get_delivery_interval() const { return delivery_interval_; };

    /**
     * Zwraca nadane wcześniej ID rampy.
     * @return ID rampy.
     */
    ElementID get_id() const { return my_ID_; };
};



class Worker : public PackageSender, public IPackageReceiver {
private:
    ElementID my_ID_;

    /**
     * Czas przetwarzania paczki.
     */
    TimeOffset processing_duration_;

    /**
     * Aktualnie przetwarzana przez pracownika paczka (jeśli obecna)
     */
    std::optional<Package> processed_package_ = std::nullopt;

    /**
     * Informacja, kiedy została rozpoczęta praca nad aktualną paczką.
     */
    Time package_processing_start_time_ = 0;

    /**
     * Wskaźnik do interfejsu kolejki przechowującej produkty.
     */
    std::unique_ptr<IPackageQueue> package_queue_;

public:
    /**
     * Konstruktor klasy pracownika.
     * @param id - ID pracownika
     * @param pd - czas przetwarzania pojedynczej paczki
     * @param q - uchwyt do kolejki IPackageQueue
     */
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) :
        my_ID_(id), processing_duration_(pd), package_queue_(std::move(q)) {};

    /**
     * Funkcja pętli symulacji - obsługuje przetwarzanie paczki przez pracownika.
     * @param t - czas symulacji
     */
    void do_work(Time t);

    /**
     * Zwraca czas przetwarzania pojedynczej paczki.
     * @return czas przetwarzania pojedynczej paczki
     */
    TimeOffset get_processing_duration() const { return processing_duration_; };

    /**
     * Zwraca czas, w którym rozpoczęta została praca nad aktualną paczką.
     * @return czas, w którym rozpoczęta została praca nad aktualną paczką
     */
    Time get_package_processing_start_time() const { return package_processing_start_time_; };

    /**
     * Informuje, czy pracownik przetwarza teraz paczkę.
     * @return True, jeśli przetwarza paczkę; False, jeśli nie przetwarza.
     */
    bool is_processing_package() const { return processed_package_.has_value(); };

    const std::optional<Package>* get_processing_buffer() const { return &processed_package_; };

    IPackageQueue* get_queue() const { return package_queue_.get(); };

    /* === IPackageReceiver === */

    void receive_package(Package&& p) override { package_queue_->push(std::move(p)); };
    ElementID get_id() const override { return my_ID_; };

    const_iterator cbegin() const override { return package_queue_->cbegin(); };
    const_iterator cend() const override { return package_queue_->cend(); };

     ReceiverType get_receiver_type() const override { return ReceiverType::WORKER; };
};

#endif //NETSIM_NODES_HPP
