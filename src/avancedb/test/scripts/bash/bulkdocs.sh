#!/bin/bash

DB=bulkdocs_test
HOST=127.0.0.1
PORT=5994
URL='http://'${HOST}':'${PORT}'/'${DB}
FILE=/tmp/bulkdocs.json

curl -s -X DELETE ${URL} > /dev/null

curl -s -X PUT ${URL} > /dev/null

ID=0
for i in {0..99}
do
    echo '{"docs":[' > ${FILE}
    for j in {0..99}
    do

        DATA='{"_id":"'`printf "%08d" ${ID}`'","test":true,"lorem":"ipsum","pi":3.14159,"fibonacci":[0,1,1,3,5,8,13,21],"index":'${ID}'},'
        let ID++
        DATA=${DATA}'{"_id":"'`printf "%08d" ${ID}`'","test":true,"lorem":"ipsum","pi":3.14159,"fibonacci":[0,1,1,3,5,8,13,21],"index":'${ID}'},'
        let ID++
        DATA=${DATA}'{"_id":"'`printf "%08d" ${ID}`'","test":true,"lorem":"ipsum","pi":3.14159,"fibonacci":[0,1,1,3,5,8,13,21],"index":'${ID}'},'
        let ID++
        DATA=${DATA}'{"_id":"'`printf "%08d" ${ID}`'","test":true,"lorem":"ipsum","pi":3.14159,"fibonacci":[0,1,1,3,5,8,13,21],"index":'${ID}'},'
        let ID++
        DATA=${DATA}'{"_id":"'`printf "%08d" ${ID}`'","test":true,"lorem":"ipsum","pi":3.14159,"fibonacci":[0,1,1,3,5,8,13,21],"index":'${ID}'},'
        let ID++
        DATA=${DATA}'{"_id":"'`printf "%08d" ${ID}`'","test":true,"lorem":"ipsum","pi":3.14159,"fibonacci":[0,1,1,3,5,8,13,21],"index":'${ID}'},'
        let ID++
        DATA=${DATA}'{"_id":"'`printf "%08d" ${ID}`'","test":true,"lorem":"ipsum","pi":3.14159,"fibonacci":[0,1,1,3,5,8,13,21],"index":'${ID}'},'
        let ID++
        DATA=${DATA}'{"_id":"'`printf "%08d" ${ID}`'","test":true,"lorem":"ipsum","pi":3.14159,"fibonacci":[0,1,1,3,5,8,13,21],"index":'${ID}'},'
        let ID++
        DATA=${DATA}'{"_id":"'`printf "%08d" ${ID}`'","test":true,"lorem":"ipsum","pi":3.14159,"fibonacci":[0,1,1,3,5,8,13,21],"index":'${ID}'},'
        let ID++
        DATA=${DATA}'{"_id":"'`printf "%08d" ${ID}`'","test":true,"lorem":"ipsum","pi":3.14159,"fibonacci":[0,1,1,3,5,8,13,21],"index":'${ID}'},'
        let ID++
        
        echo ${DATA} >> ${FILE}
    done
    
    echo ']}' >> ${FILE}

    curl -0 -s -X POST ${URL}/_bulk_docs --header 'Content-Type: application/json' --header "Expect: " --data-binary @${FILE}
done
