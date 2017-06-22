/**
    layer.hpp
    Deep learning base layer

    @author Antoine Passemiers
    @version 1.0 10/06/2017
*/

#include "layer.hpp"


LayerConfig::LayerConfig() :
     fconfig(), n_forests(0), forest_type(gbdf::COMPLETE_RANDOM_FOREST) {}

Layer::Layer(LayerConfig lconfig) :
    name(), in_shape(), virtual_in_shape(), virtual_out_shape(), 
    children(), forests(), vdataset(nullptr), vtargets(nullptr), lconfig() {
    this->lconfig = lconfig;
}

Layer::~Layer() {}

void Layer::grow(VirtualDataset* vdataset, VirtualTargets* vtargets) {
    assert(forests.size() == 0);
    Forest* forest;
    for (unsigned int i = 0; i < lconfig.n_forests; i++) {
        if (lconfig.forest_type == gbdf::RANDOM_FOREST) {
            forest = new ClassificationRF(&lconfig.fconfig, vdataset->getNumInstances(), vdataset->getNumFeatures());
        }
        else if (lconfig.forest_type == gbdf::GB_FOREST) {
            // forest = new ClassificationGB(config, dataset->n_rows, dataset->n_cols);
            std::cout << "Error: gradient boosting is not supported" << std::endl;
        }
        else if (lconfig.forest_type == gbdf::COMPLETE_RANDOM_FOREST) {
            forest = new ClassificationCompleteRF(&lconfig.fconfig, vdataset->getNumInstances(), vdataset->getNumFeatures());
        }
        else {
            std::cout << "Error: this type of forest does not exist" << std::endl;
        }
        forest->fit(vdataset, vtargets);
        forests.push_back(std::shared_ptr<Forest>(forest));
    }
}

void Layer::add(layer_p layer) {
    children.push_back(layer);
}

vdataset_p Layer::getVirtualDataset() {
    return this->vdataset;
}

size_t Layer::getNumChildren() {
    return this->children.size();
}

std::ostream& operator<<(std::ostream& os, Layer* const layer) {
    std::cout << "VVVVVV" << std::endl;
    std::cout << layer->getType() << std::endl;
    std::cout << layer->getRequiredMemorySize() << std::endl;
    return os << "Layer type: "   << layer->getType() << std::endl
              << "Virtual size: " << layer->getRequiredMemorySize() << std::endl;
}