#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <vector>
#include <algorithm>
#include <functional>
#include <utility>

#include <iostream>
#include <cassert>

#include "Utils.hpp"

typedef std::vector< /* Layer */ std::vector< /* Neuron */ std::vector< double /* Weight */ > > > Network;

typedef std::vector< /* Layer */ std::vector< /* Neuron */ std::vector< double /* Weight delta */ > > > WeightDeltas;

typedef std::vector< /* Layer */ std::vector< /* Neuron */ double > > Deltas;

struct SigmoidFunction {
    inline double operator() ( const double t ) const {
        return ( 1.0/( 1.0 + exp(-t) ) );
    }
};


//template< typename IT, typename OT, typename ActivationFunction >
//std::vector< /* Layer */ std::vector< IT /* Neuron */ > >
//feed( const Network & network, const std::pair< IT, OT > & data_sample );


template< typename IT, typename OT, typename ActivationFunction = SigmoidFunction >
std::vector< /* Layer */ std::vector< IT /* Neuron */ > >
feed( const Network & network, const std::pair< IT, OT > & data_sample ) {
    static ActivationFunction ac;

    //    std::cerr << std::endl;
    //    std::cerr << "data_sample: " << data_sample << std::endl;

    typedef std::vector< /* Layer */ std::vector< IT /* Neuron */ > >  NetworkResult;

    NetworkResult network_result( network.size() );

    for ( std::size_t i = 0; i < network.size(); ++ i ) {
        // Init layer
        network_result[ i ].resize( network[ i ].size() ); // Init with number of neurons
    }

    // Init input layer
    for ( std::size_t i = 0; i < network_result.front().size(); ++ i ) {
        assert( dimension( network_result.front()[ i ] ) == dimension( std::get<0>( data_sample ) ) );
        network_result.front()[ i ] = data_sample.first;
    }

    for ( std::size_t i = 1; i < network.size(); ++ i ) { // Loop over layers
        for ( std::size_t j = 0; j < network[ i ].size(); ++ j ) { // Loop over neurons
            double sum = 0.0;
            for ( std::size_t k = 0; k < network[ i ][ j ].size(); ++ k ) { // Loop over weights
                sum += network_result[ i - 1 ][ k ] * network[ i ][ j ][ k ];
            }
            network_result[ i ][ j ] = ac( sum );
        }
    }

    //    std::cerr << "network_result.back(): " << network_result.back() << std::endl;

    return std::move( network_result );
}

template < typename OT >
OT output_gradient( const OT & obtained, const OT & desired )
{
    //    std::cerr << "obtained: " << obtained << std::endl;
    //    std::cerr << "desired: " << desired << std::endl;
    OT gradient;
    gradient = obtained * ( 1.0 - obtained ) * ( desired - obtained );
    //    std::cerr << "gradient: " << gradient << std::endl;
    return gradient;
}

template< typename OT >
std::vector<OT> output_gradient( const std::vector<OT> & obtained_vector, const OT & desired )
{
    std::vector<OT> gradient_vector( obtained_vector.size() );
    for ( std::size_t i = 0; i < gradient_vector.size(); ++ i ) {
        gradient_vector[ i ] = output_gradient( obtained_vector[ i ], desired );
    }
    return gradient_vector;
}


template< typename IT, typename OT, typename RT >
inline
Deltas calculate_new_deltas( const Network & network,
                             const RT & network_result,
                             const std::pair< IT, OT > & data_sample )
{
    Deltas new_deltas( network.size() );
    for ( std::size_t i = 0; i < network.size(); ++ i ) {
        new_deltas[ i ].resize( network[ i ].size() );
    }

    // Calculate error gradient
    {
        const auto error_gradient = output_gradient( network_result.back(), data_sample.second );
        assert( new_deltas.back().size() == error_gradient.size() );
        new_deltas.back() = error_gradient;
    }

    for ( std::size_t i = new_deltas.size() - 2; i > 0; -- i ) {
        std::cerr << std::endl;
        for ( std::size_t j = 0; j < network[ i ].size(); ++ j ) {
            double sum = 0.0;
            for ( std::size_t k = 0; k < network[ i + 1 ].size(); ++ k ) {
                sum += new_deltas[ i + 1 ][ k ] * network[ i + 1 ][ k ][ j ];
            }
            new_deltas[ i ][ j ] = network_result[ i ][ j ] * ( 1.0 - network_result[ i ][ j ] ) * sum;
        }
    }

    return std::move( new_deltas );
}

template< typename IT, typename OT, typename RT >
inline
WeightDeltas
backpropagation( const Network & network,
                 const WeightDeltas & prev_weight_deltas,
                 const RT & network_result,
                 const std::pair< IT, OT > & data_sample,
                 const double momentum,
                 const double learning_rate )
{
    //  Calculate new deltas
    WeightDeltas new_weight_deltas = prev_weight_deltas;

    const Deltas new_deltas = calculate_new_deltas( network, network_result, data_sample );

    // Calculate new weigth deltas
    for ( std::size_t i = 0; i < new_weight_deltas.size(); ++ i ) {
        // Layer
        for ( std::size_t j = 0; j < new_weight_deltas[ i ].size(); ++ j ) {
            // Neuron
            for ( std::size_t k = 0; k < new_weight_deltas[ i ][ j ].size(); ++ k ) {
                // Weight
                new_weight_deltas[ i ][ j ][ k ] = learning_rate * network_result[ i ][ j ] * new_deltas[ i ][ j ] + momentum * new_weight_deltas[ i ][ j ][ k ];
            }
        }
    }

    //    std::cerr << "new_weight_deltas:\n" << new_weight_deltas << std::endl;

    return std::move( new_weight_deltas );
}


template < typename IT, typename OT >
std::pair< Network, WeightDeltas >
init_network( const std::vector< unsigned int > & layers_description,
              const std::vector< std::pair< IT, OT > > & training_dataset )
{
    Network network;
    WeightDeltas weight_deltas;
    {
        weight_deltas.resize( layers_description.size() );
        network.resize( layers_description.size() );
        for ( std::size_t i = 0; i < layers_description.size(); ++ i ) {

            weight_deltas[ i ].resize( layers_description[ i ] );

            // Resize layer
            network[ i ].resize( layers_description[ i ] );

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
        std::cerr << "network:\n" << network << std::endl;
        std::cerr << "weight_deltas:\n" << weight_deltas << std::endl;
    }
    return std::move( std::make_pair( network, weight_deltas ) );
}



void update_weights( Network & network, const WeightDeltas & weight_deltas )
{
    //            std::cerr << "network:\n" << network << std::endl;
    //            std::cerr << std::endl;
    for ( std::size_t i = 0; i < network.size(); ++ i ) {
        // Layer
        for ( std::size_t j = 0; j < network[ i ].size(); ++ j ) {
            // Neuron
            for ( std::size_t k = 0; k < network[ i ][ j ].size(); ++k ) {
                // weight
                //                            std::cerr << "network[ " << i << " ][ " << j << " ][ " <<  k  << " ]: "
                //                                      << network[ i ][ j ][ k ] << std::endl;
                //                            std::cerr << "weight_deltas[ " << i << " ][ " << j << " ][ " <<  k  << " ]: "
                //                                      << weight_deltas[ i ][ j ][ k ] << std::endl;
                network[ i ][ j ][ k ] += weight_deltas[ i ][ j ][ k ];
            }
            //                        std::cerr << std::endl;
        }
        //                    std::cerr << std::endl;
    }
    //            std::cerr << "network:\n" << network << std::endl;
}


template< typename IT, typename OT >
Network
network( const std::vector< unsigned int > & layers_description,
         const std::vector< std::pair< IT, OT > > & training_dataset,
         const std::vector< std::pair< IT, OT > > & testing_dataset,
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
    auto init_network_pair = init_network( layers_description, training_dataset );
    Network network = std::move( init_network_pair.first );
    WeightDeltas weight_deltas = std::move( init_network_pair.second );

    //    /*
    unsigned int epoch_counter = 0;

    while ( true ) {
        if ( max_epoch == epoch_counter++ ) break;
        std::cerr << "epoch_counter: " << epoch_counter << std::endl;

        double epoch_error = 0.0;

        for ( size_t dataIndex = 0; dataIndex < training_dataset.size(); ++ dataIndex )
        {
            // Feed forward
            const std::pair< IT, OT > & data_sample = training_dataset.at( dataIndex );
            const auto network_result = feed( network, data_sample );

            const double error = std::pow( (network_result.back()[ 0 ] - data_sample.second), 2.0 );
            std::cerr << "error: " << error << std::endl;
            epoch_error += error;

            // Back propagation
            weight_deltas =  backpropagation( network, weight_deltas, network_result, data_sample, momentum, learning_rate );
        }

        epoch_error /= training_dataset.size();

        std::cerr << "epoch_error: " << epoch_error << std::endl;
        update_weights( network, weight_deltas );

    }
    //    */


    return network;

}


#endif // NETWORK_HPP
