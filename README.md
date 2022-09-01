# Scalable-remote-attestation-of-IoT-swarm-using-IOTA-

This repository consists a master's thesis project about scalebale remote attestation of IoT swarm using IOTA.
All the c code is compiled using Cmake. This makes libary management easy because IOTA only gives a binary libary.

The bash files take one input the size of the IoT swarm. In the bash file the IOTA node url can be changed to make connection to the IOTA tangle.

The network bash file makes an IOTA streams channel with the wanted amount of subscribers.
The network_start bash files starts a sampleproject where the IOT devices each have the same set of attributes and send the same message. In a real IoT network each devices attributes must be changed and the attribute policy for sending the message needs to be changed to. 

## The libaries
### IOTA libary 
This libary contains all of the IOTA functions.
#### import author
This fuction imports the author from a file location and then returns it.
#### export author
This fuction exports the author to a filesdsffssfsf
#### import sub
This fuction imports the sub from a file location and then returns it.
#### export sub
This fuction exports the subscriber.
#### announce
This function creates an author of a channel on the tangle. It saves the announcement message as a text file in the announcementlocation. The implementation type is 0 for a single branched channel and 1 for a multibranched channel. The tsp is an IOTA channel variable wich defines which node the program should use to access the tangle.
#### subscribe 
This functions makes the subscriber subscribe to a channel. It needs to have the location of the announcement message. 
#### send_keyload
#### keyload_receive_start_branch 
#### security_string
#### write_last_msg_adress
#### send_message_on_tangle 
#### Securityreceive
#### receive_messages_from_tangle
#### check_nonce_timestamp
#### add_nonce_timestamp
#### cleanup_nonce_timestamp 

### attribute libary 
#### AND
#### OR
#### policy_to_array 
#### alloptions 

### other libaries
The channels libary is from IOTA and its functions are mainly used in the self-made IOTA libary 
The Cjson libary is a standard libary. This makes it possible to use JSON in c. Be carefule with this libary you have to clean up your objects otherwise you get a segmentation fault. This libary is used to simulate arrays in the code because it makes it easier to access them as a json. 
The sha256 libary contains the code to make a sha256 hash.
