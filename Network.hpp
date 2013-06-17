#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <utility>

#include <iostream>
#include <cassert>

#include "Utils.hpp"
#include "ActivationFunction.hpp"

typedef std::vector< /* Layer */ std::vector< /* Neuron */ std::vector< double /* Weight */ > > > Network;

typedef std::vector< /* Layer */ std::vector< /* Neuron */ std::vector< double /* Weight delta */ > > > WeightDeltas;

typedef std::vector< /* Layer */ std::vector< /* Neuron */ double > > Deltas;

typedef std::vector< /* Layer */ std::vector< /* Neuron */ double > > Biases;

typedef std::vector< /* Layer */ std::vector< /* Neuron */ double > > BiasesDeltas;


template< typename IT, typename OT, typename ActivationFunction = SigmoidFunction >
std::vector< /* Layer */ std::vector< OT /* Neuron */ > >
feed( const Network & network, const Biases & biases, const std::pair< IT, OT > & data_sample )
{
    static ActivationFunction ac;

    //    std::cerr << std::endl; std::cerr << "data_sample:\n" << data_sample << std::endl;

    typedef std::vector< /* Layer */ std::vector< OT /* Neuron */ > >  NetworkResult;

    NetworkResult network_result( network.size() );

    for ( std::size_t i = 0; i < network.size(); ++ i ) {
        // Init layer
        network_result[ i ].resize( network[ i ].size() ); // Init with number of neurons
    }

    // Init input layer
    assert( network_result.front().size() == data_sample.first.size() );
    for ( std::size_t i = 0; i < network_result.front().size(); ++ i ) {
        network_result.front()[ i ] = data_sample.first[ i ];
    }

    for ( std::size_t i = 1; i < network.size(); ++ i ) { // Loop over layers
        for ( std::size_t j = 0; j < network[ i ].size(); ++ j ) { // Loop over neurons
            double sum = 0.0;
            for ( std::size_t k = 0; k < network[ i ][ j ].size(); ++ k ) { // Loop over weights
                sum += network_result[ i - 1 ][ k ] * network[ i ][ j ][ k ];
            }
            // Last layer
            if ( i == network.size() - 1 ) {
                network_result[ i ][ j ] = sum;
            }
            else {
                sum += biases[ i ][ j ];
                network_result[ i ][ j ] = ac( sum );
            }
        }
    }
    //    std::cerr << "network_result:\n" << network_result << std::endl; std::cerr << std::endl;

    return std::move( network_result );
}

template < typename OT, typename ActivationFunctionDerivative = SigmoidFunctionDerivative >
inline
OT
output_gradient( const OT & obtained, const OT & desired )
{
    static ActivationFunctionDerivative acd;
    //    return acd( obtained ) * ( desired - obtained );
    std::cerr << "desired: " << desired << std::endl;
    std::cerr << "obtained: " << obtained << std::endl;
    OT gradient = acd( obtained ) * ( desired - obtained );
    std::cerr << "gradient: " << gradient << std::endl;
    return gradient;
}

template< typename OT, typename ActivationFunctionDerivative = SigmoidFunctionDerivative >
inline
std::vector<OT>
output_gradient( const std::vector<OT> & obtained_vector, const OT & desired )
{
    std::vector<OT> gradient_vector( obtained_vector.size() );
    for ( std::size_t i = 0; i < gradient_vector.size(); ++ i ) {
        gradient_vector[ i ] = output_gradient< OT, ActivationFunctionDerivative >( obtained_vector[ i ], desired );
    }
    return std::move( gradient_vector );
}


template< typename IT, typename OT, typename RT, typename ActivationFunctionDerivative = SigmoidFunctionDerivative >
inline
Deltas calculate_new_deltas( const Network & network,
                             const RT & network_result,
                             const std::pair< IT, OT > & data_sample )
{
    static ActivationFunctionDerivative acd;

    Deltas deltas( network.size() );
    for ( std::size_t i = 0; i < network.size(); ++ i ) {
        deltas[ i ].resize( network[ i ].size() );
    }

    // Calculate error gradient
    {
        const auto error_gradient = output_gradient< OT, ActivationFunctionDerivative >( network_result.back(), data_sample.second );
        assert( deltas.back().size() == error_gradient.size() );
        deltas.back() = error_gradient;
    }

    for ( std::size_t i = network.size() - 2; i > 0; -- i ) {
        for ( std::size_t j = 0; j < network[ i ].size(); ++ j ) {
            double sum = 0.0;
            for ( std::size_t k = 0; k < network[ i + 1 ].size(); ++ k ) {
                sum += deltas[ i + 1 ][ k ] * network[ i + 1 ][ k ][ j ];
            }
            deltas[ i ][ j ] = acd( network_result[ i ][ j ] ) * sum;
        }
    }

    //    std::cerr << "deltas: " << deltas << std::endl;

    return std::move( deltas );
}

template< typename IT,
          typename OT,
          typename RT,
          typename ActivationFunction = SigmoidFunction,
          typename ActivationFunctionDerivative = SigmoidFunctionDerivative >

std::tuple< WeightDeltas, BiasesDeltas >

backpropagation( const Network & network,
                 const WeightDeltas & prev_weight_deltas,
                 const BiasesDeltas & prev_biases_deltas,
                 const RT & network_result,
                 const std::pair< IT, OT > & data_sample,
                 const double momentum,
                 const double learning_rate )
{
    //  Calculate new deltas
    WeightDeltas new_weight_deltas = prev_weight_deltas;
    BiasesDeltas new_biases_deltas = prev_biases_deltas;

    const Deltas new_deltas = calculate_new_deltas< IT, OT, RT, ActivationFunctionDerivative > ( network, network_result, data_sample );

    // Calculate new weigth deltas
    for ( std::size_t i = 0; i < new_weight_deltas.size(); ++ i ) {
        // Layer
        for ( std::size_t j = 0; j < new_weight_deltas[ i ].size(); ++ j ) {
            // Neuron
            for ( std::size_t k = 0; k < new_weight_deltas[ i ][ j ].size(); ++ k ) {
                // Weight
                new_weight_deltas[ i ][ j ][ k ] = learning_rate * network_result[ i ][ j ] * new_deltas[ i ][ j ] +
                        momentum * prev_weight_deltas[ i ][ j ][ k ];
            }

            // Bias
            new_biases_deltas[ i ][ j ] = learning_rate * new_deltas[ i ][ j ] + momentum * prev_biases_deltas[ i ][ j ];
        }
    }

    //    std::cerr << "new_weight_deltas:\n" << new_weight_deltas << std::endl;

    return std::move( std::make_tuple( new_weight_deltas, prev_biases_deltas ) );
}


template < typename IT, typename OT >
std::tuple< Network, WeightDeltas, Biases, BiasesDeltas >
init_network( const std::vector< unsigned int > & layers_description,
              const std::vector< std::pair< IT, OT > > & training_dataset )
{
    Network network;
    WeightDeltas weight_deltas;
    Biases biases;
    BiasesDeltas biases_deltas;
    {
        network.resize( layers_description.size() );
        weight_deltas.resize( layers_description.size() );
        biases.resize( layers_description.size() );
        biases_deltas.resize( layers_description.size() );

        for ( std::size_t i = 0; i < layers_description.size(); ++ i ) {

            network[ i ].resize( layers_description[ i ] );
            weight_deltas[ i ].resize( layers_description[ i ] );
            biases[ i ].resize( layers_description[ i ] );
            biases_deltas[ i ].resize( layers_description[ i ] );

            std::fill( biases[ i ].begin(), biases[ i ].end(), -1.0 );

            for ( std::size_t j = 0; j < network[ i ].size(); ++ j ) {

                // Resize neuron
                if ( i == 0 ) {
                    // Input layer
                    network[ i ][ j ].resize( dimension( training_dataset.at(0).first ) );
                    std::fill( network[ i ][ j ].begin(), network[ i ][ j ].end(), 1.0 );
                }
                else /*( i > 0 )*/ {
                    // Hidden layers
                    network[ i ][ j ].resize( network[ i - 1 ].size() );
                    std::generate( network[ i ][ j ].begin(), network[ i ][ j ].end(), std::bind( rand_range, -0.5, 0.5 ) );
                }

                weight_deltas[ i ][ j ].resize( network[ i ] [ j ].size() );
            }
        }

        //        std::cerr << "network:\n" << network << std::endl;
        //        std::cerr << "weight_deltas:\n" << weight_deltas << std::endl;
        //        std::cerr << "biases:\n" << biases << std::endl;
        //        std::cerr << "biases_deltas:\n" << biases_deltas << std::endl;
    }
    return std::move( std::make_tuple( network, weight_deltas, biases, biases_deltas ) );
}


inline
Network
update_weights( const Network & network, const WeightDeltas & weight_deltas )
{
    //    std::cerr << "network:\n" << network << std::endl;
    //    std::cerr << "weight_deltas:\n" << weight_deltas << std::endl;
    Network new_network = network;
    for ( std::size_t i = 0; i < network.size(); ++ i ) {
        // Layer
        for ( std::size_t j = 0; j < network[ i ].size(); ++ j ) {
            // Neuron
            for ( std::size_t k = 0; k < network[ i ][ j ].size(); ++k ) {
                // weight
                new_network[ i ][ j ][ k ] += weight_deltas[ i ][ j ][ k ];
            }
        }
    }
    //    std::cerr << "new_network:\n" << new_network << std::endl;
    return std::move( new_network );
}


inline
Biases
update_biases( const Biases & biases, const BiasesDeltas & biases_deltas )
{
    Biases new_biases = biases;
    for ( std::size_t i = 0; i < biases.size(); ++ i ) {
        for ( std::size_t j = 0; j < biases[ i ].size(); ++ j ) {
            new_biases[ i ][ j ] += biases_deltas[ i ][ j ];
        }
    }
    return std::move( new_biases );
}


template< typename IT,
          typename OT,
          typename ActivationFunction = SigmoidFunction,
          typename ActivationFunctionDerivative = SigmoidFunctionDerivative >
Network
network( const std::vector< unsigned int > & layers_description,
         const std::vector< std::pair< IT, OT > > & training_dataset,
         const std::vector< std::pair< IT, OT > > & testing_dataset,
         const double desired_accuracy,
         const unsigned int max_epoch,
         const double momentum,
         const double learning_rate)
{

    std::cerr << std::endl;
    std::cerr << "training_dataset: " << training_dataset << std::endl;
    std::cerr << std::endl;
    std::cerr << "testing_dataset: " << testing_dataset << std::endl;
    std::cerr << std::endl;
    std::cerr << "layers_description: " << layers_description << std::endl;

    // Init network
    auto init_network_weight_bias_biases = init_network( layers_description, training_dataset );
    Network network = std::move( std::get<0>(init_network_weight_bias_biases) );
    WeightDeltas weight_deltas = std::move( std::get<1>(init_network_weight_bias_biases) );
    Biases biases = std::move( std::get<2>(init_network_weight_bias_biases) );
    BiasesDeltas biases_deltas = std::move( std::get<3>(init_network_weight_bias_biases) );

    //    std::cerr << "network:\n" << network << std::endl;
    //    std::cerr << "weight_deltas:\n" << weight_deltas << std::endl;
    //    std::cerr << "biases:\n" << biases << std::endl;
    //    std::cerr << "biases_deltas:\n" << biases_deltas << std::endl;


    //        /*
    unsigned int epoch_counter = 0;
    double prev_err = 1.0;

    while ( true ) {
        if ( max_epoch == epoch_counter++ ) break;
        //        std::cerr << "epoch_counter: " << epoch_counter << std::endl;

        //        auto init_network_weight_bias_biases = init_network( layers_description, training_dataset );
        //        weight_deltas = std::move( std::get<1>(init_network_weight_bias_biases) );
        //        biases = std::move( std::get<2>(init_network_weight_bias_biases) );
        //        biases_deltas = std::move( std::get<3>(init_network_weight_bias_biases) );

        double epoch_error = 0.0;

        // Randomly pick a pattern
        //        std::list< unsigned int > picked_list;

        for ( size_t dataIndex = 0; dataIndex < training_dataset.size(); ++ dataIndex )
        {
            //        while( picked_list.size() < training_dataset.size() ) {
            //            bool picked = true;
            //            unsigned int dataIndex;
            //            while ( picked ) {
            //                dataIndex = rand() % training_dataset.size();
            //                if ( picked_list.cend() == std::find( picked_list.cbegin(), picked_list.cend(), dataIndex ) ) {
            //                    picked = false;
            //                    picked_list.push_back( dataIndex );
            //                }
            //            }
            //            std::cerr << "dataIndex: " << dataIndex << std::endl;

            // Feed forward
            const std::pair< IT, OT > & data_sample = training_dataset.at( dataIndex );
            const auto network_result = feed< IT, OT, ActivationFunction >( network, biases, data_sample );

            const double error = std::pow( (network_result.back()[ 0 ] - data_sample.second), 2.0 );
            //            std::cerr << "error: " << error << std::endl;
            epoch_error += error;

            // Back propagation
            auto weight_bias_deltas = backpropagation< IT, OT, typename std::remove_reference< decltype(network_result) >::type, ActivationFunction, ActivationFunctionDerivative >( network, weight_deltas, biases_deltas, network_result, data_sample, momentum, learning_rate );
            weight_deltas = std::move( std::get<0>( weight_bias_deltas ) );
            biases_deltas = std::move( std::get<1>( weight_bias_deltas ) );

            //                        network = update_weights( network, weight_deltas );
            //                        biases = update_biases( biases, biases_deltas );

        }

        epoch_error /= training_dataset.size();


        if ( (epoch_error <= desired_accuracy) || ( epoch_error > prev_err ) ) {
            std::cerr << "Achieved accuracy: " << epoch_error << std::endl;
            break;
        }
        std::cerr << "epoch_error: " << epoch_error << std::endl;
        std::cerr << std::endl;
        prev_err = epoch_error;

        network = update_weights( network, weight_deltas );
        biases = update_biases( biases, biases_deltas );

    }
    std::cerr << std::endl; std::cerr << "epoch_counter: " << epoch_counter << std::endl; std::cerr << std::endl;
    //        */

    // Evaluation
    std::cerr << "Testing evaluation:\n";
    for ( std::size_t i = 0; i < testing_dataset.size(); ++ i ) {
        const std::pair< IT, OT > & data_sample = testing_dataset[ i ];

        const auto result = feed< IT, OT, ActivationFunction >( network, biases, data_sample );
        const OT & obtained = result.back()[ 0 ];

        std::cerr << "desired: " << data_sample.second << std::endl;
        std::cerr << "obtained: " << obtained << std::endl;
        std::cerr << std::endl;
    }

    //    std::cerr << "Training evaluation:\n";
    //    for ( std::size_t i = 0; i < training_dataset.size(); ++ i ) {
    //        const std::pair< IT, OT > & data_sample = training_dataset[ i ];

    //        const auto result = feed( network, biases, data_sample );
    //        const OT & obtained = result.back()[ 0 ];

    //        std::cerr << "desired: " << data_sample.second << std::endl;
    //        std::cerr << "obtained: " << obtained << std::endl;
    //        std::cerr << std::endl;
    //    }


    return network;

}


#endif // NETWORK_HPP
