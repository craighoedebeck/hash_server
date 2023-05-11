# hash_server
Server for taking in newline terminated strings, and returning the hash of the string as ascii text.

## Building
This was build and tested on Ubuntu 20.04.  Cmake is used to build.  If you wish to build natively do the following:

cmake .
make

You will need openssl development libraries, boost, and google test in order to build.

After successfully building, you should have a unit test executable called hash_tests and a server utility called hash_server.  Both run without any command line arguments.

For simplicity, the server runs in a static port bound to 6060.  After running, you should be able to connect into this port from telnet or netcat and send the input of your choice.

## Building in a docker
If you wish to build and test in a docker container you can run the build_in_docker.sh command in lieu of the above commands.  This creates a hash_server image with the server and unit tests built appropriately.

If you wish to run the unit tests in the docker, you can run the following command:
./run_test_in_docker.sh

If you wish to run the server in the docker, you can run the following command:
./run_server_in_docker.sh

The run server script forwards port 6060 to your host machine, but you may have issues connecting in depending on host OS and firewall issues.  If you want to connect into the docker interactively to test, you can do so with the following command after running build_in_docker.sh
docker run -it hash_server

the nc command should be available in the docker to then test the server however you would like.  You can run the server and the unit tests from here using the hash_server and hash_tests commands detailed above.
