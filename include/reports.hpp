#ifndef NETSIM_REPORTS_HPP
#define NETSIM_REPORTS_HPP

#include "factory.hpp"
#include "types.hpp"
#include <ostream>
#include <string>
#include <map>
#include <set>

std::map<PackageQueueType, std::string> extern queue_type_to_string;
std::map<ReceiverType, std::string> extern receiver_type_to_string;

void extern inprint_receivers(const ReceiverPreferences& prefs, std::ostream& os);

void extern generate_structure_report(const Factory& f, std::ostream& os);

void extern generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);

//class SpecificTurnsReportNotifier{
//public:
//    std::set<Time> my_turns;
//
//    SpecificTurnsReportNotifier(std::set<Time> turns){ my_turns = turns; }
//    bool should_generate_report(Time t){
//        for(auto iter = my_turns.cbegin(); iter != my_turns.cend(); iter++){
//            if(*iter == t) {
//                return true;
//            }
//        }
//        return false;
//    }
//
//};
//
//class IntervalReportNotifier{
//public:
//    TimeOffset my_to;
//
//    IntervalReportNotifier(TimeOffset to) { my_to = to; }
//
//    bool should_generate_report(Time t){
//        return (fmodf(static_cast<float>(t), static_cast<float>(my_to)) == 1);
//    }
//
//};


#endif //NETSIM_REPORTS_HPP
