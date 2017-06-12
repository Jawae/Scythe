/**
    layer.hpp
    Deep learning base layer

    @author Antoine Passemiers
    @version 1.0 10/06/2017
*/

#ifndef LAYER_HPP_
#define LAYER_HPP_

#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <memory>
#include <limits>
#include <cassert>
#include <string.h>

#include "../../forest/forest.hpp"

constexpr size_t MAX_N_FORESTS_PER_LAYER = 10000;


struct LayerConfig {
    ForestConfig fconfig;
    size_t       n_forests;
};

/**
    Main goal of layers: ensuring that each forest gets
    a two-dimensional dataset as input, and ensuring that
    the dimensionality of the output is right
    (1d for regression, 2d for classification). These dimensionalities
    must be invariant to the complexity of cascades and convolutional layers.

    Therefore, the shapes of the datasets are "re-mapped" between layers, and
    the definition of how it works must be defined in each layer class.
*/

class Layer {
protected:
    std::string name; // Layer name

    // The product of in_shape elements must be equal to the product of
    // virtual_in_shape elements.
    std::vector<size_t> in_shape;          // Input shape
    std::vector<size_t> virtual_in_shape;  // Virtual input shape
    std::vector<size_t> virtual_out_shape; // Virtual output shape

    std::vector<std::shared_ptr<Layer>> children; // children layers
    std::vector<std::shared_ptr<Forest>> forests; // Intern forests

    std::shared_ptr<VirtualDataset> vdataset; // Virtual dataset
    LayerConfig lconfig; // Layer configuration

public:
    Layer(LayerConfig lconfig);
    ~Layer() = default;
};

#endif // LAYER_HPP_