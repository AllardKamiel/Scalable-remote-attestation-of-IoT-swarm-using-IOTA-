# Scalable-remote-attestation-of-IoT-swarm-using-IOTA-

This repository consists a master's thesis project about scalebale remote attestation of IoT swarm using IOTA.
All the c code is compiled using Cmake. This makes libary management easy because IOTA only gives a binary libary.

The bash files take one input the size of the IoT swarm. In the bash file the IOTA node url can be changed to make connection to the IOTA tangle.

The network bash file makes an IOTA streams channel with the wanted amount of subscribers.
The network_start bash files starts a sampleproject where the IOT devices each have the same set of attributes and send the same message. In a real IoT network each devices attributes must be changed and the attribute policy for sending the message needs to be changed to. 

The bash files copy and execute the project files in a right order to create the IOTA streams channel an give ach of the subscribers it's own branch on this channel.

##IOTA tangle
You can make your own private tangle by following this link: https://wiki.iota.org/introduction/how_tos/one_click_private_tangle. If you do this on a virtual machine on the school server you will have to do port forwarding to acess the IOTA nodes.

You can also use the public tangle by just deploying a node connected to the real network. This makes it more difficult to look at your network because there is more data traffic. This is done by following the instructions in this link: https://wiki.iota.org/hornet/getting_started. 

## The libaries
### IOTA libary 
This libary contains all of the IOTA functions.

#### import author
This fuction imports the author from a file location and then returns it.

#### export author
This fuction exports the author to a file at the author location.

#### import sub
This fuction imports the sub from a file location and then returns it.

#### export sub
This fuction exports the subscriber to a file at the subscriber location.

#### announce
This function creates an author of a channel on the tangle. It saves the announcement message as a text file in the announcementlocation. The implementation type is 0 for a single branched channel and 1 for a multibranched channel. The tsp is an IOTA channel variable wich defines which node the program should use to access the tangle.

#### subscribe 
This functions makes the subscriber subscribe to a channel. It needs to have the location of the announcement message. 

#### send_keyload
This functions accepts all the subscribers when there adress is in the file located at the subscribers location. After accepting all of the subscribers the author of the channel the author creates a keyload message.

#### keyload_receive_start_branch 
This function lets each subscriber accept the keyload message by giving it the adress in the file at the keyload location. After accepting the keyload the subscriber starts its own branch by adding a streams message to the tangle.

#### security_string
The security string makes the a json string with the nonce and timestamp and deltas. This function calculates the delta. Because the functions was not deployed on a real iot device the hash of the firmware and software is not calculated seperatly but a test value is used to do this a seperate hash calculation of these 2 should be added. This is doe in the same way as is done for the nonce and timstamp. For the firmware the begin adress and size of the firmware must be given and for the software the begin adress and size of the software must be given. 

In this protocol this art is later placed in the public part of the iota streams message.

#### write_last_msg_adress
This functions writes te last message adress and sequende  to a file in the last message adress and sequence location.

#### send_message_on_tangle 
This function sends a streams message on the channel on the IOTA tangle. The public and private part are seperate intputs and the the last message and sequence file locations are neede because the message is linked to this and after publisching the files are updeate with the new adress and sequence of the new message.

#### Securityreceive
This function checks wheter or not you have accss to a send message. This is done by calculating the deltas of the iot device itself and checking them with the deltas in the received public part of the IOTA streams message. If you have access 0 is reeturned otherwise 1 is returned.

#### receive_messages_from_tangle
This function simply fetches all of the unreaded messages from the IOTA streams channel. It uses the security receive function to determine of you have acess to a message or not. if you have access the private part of the message is printed. In the future a JSON can be made where all of the private messgaes you have acccess to are put in and then i returned. Then the IoT device has to act on these messages. 

#### check_nonce_timestamp
This function checks if the given nonce timestamp pair is already in the list. Or if the timestamp value is to low.

#### add_nonce_timestamp
This function adds a nonce timestamp pair to the nonce timestamp pair list. This list is a json list.

#### cleanup_nonce_timestamp 
This function makes sure that the nonce timestamp list does not grow infinitely long by deleting nonce timestamp pairs  that are under the a certain timestamp.

### attribute libary 

#### AND
This function helps making the different attribute lists when there is an AND in the policy.

#### OR
This function helps making the different attribute lsits when there is an OR in the policy.


#### policy_to_array 
The policy_to_array function converts an attribute policy to a cJSON list with all the attributes lists as components of the cJSON.

#### alloptions 
The all otions function creates a cJSON where all of the possible combinations of the devices attributes are put in.

### other libaries
The channels libary is from IOTA and its functions are mainly used in the self-made IOTA libary 
The Cjson libary is a standard libary. This makes it possible to use JSON in c. Be carefule with this libary you have to clean up your objects otherwise you get a segmentation fault. This libary is used to simulate arrays in the code because it makes it easier to access them as a json. 
The sha256 libary contains the code to make a sha256 hash.
