#ifndef ANNETWORK_HPP
#define ANNETWORK_HPP

#include <iostream>

#include <vector>
#include <functional>
#include <algorithm>
#include <utility>
#include <set>
#include <tuple>
#include <cassert>
#include <chrono>

#include "Utils.hpp"

std::tuple<std::vector<long double> , std::vector<std::vector<long double> > , std::vector<long double> >
inline
training(const std::size_t hidden_neurons,
         const long double hlr,
         const std::size_t epochs,
         const long double desired_accuracy,
         const std::vector< long double > & train_ou,
         const std::vector< std::vector< long double > >& train_in,
         volatile bool * reset)
{


    auto train_inp = train_in;
    auto train_out = train_ou;
    const auto mu_inp = mean( train_inp );
    const auto sigma_inp = stand( train_inp );
    train_inp = ( train_inp - mu_inp[ 0 ] ) / sigma_inp[ 0 ];
    const auto mu_out = mean( train_out );
    const auto sigma_out = stand( train_out );
    train_out = ( train_out - mu_out ) / sigma_out;
    const auto patterns = size( train_inp ).first;

    std::cout << "patterns: " << patterns << std::endl;
    auto bias = ones( patterns );
    train_inp = merge( train_inp, bias );
    const auto inputs = size( train_inp ).second;

    std::vector< long double > err( epochs );

    auto weight_input_hidden = ( randn( inputs, hidden_neurons) - 0.5l ) / 10.0l;
    auto weight_hidden_output = ( randn( hidden_neurons ) - 0.5l ) / 10.0l;

    for( std::size_t i = 0; i < epochs; ++i ) {
        const auto alr = hlr;
        const auto blr = alr / 10.0;
        for( std::size_t j = 0; j < patterns; ++j ){
            const auto patnum = ( static_cast<std::size_t>( round( randd() * patterns + 0.5 ) ) - 1 ) % patterns;
            const auto this_pat = train_inp[ patnum ];
            const auto act = train_out[ patnum ];
            const auto hval = feval( []( const long double & v ){ return std::tanh( v ); }, this_pat * weight_input_hidden );
            const auto pred = hval * weight_hidden_output;
            const auto error = pred - act;
            const auto delta_HO = hval * error * blr;
            weight_hidden_output = weight_hidden_output - delta_HO;
            const auto m1 = weight_hidden_output * alr * error;
            const auto m2 = 1.0l - (hval^2);
            const auto m3 = dot_operator( m1, m2, std::multiplies< long double >());
            const auto m4 = vec_to_vecvec( m3 );
            const auto delta_IH = m4 * this_pat;
            weight_input_hidden = weight_input_hidden - trans( delta_IH );
        }
        const auto p1 = feval( []( const long double& v ){ return std::tanh( v ); }, train_inp * weight_input_hidden );
        const auto pred = weight_hidden_output * trans( p1 );
        const auto error = pred - train_out;
        const auto error_sq = error ^ 2;
        err[ i ] = std::sqrt( std::accumulate( error_sq.cbegin(), error_sq.cend(), 0.0, std::plus<long double> () ) );
        std::cerr << "err[ " <<  i << " ]: " << err[ i ] << ' ' << i <<  std::endl;

        if ( *reset ) break;
        if ( err[ i ] <= desired_accuracy ) break;
    }
    return std::move(std::make_tuple(weight_hidden_output, weight_input_hidden, err));
}

std::vector< long double >
inline
eval(const std::vector<std::vector<long double> >& train_test,
     const std::vector<long double>& weight_hidden_output,
     const std::vector<std::vector<long double>> & weight_input_hidden)
{
    auto pred = weight_hidden_output * trans( feval( []( const long double & v) { return std::tanh( v ); }, train_test * weight_input_hidden ) );
    return std::move(pred);
}

std::vector< long double >
inline
test_error(const std::vector< long double > & test_out,
           const std::vector< long double > & pred,
           const std::vector< long double > & train_out)
{
    const auto a = test_out;//(train_out * sigma_out) + mu_out;
    const auto mu_out = mean( train_out );
    const auto sigma_out = stand( train_out );
    const auto b = (pred * sigma_out) + mu_out;
    const auto act_pred_err = feval( []( const long double & v ){ return std::abs( v ); }, b - a );

    const auto c = std::accumulate( act_pred_err.cbegin(), act_pred_err.cend(), 0.0, std::plus< long double >() ) / act_pred_err.size();
    const auto cc = std::max_element( act_pred_err.cbegin(), act_pred_err.cend() );
    std::cout << "Average arror: " << c << std::endl;
    std::cout << "Max error: " << *cc << std::endl;
    return act_pred_err;
}
#endif // ANNETWORK_HPP
