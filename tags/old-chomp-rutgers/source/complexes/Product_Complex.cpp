/* Product_Complex.cpp
 * CHomP project
 * Shaun Harker 3/31/10
 */

#include "complexes/Product_Complex.h"

#ifndef CHOMP_HEADER_ONLY_
/* Template Instances */
#include "complexes/Cubical_Complex.h" 
template class Product_Complex < Cubical_Complex, Cubical_Complex >;
#endif
