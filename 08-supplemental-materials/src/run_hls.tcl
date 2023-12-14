

# Create a project
open_project proj-vadd -reset

# Add design files
add_files ../src/vadd.cpp
add_files ../src/vadd.h
add_files -tb ../src/vadd_tb.cpp


# Set the top-level function
set_top vadd

# ########################################################
# Create a solution
open_solution solution -reset 

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