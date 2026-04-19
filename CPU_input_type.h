#ifndef CPU_INPUT_DT_SC_WRAPPER_TYPE
#define CPU_INPUT_DT_SC_WRAPPER_TYPE 1

#include "cynw_memory.h"

struct CPU_INPUT_DT
{
    //
    // Member declarations.
    //
    sc_uint< 16 > in1;

    typedef sc_uint< 16 > raw_type;

    //
    // Default constructor.
    //
    CPU_INPUT_DT()
    {
    }

    //
    // Copy constructor.
    //
    CPU_INPUT_DT( const CPU_INPUT_DT& other )
    {
        in1 = other.in1;
    }

    //
    // Comparison operator.
    //
    inline bool operator == ( const CPU_INPUT_DT& other )
    {
        if ( !(in1 == other.in1) )
            return false;
        return true;
    }

    //
    // Assignment operator from CPU_INPUT_DT.
    //
    inline CPU_INPUT_DT& operator = ( const CPU_INPUT_DT& other )
    {
        in1 = other.in1;
        return *this;
    }

};
//
// sc_trace function.
//
inline void sc_trace( sc_trace_file* tf, const CPU_INPUT_DT& object, const std::string& in_name )
{
    if (tf)
    {
        tf->trace( object.in1, in_name + std::string(".in1"));
    }
}

//
// stream operator.
//
inline ostream & operator << ( ostream & os, const CPU_INPUT_DT& object )
{
#ifndef STRATUS_HLS
    os << "(";
    os <<  object.in1;
    os << ")";
#endif
    return os;
}

//
// cynw_interpret function to generate a flat vector.
//
inline void cynw_interpret ( CPU_INPUT_DT& from, CPU_INPUT_DT::raw_type& to )
{
    to = (
                from.in1
                );
}

//
// cynw_interpret function to generate a CPU_INPUT_DT from a flat vector.
//
inline void cynw_interpret ( const CPU_INPUT_DT::raw_type& from, CPU_INPUT_DT& to )
{
    cynw_interpret( (sc_uint<16>)from.range( 15,0 ), to.in1 );
}


template <>
struct cynw_sc_wrap< CPU_INPUT_DT >
{
    typedef CPU_INPUT_DT spec;
    typedef CPU_INPUT_DT sc;
};
#endif
