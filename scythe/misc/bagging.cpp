/**
    bagging.cpp
    Bootstrap aggregation
    
    @author Antoine Passemiers
    @version 1.0 09/06/2017
*/

#include "bagging.hpp"


namespace scythe {

size_t randomInstance(size_t n_instances) {
    return rand() % n_instances;
}

std::shared_ptr<size_t> createSubsetWithReplacement(size_t n_instances, size_t m) {
    /**
        Warning : one instance cannot be used twice inside a same tree
    */
    assert(m <= n_instances);
    size_t* subset = static_cast<size_t*>(malloc(n_instances * sizeof(size_t)));
    for (unsigned int i = 0; i < n_instances; i++) { subset[i] = OUT_OF_BAG; }
    for (unsigned int i = 0; i < m; i++) {
        size_t instance_id = randomInstance(n_instances);
        subset[instance_id] = USED_IN_BAG;
    }
    return std::shared_ptr<size_t>(subset);
}

}