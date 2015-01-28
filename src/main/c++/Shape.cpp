#include "Shape.h"

boost::unordered_map<std::string, int> Shape::xmap = boost::assign::map_list_of("flat", FLAT)
    ("skillion", SKILLION)
    ("gabled", GABLED)
    ("half-hipped", HALF_HIPPED)
    ("pyramidal", PYRAMIDAL)
    ("gambrel", GAMBREL)
    ("mansard", MANSARD)
    ("dome", DOME)
    ("onion", ONION)
    ("round", ROUND)
    ("saltbox", SALTBOX);
