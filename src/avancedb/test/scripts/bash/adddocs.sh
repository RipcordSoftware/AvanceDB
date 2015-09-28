#!/bin/bash

DB=adddocs_test
HOST=127.0.0.1
PORT=5994
URL='http://'${HOST}':'${PORT}'/'${DB}

curl -s -X DELETE ${URL} > /dev/null

curl -s -X PUT ${URL} > /dev/null

for i in {0..999}
do
    curl -s -X PUT ${URL}/`printf "%08d" ${i}` --header 'Content-Type: application/json' \
	--data-binary '{"test":true,"lorem":"ipsum","pi":3.14159,"fibonacci":[0,1,1,3,5,8,13,21,13,21,34,55,89,144],"child":{"ten":10,"hello":"world"},"array":[1,2,3],"null":null}' > /dev/null
done
