###################################################################################
## This file is distributed as part of the libLCS library.
## libLCS is C++ Logic Circuit Simulation library.
##
## Copyright (c) 2006-2007, B. R. Siva Chandra
##
## This library is free software; you can redistribute it and/or
## modify it under the terms of the GNU Lesser General Public
## License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
##
## In case you would like to contact the author, use the following e-mail
## address: sivachandra_br@yahoo.com
###################################################################################

## ************************************ INFO **************************************
##
## This make file should work as it is on most Linuxes and MinGW/MSYS. You will not 
## need to change anything on these platforms. However, if you would like to build
## libLCS on other platforms, you will need to make the neccesary changes. If are 
## able to build the library successfully on a different platform, kindly pass me 
## the instructions (or a link to the instructions). I shall make them available 
## with the next release of libLCS with due acknowledgements.
## 
## ********************************************************************************

## The path/command for the C compiler.
CC = gcc

## The path/command for the C++ compiler.
CPP = g++

## The path/command for the static library archiver.
ARC  = ar

PR_INC = -I.
PR_LNK = -L./lib

OBJS = obj/clock.o obj/line.o obj/fa.o obj/linestate.o obj/simul.o obj/systime.o obj/hiddenmodman.o \
obj/bit.o obj/vcdman.o obj/bus.o

EXAMPLES = 1bit_fulladd_using_gates 4bit_shiftregister_using_Dflipflops \
4bit_counter_using_Dflipflops functional_module_fulladder \
functional_module_4bit_counter gatelevel_module_fulladder \
delayExample-1 delayExample-2 delayExample-3 delayExample-4 \
xor_using_and_or_not simple_cont_assign_example 1bit_fulladd_using_cont_assign \
2to1_mux_using_cont_assign 2to1_mux_using_cont_assign_bitselects not_feedback \
1_bit_fulladd_using_bit_select bit_select_port_arguments 2bit_fulladder_as_hybrid_module

#############################################################################
## .PHONY targets

.PHONY : all
all : lib examples

.PHONY : lib
lib : lib/libLCS.a

.PHONY : examples
examples : $(EXAMPLES)

.PHONY : clean
clean:
	rm lib/*.a bin/*.exe obj/*.o 


##############################################################################
## Example targets

1bit_fulladd_using_gates : bin/1bit_fulladd_using_gates.exe
bin/1bit_fulladd_using_gates.exe : examples/1bit_fulladd_using_gates.cpp lib/libLCS.a
	$(CPP) examples/1bit_fulladd_using_gates.cpp -o bin/1bit_fulladd_using_gates.exe $(PR_INC) $(PR_LNK) -lLCS

4bit_shiftregister_using_Dflipflops : bin/4bit_shiftregister_using_Dflipflops.exe
bin/4bit_shiftregister_using_Dflipflops.exe : examples/4bit_shiftregister_using_Dflipflops.cpp  lib/libLCS.a
	$(CPP) examples/4bit_shiftregister_using_Dflipflops.cpp  -o bin/4bit_shiftregister_using_Dflipflops.exe $(PR_INC) $(PR_LNK) -lLCS

4bit_counter_using_Dflipflops : bin/4bit_counter_using_Dflipflops.exe
bin/4bit_counter_using_Dflipflops.exe : examples/4bit_counter_using_Dflipflops.cpp lib/libLCS.a
	$(CPP) examples/4bit_counter_using_Dflipflops.cpp -o bin/4bit_counter_using_Dflipflops.exe $(PR_INC) $(PR_LNK) -lLCS

functional_module_fulladder : bin/functional_module_fulladder.exe 
bin/functional_module_fulladder.exe : examples/functional_module_fulladder.cpp lib/libLCS.a
	$(CPP) examples/functional_module_fulladder.cpp -o bin/functional_module_fulladder.exe $(PR_INC) $(PR_LNK) -lLCS

functional_module_4bit_counter : bin/functional_module_4bit_counter.exe
bin/functional_module_4bit_counter.exe : examples/functional_module_4bit_counter.cpp lib/libLCS.a
	$(CPP) examples/functional_module_4bit_counter.cpp -o bin/functional_module_4bit_counter.exe $(PR_INC) $(PR_LNK) -lLCS

gatelevel_module_fulladder : bin/gatelevel_module_fulladder.exe 
bin/gatelevel_module_fulladder.exe : examples/gatelevel_module_fulladder.cpp lib/libLCS.a
	$(CPP) examples/gatelevel_module_fulladder.cpp -o bin/gatelevel_module_fulladder.exe $(PR_INC) $(PR_LNK) -lLCS

delayExample-1 : bin/delayExample-1.exe 
bin/delayExample-1.exe : examples/delayExample-1.cpp lib/libLCS.a
	$(CPP) examples/delayExample-1.cpp -o bin/delayExample-1.exe $(PR_INC) $(PR_LNK) -lLCS

delayExample-2 : bin/delayExample-2.exe
bin/delayExample-2.exe : examples/delayExample-2.cpp lib/libLCS.a
	$(CPP) examples/delayExample-2.cpp -o bin/delayExample-2.exe $(PR_INC) $(PR_LNK) -lLCS

delayExample-3 : bin/delayExample-3.exe
bin/delayExample-3.exe : examples/delayExample-3.cpp lib/libLCS.a
	$(CPP) examples/delayExample-3.cpp -o bin/delayExample-3.exe $(PR_INC) $(PR_LNK) -lLCS

delayExample-4 : bin/delayExample-4.exe
bin/delayExample-4.exe : examples/delayExample-4.cpp lib/libLCS.a
	$(CPP) examples/delayExample-4.cpp -o bin/delayExample-4.exe $(PR_INC) $(PR_LNK) -lLCS

xor_using_and_or_not : bin/xor_using_and_or_not.exe
bin/xor_using_and_or_not.exe : examples/xor_using_and_or_not.cpp lib/libLCS.a
	$(CPP) examples/xor_using_and_or_not.cpp -o bin/xor_using_and_or_not.exe $(PR_INC) $(PR_LNK) -lLCS
	
simple_cont_assign_example : bin/simple_cont_assign_example.exe
bin/simple_cont_assign_example.exe : examples/simple_cont_assign_example.cpp lib/libLCS.a
	$(CPP) examples/simple_cont_assign_example.cpp -o bin/simple_cont_assign_example.exe $(PR_INC) $(PR_LNK) -lLCS

1bit_fulladd_using_cont_assign : bin/1bit_fulladd_using_cont_assign.exe
bin/1bit_fulladd_using_cont_assign.exe : examples/1bit_fulladd_using_cont_assign.cpp lib/libLCS.a
	$(CPP) examples/1bit_fulladd_using_cont_assign.cpp -o bin/1bit_fulladd_using_cont_assign.exe $(PR_INC) $(PR_LNK) -lLCS
	
2to1_mux_using_cont_assign : bin/2to1_mux_using_cont_assign.exe
bin/2to1_mux_using_cont_assign.exe : examples/2to1_mux_using_cont_assign.cpp lib/libLCS.a
	$(CPP) examples/2to1_mux_using_cont_assign.cpp -o bin/2to1_mux_using_cont_assign.exe $(PR_INC) $(PR_LNK) -lLCS

2to1_mux_using_cont_assign_bitselects : bin/2to1_mux_using_cont_assign_bitselects.exe
bin/2to1_mux_using_cont_assign_bitselects.exe : examples/2to1_mux_using_cont_assign_bitselects.cpp lib/libLCS.a
	$(CPP) examples/2to1_mux_using_cont_assign_bitselects.cpp -o bin/2to1_mux_using_cont_assign_bitselects.exe $(PR_INC) $(PR_LNK) -lLCS
	
not_feedback : bin/not_feedback.exe
bin/not_feedback.exe : examples/not_feedback.cpp lib/libLCS.a
	$(CPP) examples/not_feedback.cpp -o bin/not_feedback.exe $(PR_INC) $(PR_LNK) -lLCS
	
1_bit_fulladd_using_bit_select : bin/1_bit_fulladd_using_bit_select.exe
bin/1_bit_fulladd_using_bit_select.exe : examples/1_bit_fulladd_using_bit_select.cpp lib/libLCS.a
	$(CPP) examples/1_bit_fulladd_using_bit_select.cpp -o bin/1_bit_fulladd_using_bit_select.exe $(PR_INC) $(PR_LNK) -lLCS
	
bit_select_port_arguments : bin/bit_select_port_arguments.exe
bin/bit_select_port_arguments.exe : examples/bit_select_port_arguments.cpp lib/libLCS.a
	$(CPP) examples/bit_select_port_arguments.cpp -o bin/bit_select_port_arguments.exe $(PR_INC) $(PR_LNK) -lLCS
	
2bit_fulladder_as_hybrid_module : bin/2bit_fulladder_as_hybrid_module.exe
bin/2bit_fulladder_as_hybrid_module.exe : examples/2bit_fulladder_as_hybrid_module.cpp lib/libLCS.a
	$(CPP) examples/2bit_fulladder_as_hybrid_module.cpp -o bin/2bit_fulladder_as_hybrid_module.exe $(PR_INC) $(PR_LNK) -lLCS
	
#################################################################################
## Static Library
	
lib/libLCS.a : $(OBJS)
	$(ARC) -rs lib/libLCS.a $(OBJS)


#################################################################################
## Sources

obj/clock.o : src/clock.cpp lcs/clock.h
	$(CPP) -c src/clock.cpp -o obj/clock.o $(PR_INC)

obj/fa.o : src/fa.cpp lcs/fa.h
	$(CPP) -c src/fa.cpp -o obj/fa.o $(PR_INC)

obj/line.o : src/line.cpp lcs/line.h
	$(CPP) -c src/line.cpp -o obj/line.o $(PR_INC)

obj/linestate.o : src/linestate.cpp lcs/linestate.h
	$(CPP) -c src/linestate.cpp -o obj/linestate.o $(PR_INC)

obj/simul.o : src/simul.cpp lcs/simul.h
	$(CPP) -c src/simul.cpp -o obj/simul.o $(PR_INC)

obj/systime.o : src/systime.cpp lcs/systime.h
	$(CPP) -c src/systime.cpp -o obj/systime.o $(PR_INC)
	
obj/hiddenmodman.o : src/hiddenmodman.cpp lcs/hiddenmodman.h
	$(CPP) -c src/hiddenmodman.cpp -o obj/hiddenmodman.o $(PR_INC)

obj/bit.o : src/bit.cpp lcs/bit.h
	$(CPP) -c src/bit.cpp -o obj/bit.o $(PR_INC)
	
obj/vcdman.o : src/vcdman.cpp lcs/vcdman.h
	$(CPP) -c src/vcdman.cpp -o obj/vcdman.o $(PR_INC)
	
obj/bus.o : src/bus.cpp lcs/bus.h
	$(CPP) -c src/bus.cpp -o obj/bus.o $(PR_INC)

