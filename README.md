# Scythe

[Under development] Machine learning library for fitting gradient boosted trees and 
deep forests.

## Features

|         | C (fit) | C (predict) | R (fit) | R (predict) |
|---------|:-------:|:-----------:|:-------:|------------:|
| RF      | done    | done        |         |             |
| CRF     |         |             |         |             |
| GB      | ?       |             |         |             |

## Todo

### General

* Create a proper configuration file

### Forests

* [CART] Set the minimum number of instances per leaf as a parameter
* Being able to pass non-contiguous Numpy arrays to the C API (not sure if necessary)
* [GB] Create a cross-validation function
* [GB] Create a predict function
* [GB] Use Friedman's formula to compute the gradient
* Adapt the code of the ClassificationGB to create a RegressionGB class

### Deep forests

(Design phase)

## License

Copyright © 2017 Scythe

Distributed under the Eclipse Public License either version 1.0.