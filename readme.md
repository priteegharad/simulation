Directory Structure:

lcgrand.c,lcgrand.h,lcgrand.h.gch

    These files are used in defining exponential distribution generator values for interarrival time of requests, Web and DBMS Server Service Time.



input.txt

    This file has input parameters required for simulation which we designed.



681assign.c++

    This has definitions of simulation class which handles all events of arrival, processing, scheduling and departure of request.



output.txt:

    Results of the simulation are redirected to this file.



Makefile:

    Makefile executes 681assign.c++ with help of input.txt and generates results to output.txt

    



To execute the code,use make command.
