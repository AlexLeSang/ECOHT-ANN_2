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

    /* Sin X
    constexpr unsigned int number_of_points = 500;
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

    auto splitted_dataset = split( dataset_vector, std::make_tuple( 0.7, 0.3 ) );

    const auto & training_res = std::move( std::get<0>( splitted_dataset ) );
    const auto & testing_res = std::move( std::get<1>( splitted_dataset ) );

//        std::cerr << "training_res: " << training_res.size() << ":: " << training_res << std::endl;
//        std::cerr << "testing_res: " << testing_res.size() << ":: " << testing_res << std::endl;

    const std::vector< unsigned int > layers_description = { 1, 20, 1 };
    network< double, double, TanhFunction, TanhFunctionDerivative > ( layers_description, training_res, testing_res, 1e-2, 60000, 0.01, 0.0001 );

//    std::vector< double > x1 = { 0, 0, 1, 1 };
//    std::vector< double > x2 = { 0, 1, 0, 1 };
//    std::vector< double > y1 = { 0, 1, 1, 0 };

    std::vector< double > x1 = { 0, 0 };
    std::vector< double > x2 = { 1, 0 };
    std::vector< double > y1 = { 1, 0 };


    const auto argument = merge_to_vector( x1, x2 );
    std::cerr << "argument: " << argument << std::endl;

    const auto training_dataset = merge( argument, y1 );
    std::cerr << "training_dataset: " << training_dataset << std::endl;

    const std::vector< unsigned int > layers_description = { 2, 4, 1 };
//    typename std::remove_cv <decltype( argument )>::type
    network< std::vector<double>, double, LogisticFunction, LogisticFunctionDerivative > ( layers_description,
                                                                              training_dataset, training_dataset, 1e-4, 9000, 0.9, 0.1 );
    */

    constexpr unsigned int number_of_points = 20;
    constexpr double a = 0.0;
    constexpr double b = 1.0;
    constexpr double step = ( b - a ) / (number_of_points - 1);

    std::vector< double > x( number_of_points );
    for ( unsigned int i = 0; i < number_of_points; ++ i ) {
        x[ i ] = step * i;
    }

    std::cerr << "x: " << x << std::endl;

    std::vector< double > x1( number_of_points );

    std::generate( x1.begin(), x1.end(), std::bind( []( const double step, int & i ) {return step * i++;}, step, 0 ) );
    std::cerr << "x1: " << x1 << std::endl;


    decltype( x ) px1x2( x.size() );
    std::inner_product( x.cbegin(), x.cend(), x1.cbegin(), px1x2.begin() );

    std::cerr << "px1x2: " << px1x2 << std::endl;

    return 0;
}

