/*
 *  Morse_Complex.cpp
 *
 *  Contains template instantiations.
 *
 *  Created by Shaun Harker on 10/21/09.
 *
 */

#include "complexes/Morse_Complex.h"


#ifndef CHOMP_HEADER_ONLY_
/* Template instantiation */
#include "complexes/Cubical_Complex.h"
template class Morse_Complex<Cubical_Complex>;
template class Morse_Value_Cell<Morse_Complex<Cubical_Complex> >;
template std::ostream & operator << < Morse_Complex < Cubical_Complex > >  (std::ostream &, const Morse_Value_Cell < Morse_Complex < Cubical_Complex > > & );

#include "complexes/Abstract_Complex.h"
template class Morse_Complex<Abstract_Complex <> >;
template class Morse_Value_Cell<Morse_Complex<Abstract_Complex <> > >;
template std::ostream & operator << < Morse_Complex < Abstract_Complex <> > >  (std::ostream &, const Morse_Value_Cell < Morse_Complex < Abstract_Complex <> > > & );
#endif
