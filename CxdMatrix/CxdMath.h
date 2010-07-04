/*
 *  CxdMath.h
 *  CxdMatrix
 *
 *  Created by Chris Davey on 11/04/10.
 *  Copyright 2010 none. All rights reserved.
 *
 */

#ifndef CXD_MATH_INCLUDES
#define CXD_MATH_INCLUDES


#ifdef __cplusplus

#include "signals2.hpp"
#include <boost/bind.hpp>
#include "armadillo"


// interfaces
#include "IOperator.hpp"


// IO
#include "IReader.h"
#include "IWriter.h"

#include "MatrixReader.h"



// operations
#include "MatrixOperator.hpp"
#include "NormaliseMatrix.hpp"
#include "MinMaxNormaliseMatrix.hpp"
#include "LogSigOperator.hpp"
#include "RangeSigOperator.hpp"
#include "McCullochPittsNeuron.hpp"
#include "BackPropogration.hpp"
#include "SquaredError.hpp"
#include "MeanSquaredError.hpp"
#include "McCullochPittsNeuralNetwork.hpp"
#endif

#endif