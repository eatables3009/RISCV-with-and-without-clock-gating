
#ifndef __CPU__H
#define __CPU__H

#include "cynw_p2p.h"

#include "CPU_input_type.h"
#include "CPU_output_type.h"

SC_MODULE(CPU)
{
    public:
    cynw_p2p < CPU_INPUT_DT  >::in     inputs;
    cynw_p2p < CPU_OUTPUT_DT >::out    outputs;
    
    // Declaration of clock and reset parameters
    sc_in_clk               clk;
    sc_in < bool >          rst;
    SC_CTOR(CPU):inputs("inputs"), outputs("outputs"), clk("clk"), rst("rst")
    {
        SC_CTHREAD(thread1, clk.pos());
        reset_signal_is(rst,0);
        
        // Connect the clk and rst signals to the metaports
        inputs.clk_rst(clk, rst);
        outputs.clk_rst(clk, rst);
    }
    void thread1();
    
    CPU_OUTPUT_DT my_function(CPU_INPUT_DT);
};

#endif

