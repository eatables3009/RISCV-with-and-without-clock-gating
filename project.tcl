######################################################################
# Technology library used for gate-level timing estimation.
######################################################################
use_tech_lib    "/home/install/STRATUS202/share/stratus/techlibs/GPDK045/gsclib045_svt_v4.4/gsclib045/timing/slow_vdd1v0_basicCells.lib"

######################################################################
# c++ compiler options
######################################################################
set CLOCK_PERIOD 20 ;# Time units are default time units of the library
set_attr cc_options " -DCLOCK_PERIOD=$CLOCK_PERIOD"
set_attr hls_cc_options " -DCLOCK_PERIOD=$CLOCK_PERIOD"

######################################################################
# Stratus options for all hls_configs
######################################################################
set_attr clock_period           $CLOCK_PERIOD
set_attr message_detail         2
set_attr cycle_slack            [expr  $CLOCK_PERIOD * 0.95] ;# Leave 5 percent margin between scheduling and allocation
set_attr path_delay_limit       105 ;# Set the limit for allocation 
set_attr rtl_annotation	 	op,stack
set_attr flatten_arrays         all ;# Flatten by default, then map specific arrays
set_attr dpopt_auto             op ;# Create custom parts for ops containing constants

######################################################################
# System Level (testbench) Modules 
######################################################################
define_system_module main.cpp
define_system_module system.cpp
define_system_module tb.cpp

######################################################################
# Modules to be synthesized
######################################################################
define_hls_module CPU CPU.cpp 

######################################################################
# HLS configurations with different constraints
######################################################################
define_hls_config CPU BASIC  ;# No additional options
define_hls_config CPU GATING --power_clock_gating=on

######################################################################
# Genus Logic Synthesis Configurations
######################################################################
define_logic_synthesis_config G {CPU -all} 

######################################################################
# Simulation Settings
######################################################################
use_verilog_simulator xcelium ;# 'xcelium', 'incisive', 'mti' or 'vcs' 
enable_waveform_logging -vcd ;# to store signal transitions shm, vcd, or fsdb
set_attr end_of_sim_command "make cmp_result"

######################################################################
# Define a behavioral simulation configuration for each hls_config
######################################################################
foreach config [find -hls_config *] {
  set cname [get_attr name $config]
  define_sim_config ${cname}_B "CPU BEH $cname"
}

######################################################################
# Define an RTL simulation configuration for each hls_config
# and a power analysis config for each RTL simulation config
######################################################################
foreach config [find -hls_config *] {
  set cname [get_attr name $config]
  define_sim_config ${cname}_V "CPU RTL_V $cname"
  define_power_config P_${cname} ${cname}_V -module CPU -command bdw_runsjoules
}



