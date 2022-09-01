#making the needed directories
NODE_URL='http://192.168.122.98:14265'
echo $NODE_URL
mkdir network
cd network

mkdir author
cd author
cp -RT  ../../project/author .
cd ..
for (( c=1; c<=$1; c++ ))
do 
    mkdir subscriber$c
    cd subscriber$c
    cp -RT  ../../project/subscriber .
    cd ..
done
cd subscriber1/subscriber_data
AUTH_SUB_DIR_PATH=$(realpath .)
echo $AUTH_SUB_DIR_PATH
cd ../..
# run the announcement message
cd author/author_data
AUTH_DIR_PATH=$(realpath .)
echo $AUTH_DIR_PATH
cd ..
cd announce/build
cmake ../code
cmake --build .
./Tutorial "$AUTH_DIR_PATH" "$AUTH_SUB_DIR_PATH" "$NODE_URL" 
cd ../../..
#run the subscribers subscribe
for (( c=1; c<=$1; c++ ))
do 
    cd subscriber$c/subscriber_data
    SUB_DIR_PATH=$(realpath .)
    cd ../subscriber_subscribe/build
    cmake ../code
    cmake --build .
    ./Tutorial "$AUTH_DIR_PATH" "$SUB_DIR_PATH" "$NODE_URL" 
    cd ../../..
    ls
done

#run author keyload
cd author/keyload/build
cmake ../code
cmake --build .
./Tutorial "$AUTH_DIR_PATH" "$AUTH_SUB_DIR_PATH" "$NODE_URL" 
cd ../../..
#accept keyload and start a branch for each subscriber
for (( c=1; c<=$1; c++ ))
do 
    cd subscriber$c/subscriber_data
    SUB_DIR_PATH=$(realpath .)
    cd ../subscriber_keyload/build
    cmake ../code
    cmake --build .
    ./Tutorial "$AUTH_DIR_PATH" "$SUB_DIR_PATH" "$NODE_URL" 
    cd ../../..
done
