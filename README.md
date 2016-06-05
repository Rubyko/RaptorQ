# RaptorQ


You can use the library as follows:

1) Add the header file rq.h to your project;

2) In the rq.h there are only two functions are available for the users:

create_encoding_packet_producer and create_encoding_packet_consumer

3) To generate the packets you use packet_producer class with its relevant method: produce

4) To consume the packets you use packet_consumer class with its relevant method: consume

5) Check if the data is decoded using is_data_ready() method

6) get your data by providing iterators using get_data method.

PLEASE LOOK AT THE COMPETE.CPP IN TEST FOLDER FOR MORE INFORMATION


ALSO, IS THERE SOMEONE WHO WANTS TO CONTRIBUTE TO THIS PROJECT ? I WOULD REALLY APPRECIATE ANY HELP.
