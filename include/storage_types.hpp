#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include <list>
#include "types.hpp"
#include "package.hpp"

enum PackageQueueType { //typ wyliczeniowy
    LIFO,
    FIFO
};

class IPackageStockpile {
public:
    using const_iterator = std::list<Package>::const_iterator; //publiczny alias

    virtual void push(Package&&) = 0; //wrzucenie elementu
    virtual bool empty() = 0; //true gdy puste
    virtual size_type size() = 0; // rozmiar

    virtual const_iterator begin() = 0; //iteratory
    virtual const_iterator end() = 0;
    virtual const_iterator cbegin() = 0;
    virtual const_iterator cend() = 0;

    virtual ~IPackageStockpile() = default; //domyślny wirtualny destruktor
};

class IPackageQueue: public IPackageStockpile {
public:
    virtual Package pop() = 0; //wyciąganie elementu
    virtual PackageQueueType get_queue_type() = 0; //typ kolejki
    ~IPackageQueue() = default; //domyślny wirtualny destruktor
};

class PackageQueue: public IPackageQueue {
private:
    std::list<Package> queue_; //kolejka
    PackageQueueType queue_type_; //typ kolejki
public:
    PackageQueue(PackageQueueType t) : queue_type_(t) {};
    bool empty() override;
    size_type size() override { return queue_.size(); };
    PackageQueueType get_queue_type() override { return queue_type_; };
    void push(Package&& p) override { queue_.push_back(std::move(p)); };
    Package pop() override;

    const_iterator begin() override { return queue_.begin(); };
    const_iterator end() override { return queue_.end(); };
    const_iterator cbegin() override { return queue_.cbegin(); };
    const_iterator cend() override { return queue_.cend(); };


};

#endif //NETSIM_STORAGE_TYPES_HPP
