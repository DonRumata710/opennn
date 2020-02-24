//   OpenNN: Open Neural Networks Library
//   www.opennn.net
//
//   N O R M A L I Z E D   S Q U A R E D   E R R O R   C L A S S   H E A D E R
//
//   Artificial Intelligence Techniques SL
//   artelnics@artelnics.com

#ifndef NORMALIZEDSQUAREDERROR_H
#define NORMALIZEDSQUAREDERROR_H

// System includes

#include <cmath>
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <sstream>

// OpenNN includes

#include "config.h"
#include "loss_index.h"
#include "data_set.h"
#include "tinyxml2.h"

namespace OpenNN
{

/// This class represents the normalized squared error term. 

///
/// This error term is used in data modeling problems.
/// If it has a value of unity then the neural network is predicting the data "in the mean",
/// A value of zero means perfect prediction of data.

class NormalizedSquaredError : public LossIndex
{

public:

    // Constructors

   explicit NormalizedSquaredError(NeuralNetwork*, DataSet*);

   explicit NormalizedSquaredError(NeuralNetwork*);

   explicit NormalizedSquaredError(DataSet*);

   explicit NormalizedSquaredError();   

   explicit NormalizedSquaredError(const tinyxml2::XMLDocument&);

    // Destructor

   virtual ~NormalizedSquaredError();

   // Get methods

    type get_normalization_coefficient() const;
    type get_selection_normalization_coefficient() const;

   // Set methods

    void set_normalization_coefficient();
    void set_normalization_coefficient(const type&);

    void set_selection_normalization_coefficient();
    void set_selection_normalization_coefficient(const type&);

    void set_default();

   // Normalization coefficients 

   type calculate_normalization_coefficient(const Tensor<type, 2>&, const Tensor<type, 1>&) const;

   // Error methods
     
   type calculate_error(const DataSet::Batch& batch, const NeuralNetwork::ForwardPropagation& forward_propagation) const
   {
       Tensor<type, 0> sum_squared_error;

       const Index trainable_layers_number = neural_network_pointer->get_trainable_layers_number();

       switch(device_pointer->get_type())
       {
            case Device::EigenDefault:
            {
                DefaultDevice* default_device = device_pointer->get_eigen_default_device();

                sum_squared_error.device(*default_device)
                        = (forward_propagation.layers[trainable_layers_number-1].activations_2d
                        - batch.targets_2d).square().sum();

                break;
            }

            case Device::EigenSimpleThreadPool:
            {
               ThreadPoolDevice* thread_pool_device = device_pointer->get_eigen_thread_pool_device();

               sum_squared_error.device(*thread_pool_device) = (forward_propagation.layers[trainable_layers_number-1].activations_2d
                                                                - batch.targets_2d).square().sum();

                break;
            }

           case Device::EigenGpu:
           {
//                GpuDevice* gpu_device = device_pointer->get_eigen_gpu_device();

                break;
           }
       }

       return sum_squared_error(0)/normalization_coefficient;
   }

   void calculate_error(BackPropagation& back_propagation) const
   {
       Tensor<type, 0> sum_squared_error;

       switch(device_pointer->get_type())
       {
            case Device::EigenDefault:
            {
                DefaultDevice* default_device = device_pointer->get_eigen_default_device();

                sum_squared_error.device(*default_device) = back_propagation.errors.square().sum();

                break;
            }

            case Device::EigenSimpleThreadPool:
            {
               ThreadPoolDevice* thread_pool_device = device_pointer->get_eigen_thread_pool_device();

               sum_squared_error.device(*thread_pool_device) = back_propagation.errors.square().sum();

                break;
            }

           case Device::EigenGpu:
           {
//                GpuDevice* gpu_device = device_pointer->get_eigen_gpu_device();

                break;
           }
       }

       back_propagation.loss = sum_squared_error(0)/normalization_coefficient;
   }


   // Gradient methods

   void calculate_output_gradient(const DataSet::Batch& batch,
                                  const NeuralNetwork::ForwardPropagation&,
                                  BackPropagation& back_propagation) const
   {
        #ifdef __OPENNN_DEBUG__

        check();

        #endif

        const type coefficient = static_cast<type>(2.0)/normalization_coefficient;

        switch(device_pointer->get_type())
        {
             case Device::EigenDefault:
             {
                 DefaultDevice* default_device = device_pointer->get_eigen_default_device();

                 back_propagation.output_gradient.device(*default_device) = coefficient*back_propagation.errors;

                 return;
             }

             case Device::EigenSimpleThreadPool:
             {
                ThreadPoolDevice* thread_pool_device = device_pointer->get_eigen_thread_pool_device();

                back_propagation.output_gradient.device(*thread_pool_device) = coefficient*back_propagation.errors;

                return;
             }

            case Device::EigenGpu:
            {
        //                 GpuDevice* gpu_device = device_pointer->get_eigen_gpu_device();

                 break;
            }
        }
   }

   // Error terms methods

   Tensor<type, 1> calculate_training_error_terms(const Tensor<type, 2>&, const Tensor<type, 2>&) const;
   Tensor<type, 1> calculate_training_error_terms(const Tensor<type, 1>&) const;

   // Squared errors methods

   LossIndex::SecondOrderLoss calculate_terms_second_order_loss() const;

   string get_error_type() const;
   string get_error_type_text() const;

   // Serialization methods

   tinyxml2::XMLDocument* to_XML() const;   
   void from_XML(const tinyxml2::XMLDocument&);

   void write_XML(tinyxml2::XMLPrinter&) const;

private:

   /// Coefficient of normalization for the calculation of the training error.

   type normalization_coefficient;

   type selection_normalization_coefficient;
};

}

#endif


// OpenNN: Open Neural Networks Library.
// Copyright(C) 2005-2020 Artificial Intelligence Techniques, SL.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

