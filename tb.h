#ifndef __TB__H
#define __TB__H

#include <cynw_p2p.h>
#include "CPU_input_type.h"
#include "CPU_output_type.h"

SC_MODULE(tb)
{
    public:
    cynw_p2p < CPU_OUTPUT_DT >::base_in    inputs;
    cynw_p2p < CPU_INPUT_DT >::base_out    outputs;

    sc_in_clk                       clk;
    sc_out < bool >                 rst;
    sc_in < bool >                  rst_in;

    SC_CTOR(tb) :
        inputs("inputs"),
        outputs("outputs"),
        clk("clk"),
        rst("rst"),
        rst_in("rst_in")
    {
        SC_CTHREAD(source, clk.pos());
        SC_CTHREAD(sink, clk.pos());

        // Tells the threads to reset when rst_in is 0
        reset_signal_is(rst_in, 0);
    }

    void source();
    void sink();
};

#endif
