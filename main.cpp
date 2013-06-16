#include <iostream>

#include <vector>
#include <functional>
#include <algorithm>
#include <utility>

#include <tuple>
#include <cassert>

#include "Utils.hpp"
#include "Network.hpp"


int main() {

    constexpr unsigned int number_of_points = 20;
    constexpr double a = 0.0;
    constexpr double b = 1.0;
    constexpr double step = ( b - a ) / (number_of_points - 1);

    std::vector< double > x( number_of_points );
    for ( unsigned int i = 0; i < number_of_points; ++ i ) {
        x[ i ] = step * i;
    }

    //    std::cerr << "x: " << x << std::endl;

    std::vector< double > sinx( x.size() );
    for ( unsigned int i = 0; i < number_of_points; ++ i ) {
        sinx[ i ] = std::sin( M_PI * x[ i ] );
    }

    //    std::cerr << "sinx: " << sinx << std::endl;
    // Create dataset
    std::vector< std::pair< double, double > > dataset_vector = merge( x, sinx );
    std::cerr << "dataset_vector: " << dataset_vector << std::endl;

    // Split dataset

    const auto splitted_dataset = split( dataset_vector, std::make_tuple( 0.7, 0.3 ) );

    const auto & training_res = std::get<0>( splitted_dataset );
    const auto & testing_res = std::get<1>( splitted_dataset );

    //    std::cerr << "training_res: " << training_res.size() << ":: " << training_res << std::endl;
    //    std::cerr << "testing_res: " << testing_res.size() << ":: " << testing_res << std::endl;

    const std::vector< unsigned int > layers_description = { 1, 5, 1 };
    network( layers_description, training_res, testing_res, 20, 0.5, 0.01 );

    /*
    {
        const auto args_2 = merge_to_vector( x, x );

        const auto dataset_vector_1 = merge( args_2, sinx );
        std::cerr << "dataset_vector_1: " << dataset_vector_1 << std::endl;

        const auto splitted_dataset_1 = split( dataset_vector_1, std::make_tuple( 0.7, 0.3 ) );

        const auto & training_res_1 = std::get<0>( splitted_dataset_1 );
        const auto & testing_res_1 = std::get<1>( splitted_dataset_1 );

        std::cerr << "dimension(training_res_1): "  << dimension( training_res_1[ 0 ].first ) << std::endl;

        network( layers_description, training_res_1, testing_res_1, 100 );
    }
    */

    return 0;
}

