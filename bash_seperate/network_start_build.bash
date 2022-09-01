cd ..
cd network
NODE_URL='http://192.168.122.98:14265'
echo $NODE_URL
cd author/author_data
AUTH_DIR_PATH=$(realpath .)
echo $AUTH_DIR_PATH

cd ../..


for (( c=1; c<=$1; c++ ))
do 
    cd subscriber$c/subscriber_data
    SUB_DIR_PATH=$(realpath .)
    cd ../subscriber_operation/build
    cmake ../code
    cmake --build .
    cd ../../..
done


