#pragma once
#include <vector>
#include <iostream>
#include <memory>

using namespace std;

struct GCObject {
    bool marked = false;
    virtual ~GCObject() = default;
};

class GarbageCollector {
    private:
        vector<GCObject*> heap;

    public:
        size_t gc_threshold = 1000;
        template<typename T, typename... Args>
        T* allocate (Args&&... args) {
            T* obj = new T(forward<Args>(args)...);
            heap.push_back(obj);
            return obj;
        }

        void sweep();
        size_t size() const { return heap.size(); }
};