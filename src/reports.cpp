#include "reports.hpp"

std::map<PackageQueueType, std::string> queue_type_to_string = {
        {LIFO, "LIFO"},
        {FIFO, "FIFO"}
};

std::map<ReceiverType, std::string> receiver_type_to_string = {
        {WORKER, "worker"},
        {STOREHOUSE, "storehouse"}
};

void extern inprint_receivers(const ReceiverPreferences &prefs, std::ostream &os) {
    // sortowanie odbiorców
    std::vector<const IPackageReceiver*> receivers = {};
    for (auto r = prefs.cbegin(); r != prefs.cend(); r++){
        receivers.insert(receivers.end(), r->first);
    }
    std::sort(receivers.begin(), receivers.end(), [](const IPackageReceiver* a, const IPackageReceiver* b) {
        if (a->get_receiver_type() == b->get_receiver_type())
            return a->get_id() < b->get_id();
        // WORKER występuje po STOREHOUSE (bo litera S < W), więcej typów nie powinno się tu znaleźć!
        return a->get_receiver_type() != WORKER;
    });

    // generowanie tekstu
    for (auto r : receivers){
        os << "\n    " << receiver_type_to_string.find(r->get_receiver_type())->second;
        os << " #" << r->get_id();
    }
}

void extern generate_structure_report(const Factory &f, std::ostream &os) {
    // Znaki nowej linii pojawiają się przede wszystkim PRZED tekstem.


    /* == LOADING RAMPS == */

    os << "\n== LOADING RAMPS ==\n";

    // sortowanie elementów
    std::vector<const Ramp*> ramps = {};
    for (auto v = f.ramp_cbegin(); v != f.ramp_cend(); v++){
        ramps.insert(ramps.end(), &(*v));
    }
    std::sort(ramps.begin(), ramps.end(), [](const Ramp* a, const Ramp* b) {return a->get_id() < b->get_id();});

    // generowanie tekstu raportu
    for (auto v : ramps){
        os << "\nLOADING RAMP #" << v->get_id();
        os << "\n  Delivery interval: " << v->get_delivery_interval();
        os << "\n  Receivers:";
        inprint_receivers(v->receiver_preferences_, os);
        os << "\n";
    }
    os << "\n";


    /* == WORKERS == */

    os << "\n== WORKERS ==\n";

    // sortowanie elementów
    std::vector<const Worker*> workers = {};
    for (auto v = f.worker_cbegin(); v != f.worker_cend(); v++){
        workers.insert(workers.end(), &(*v));
    }
    std::sort(workers.begin(), workers.end(), [](const Worker* a, const Worker* b) {return a->get_id() < b->get_id();});

    // generowanie tekstu raportu
    for (auto v : workers){
        os << "\nWORKER #" << v->get_id();
        os << "\n  Processing time: " << v->get_processing_duration();
        os << "\n  Queue type: " << queue_type_to_string.find(v->get_queue()->get_queue_type())->second;
        os << "\n  Receivers:";
        inprint_receivers(v->receiver_preferences_, os);
        os << "\n";
    }
    os << "\n";


    /* == STOREHOUSES == */

    os << "\n== STOREHOUSES ==\n";

    // sortowanie elementów
    std::vector<const Storehouse*> storehouses = {};
    for (auto v = f.storehouse_cbegin(); v != f.storehouse_cend(); v++){
        storehouses.insert(storehouses.end(), &(*v));
    }
    std::sort(storehouses.begin(), storehouses.end(), [](const Storehouse* a, const Storehouse* b) {return a->get_id() < b->get_id();});

    // generowanie tekstu raportu
    for (auto v : storehouses){
        os << "\nSTOREHOUSE #" << v->get_id();
        os << "\n";
    }
    os << "\n";
}

void extern generate_simulation_turn_report(const Factory &f, std::ostream &os, Time t) {

    os << "=== [ Turn: " << t << " ] ===\n";


    /* == WORKERS == */

    os << "\n== WORKERS ==\n";

    // sortowanie elementów
    std::vector<const Worker*> workers = {};
    for (auto v = f.worker_cbegin(); v != f.worker_cend(); v++){
        workers.insert(workers.end(), &(*v));
    }
    std::sort(workers.begin(), workers.end(), [](const Worker* a, const Worker* b) {return a->get_id() < b->get_id();});

    // generowanie tekstu raportu
    for (auto v : workers){
        os << "\nWORKER #" << v->get_id();

        os << "\n  PBuffer: ";
        if (v->get_processing_buffer()->has_value())
            os << "#" << v->get_processing_buffer()->value().get_id()
                << " (pt = " << t - v->get_package_processing_start_time() << ")";
        else os << "(empty)";

        os << "\n  Queue: ";
        auto el = v->cbegin();
        if (el != v->cend()) {
            os << "#" << el->get_id();
            el++;
            while (el != v->cend()) {
                os << ", #" << el->get_id();
                el++;
            }
        }
        else os << "(empty)";

        os << "\n  SBuffer: ";
        if (v->get_sending_buffer().has_value())
            os << "#" << v->get_sending_buffer()->get_id();
        else os << "(empty)";

        os << "\n";
    }
    os << "\n";


    os << "\n== STOREHOUSES ==\n";

    // sortowanie elementów
    std::vector<const Storehouse*> storehouses = {};
    for (auto v = f.storehouse_cbegin(); v != f.storehouse_cend(); v++){
        storehouses.insert(storehouses.end(), &(*v));
    }
    std::sort(storehouses.begin(), storehouses.end(),
              [](const Storehouse* a, const Storehouse* b) { return a->get_id() < b->get_id(); });

    // generowanie tekstu raportu
    for (auto v : storehouses){
        os << "\nSTOREHOUSE #" << v->get_id();

        os << "\n  Stock: ";
        auto el = v->cbegin();
        if (el != v->cend()) {
            os << "#" << el->get_id();
            el++;
            while (el != v->cend()) {
                os << ", #" << el->get_id();
                el++;
            }
        }
        else os << "(empty)";

        os << "\n";
    }
    os << "\n";
}
