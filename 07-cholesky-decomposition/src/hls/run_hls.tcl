##
##  Created by Ratko Pilipovic on 22/11/2023.
##  Copyright © 023 Ratko Pilipovic  UL FRI. All rights reserved.
##

# Create a project
open_project proj-CHO-DEC -reset

# Add design files
add_files ../src/cho_dec.cpp
add_files ../src/cho_dec.h
add_files -tb ../src/hls/cho_dec_tb.cpp


# Set the top-level function
set_top cho_dec

# ########################################################
# Create a solution
open_solution solution-CHO-DEC -reset 

# Define technology and clock rate
set_part {xcu250-figd2104-2L-e}
create_clock -period 10 -name default

# Set variable to select which steps to execute
set hls_exec 1

# Simulate desing, this equals to software emulation
csim_design

#synthesise desing and obtain results 
csynth_design



exit