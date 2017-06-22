/**
    scanner2D.cpp
    Multi-grained scanning

    @author Antoine Passemiers
    @version 1.0 10/06/2017
*/

#include "scanner2D.hpp"


ScannedDataset2D::ScannedDataset2D(
    data_t* data, size_t N, size_t M, size_t P, size_t kc, size_t kr) : 
    N(N),                // Number of instances
    M(M),                // Instance height
    P(P),                // Instance width
    kc(kc),              // Kernel width
    kr(kr),              // Kernel height
    sc(P - kc + 1),      // Number of kernel positions per column
    sr(M - kr + 1),      // Number of kernel positions per row
    Nprime(N * sr * sc), // Number of instances after scanning
    Mprime(kc * kr),     // Number of features after scanning
    data(data) {}

ScannedDataset2D::ScannedDataset2D(const ScannedDataset2D& other) :
    N(other.N),
    M(other.M),
    P(other.P),
    kc(other.kc),
    kr(other.kr),
    sc(other.sc),
    sr(other.sr),
    Nprime(other.Nprime),
    Mprime(other.Mprime),
    data(other.data) {}

ScannedDataset2D& ScannedDataset2D::operator=(const ScannedDataset2D& other) {
    this->N = other.N;
    this->M = other.M;
    this->P = other.P;
    this->kc = other.kc;
    this->kr = other.kr;
    this->sc = other.sc;
    this->sr = other.sr;
    this->Nprime = other.Nprime;
    this->Mprime = other.Mprime;
    this->data = other.data;
    return *this;
}

data_t ScannedDataset2D::operator()(size_t i, size_t j) {
    return 0; // TODO
}

size_t ScannedDataset2D::getSc() {
    return this->sc;
}

size_t ScannedDataset2D::getSr() {
    return this->sr;
}

size_t ScannedDataset2D::getNumInstances() {
    return this->Nprime;
}

size_t ScannedDataset2D::getNumFeatures() {
    return this->Mprime;
}

size_t ScannedDataset2D::getRequiredMemorySize() {
    return this->Nprime * this->Mprime;
}

ScannedTargets2D::ScannedTargets2D(data_t* data, size_t n_instances, size_t sc, size_t sr) :
    data(data), n_rows(n_instances * sc * sr), s(sc * sr) {}

ScannedTargets2D::ScannedTargets2D(const ScannedTargets2D& other) :
    data(other.data), n_rows(other.n_rows), s(other.s) {}

ScannedTargets2D& ScannedTargets2D::operator=(const ScannedTargets2D& other) {
    this->data = data;
    this->n_rows = n_rows;
    this->s = s;
    return *this;
}

data_t ScannedTargets2D::operator[](const size_t i) {
    return data[i / s];
}

MultiGrainedScanner2D::MultiGrainedScanner2D(
    LayerConfig lconfig, size_t kc, size_t kr) : Layer(lconfig), kc(kc), kr(kr) {}

vdataset_p MultiGrainedScanner2D::virtualize(MDDataset dataset) {
    assert(dataset.n_dims == 3);
    assert(dataset.dims[0] > 0);
    assert(dataset.dims[1] > 0);
    assert(dataset.dims[2] > 0);
    Layer::vdataset = std::shared_ptr<ScannedDataset2D>(
        new ScannedDataset2D(
            dataset.data,
            dataset.dims[0],
            dataset.dims[1],
            dataset.dims[2],
            this->kc,
            this->kr));
    return Layer::vdataset;
}

vtargets_p MultiGrainedScanner2D::virtualizeTargets(Labels<target_t>* targets) {
    ScannedDataset2D* vdataset = dynamic_cast<ScannedDataset2D*>((this->vdataset).get());
    size_t sc = vdataset->getSc();
    size_t sr = vdataset->getSr();
    size_t n_rows = vdataset->getNumInstances();
    assert(sc > 0);
    assert(sr > 0);
    return std::shared_ptr<ScannedTargets2D>(
        new ScannedTargets2D(targets->data, n_rows, sc, sr));
}

size_t MultiGrainedScanner2D::getRequiredMemorySize() {
    size_t memory_size = this->vdataset.get()->getNumInstances();
    assert(memory_size > 0);
    if (lconfig.fconfig.task == gbdf::CLASSIFICATION_TASK) {
        memory_size *= lconfig.fconfig.n_classes;
    }
    return memory_size;
}