#include "gc.h"

using namespace std;

void GarbageCollector::sweep() {
    vector<GCObject*> surviving_objects;
    int deleted_count = 0;

    for(GCObject* obj : heap) {
        if (obj->marked) {
            obj->marked = false;
            surviving_objects.push_back(obj);
        }
        else {
            delete obj;
            deleted_count++;
        }
    }
    
    heap = surviving_objects;
    
    cout << "\n[GC] Swept " << deleted_count << " objects. Heap size: " << heap.size() << endl;
}
